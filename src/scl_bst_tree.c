/**
 * @file scl_bst_tree.c
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2022-06-21
 * 
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of C-language-Data-Structures.
 *
 * C-language-Data-Structures is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C-language-Data-Structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with C-language-Data-Structures.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "./include/scl_bst_tree.h"
#include "./include/scl_queue.h"

/**
 * @brief Create a bst object. Allocation may fail if there
 * is not enough memory on heap or cmp function is not valid
 * (data arranges in bst tree by comparation) in this case an exception will be thrown.
 * 
 * @param cmp pointer to a function to compare two sets of data
 * @param frd pointer to a function to free content of one data
 * @param data_size length in bytes of the data data type
 * @return bst_tree_t* a new allocated binary search tree or NULL (if function failed)
 */
bst_tree_t* create_bst(compare_func cmp, free_func frd, size_t data_size) {
    /* Check if cmp function is valid */
    if (NULL == cmp) {
        errno = EINVAL;
        perror("Compare function undefined for binary search tree");
        return NULL;
    }

    /* Check if data size is valid */
    if (0 == data_size) {
        errno = EINVAL;
        perror("Data size at creation is zero");
        return NULL;
    }

    /* Allocate a new binary search tree on heap */
    bst_tree_t *new_tree = malloc(sizeof(*new_tree));

    /* Check if binary search tree was allocated */
    if (NULL != new_tree) {

        /* Set function pointers */
        new_tree->cmp = cmp;
        new_tree->frd = frd;

        /* Create `nil` node */
        new_tree->nil = malloc(sizeof(*new_tree->nil));

        /* Set default values for a `nil` cell*/
        if (NULL != new_tree->nil) {
            new_tree->nil->data = NULL;
            new_tree->nil->count = 1;
            new_tree->nil->left = new_tree->nil->right = new_tree->nil;
            new_tree->nil->parent = new_tree->nil;
        } else {
            errno = ENOMEM;
            perror("Not enough memory for nil red-black allocation");
        }

        /* Set root and size of the binary search tree */
        new_tree->root = new_tree->nil;
        new_tree->data_size = data_size;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for bst allocation");
    }
    
    /* Return a new allocated binary search tree or `NULL` */
    return new_tree;
}

/**
 * @brief Create a bst node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return `nil` and an exception
 * will be thrown.
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data
 * @return bst_tree_node_t* a new allocated binary search tree node object or `nil`
 */
static bst_tree_node_t* create_bst_node(const bst_tree_t * const __restrict__ tree, const void * __restrict__ data) {
    /* Check if data address is valid */
    if (NULL == data) {
        return tree->nil;
    }

    /* Allocate a new node on the heap */
    bst_tree_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if allocation went successfully */
    if (NULL != new_node) {

        /* Set default node data */
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;

        /* Allocate heap memory for data */
        new_node->data = malloc(tree->data_size);

        /* Check if memory allocation went right */
        if (NULL != new_node->data) {

            /*
             * Copy all bytes from data pointer
             * to memory allocated on heap
             */
            memcpy(new_node->data, data, tree->data_size);
        } else {
            free(new_node);
            new_node = tree->nil;

            errno = ENOMEM;
            perror("Not enough memory for node bst data allocation");
        }
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node bst allocation");
    }

    /* Return a new binary search tree node object or `nil` */
    return new_node;
}

/**
 * @brief A helper function for free_bst function.
 * Function will iterate through all nodes recursively
 * by Left-Right-Root principle.
 * 
 * @param tree an allocated binary search tree object
 * @param root pointer to pointer to current bst node object
 */
static void free_bst_helper(const bst_tree_t * const __restrict__ tree, bst_tree_node_t ** const __restrict__ root) {
    /* Check if current node is valid */
    if (tree->nil == *root) {
        return;
    }

    /* Recursive calls */
    free_bst_helper(tree, &(*root)->left);
    free_bst_helper(tree, &(*root)->right);

    /* Free content of the data pointer */
    if ((NULL != tree->frd) && (NULL != (*root)->data)) {
        tree->frd((*root)->data);
    }

    /* Free data pointer */
    if (NULL != (*root)->data) {
        free((*root)->data);
    }

    /* Set data pointer as NULL */
    (*root)->data = NULL;

    /* Free bst node pointer */
    if (tree->nil != *root) {
        free(*root);

        *root = tree->nil;
    }
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * binary search tree object. The function will iterate through all nodes
 * and will free the data content according to frd function provided
 * by user at creation of binary search tree, however if no free function
 * was provided it means that data pointer does not contain any dynamically
 * allocated elements.
 * 
 * @param tree an allocated binary search tree object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_bst(bst_tree_t *const __restrict__ tree) {
    /* Check if tree needs to be freed */
    if (NULL != tree) {

        /* Free every node from bst -> tree */
        free_bst_helper(tree, &tree->root);
        
        /* Free `nil` cell*/
        free(tree->nil);

        tree->nil = NULL;

        /* Free binary search tree object */
        free(tree);

        return SCL_OK;
    }

    return SCL_NULL_BST;
}

/**
 * @brief Function to insert one generic data to a bst.
 * Function may fail if bst or data os not valid (have
 * address NULL) or not enough heap memory is left. You
 * CANNOT insert different data types into bst tree, this
 * will evolve into an uknown behavior or segmentation fault.
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_insert(bst_tree_t * const __restrict__ tree, const void * __restrict__ data) {
    /* Check if tree and data are valid */
    if (NULL == tree) {
        return SCL_NULL_BST;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Set iterator pointers   */
    bst_tree_node_t *iterator = tree->root;
    bst_tree_node_t *parent_iterator = tree->nil;

    /* Find a valid position for insertion */
    while (tree->nil != iterator) {
        parent_iterator = iterator;

        if (tree->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else if (tree->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else {

            /*
             * Node already exists in current bst tree
             * increment count value of node
             */
            ++(iterator->count);
            return 0;
        }
    }

    /* Create a new bst node object */
    bst_tree_node_t *new_node = create_bst_node(tree, data);

    /* Check if new bst node was created */
    if (tree->nil == new_node) { 
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
        
    if (tree->nil != parent_iterator) {

        /* Update parent links */
        new_node->parent = parent_iterator;

        /* Update children links */
        if (tree->cmp(parent_iterator->data, new_node->data) >= 1) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }
    } else {

        /* Created node is root node */
        tree->root = new_node;
    }

    /* Increase bst tree size */
    ++(tree->size);

    /* Insertion in bst went successfully */
    return SCL_OK;
}

/**
 * @brief Function to search data in binary search tree O(log h).
 * Function will start searching from bst tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return bst_tree_node_t* binary search tree node object containing
 * data value or `nil` in case no such node exists
 */
static bst_tree_node_t* bst_find_node(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    /* Set iterator pointer */
    bst_tree_node_t *iterator = tree->root;

    /* Search for imput data (void *data) in all tree */
    while (tree->nil != iterator) {
        if (tree->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else if (tree->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    /* Data was not found */
    return tree->nil;
}

/**
 * @brief Function to search data in binary search tree O(log h).
 * Function will start searching from bst tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return const void* binary search tree data node object containing
 * data value or NULL in case no such node exists
 */
const void* bst_find_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* 
     * Find node according to data pointer and return pointer
     * to location of the data from  node or `NULL` if node is `nil`
     */
    return bst_find_node(tree, data)->data;
}

/**
 * @brief Function to swap two nodes from an bst tree object.
 * This function MUST NOT be used by users, because it will
 * break the proprety of binary search tree, it is used by program
 * to delete nodes from binary search tree and must be used just
 * in delete function provided by the program.
 * 
 * @param tree an allocated binary search tree object
 * @param dest_node bst node object to rewrite data bytes from src_node
 * @param src_node bst node object to copy data bytes
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t bst_swap_nodes(bst_tree_t * const __restrict__ tree, bst_tree_node_t * const __restrict__ dest_node, bst_tree_node_t * const __restrict__ src_node) {
    /* Check if swap is posible */
    if ((tree->nil == dest_node) || (tree->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    /* Interchange the right child */

    bst_tree_node_t *temp = dest_node->right;

    dest_node->right = src_node->right;

    if (tree->nil != dest_node->right) {
        dest_node->right->parent = dest_node;
    }

    src_node->right = temp;

    if (tree->nil != src_node->right) {
        src_node->right->parent = src_node;
    }

    /* Interchange the left child */

    temp = dest_node->left;

    dest_node->left = src_node->left;

    if (tree->nil != dest_node->left) {
        dest_node->left->parent = dest_node;
    }

    src_node->left = temp;

    if (tree->nil != src_node->left) {
        src_node->left->parent = src_node;
    }

    /* Interchange parents of the two nodes */

    temp = dest_node->parent;

    dest_node->parent = src_node->parent;

    if (tree->nil != dest_node->parent) {
        if (dest_node->parent->left == src_node) {
            dest_node->parent->left = dest_node;
        } else {
            dest_node->parent->right = dest_node;
        }
    } else {
        tree->root = dest_node;
    }

    src_node->parent = temp;

    if (tree->nil != src_node->parent) {
        if (src_node->parent->left == dest_node) {
            src_node->parent->left = src_node;
        } else {
            src_node->parent->right = src_node;
        }
    } else {
        tree->root = src_node;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in bst tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param tree an allocated binary search tree object
 * @param base_node bst node object to calculate its level
 * @return int32_t level of input bst object node
 */
static int32_t bst_node_level(const bst_tree_t * const __restrict__ tree, const bst_tree_node_t * __restrict__ base_node) {
    /* Check if input data is valid */
    if (tree->nil == base_node) {
        return -1;
    }

    /* Set level of node as -1 */
    int32_t level_count = -1;

    /* Compute level of input node */
    while (tree->nil != base_node) {
        base_node = base_node->parent;
        ++level_count;
    }

    /* Return node level */
    return level_count;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in bst tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to a value type to find level of node
 * containing current data
 * @return int32_t level of input bst object node
 */
int32_t bst_data_level(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return -1;
    }

    /* Return node level of the node according to data pointer */
    return bst_node_level(tree, bst_find_node(tree, data));
}

/**
 * @brief Function to check whether a binary
 * searc tree object is empty or not.
 * 
 * @param tree an allocated binary search tree
 * @return uint8_t 1 if bst tree is empty or not allocated
 * 0 if it is not empty
 */
uint8_t is_bst_empty(const bst_tree_t * const __restrict__ tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Function to get root data node of the bst tree.
 * 
 * @param tree an allocated binary search tree object
 * @return const void* the root data node of the current binary
 * search tree
 */
const void* get_bst_root(const bst_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return NULL;
    }

    return tree->root->data;
}

/**
 * @brief Function to get size of the bst tree.
 * 
 * @param tree an allocated binary search tree object
 * @return size_t size of the current bst tree or SIZE_MAX
 */
size_t get_bst_size(const bst_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return SIZE_MAX;
    }

    return tree->size;
}

/**
 * @brief Function to get node with maximum data value.
 * Function will search the maximum considering root node
 * as the beginning of the tree (root != tree(root)).
 * 
 * @param tree an allocated binary search tree object
 * @param root pointer to current working bst node object
 * @return bst_tree_node_t* pointer to maximum node value from bst
 */
static bst_tree_node_t* bst_max_node(const bst_tree_t * const __restrict__ tree, bst_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->right) {
            root = root->right;
        }
    }

    return root;
}

/**
 * @brief Function to get node with minimum data value.
 * Function will search the minimum considering root node
 * as the beginning of the tree (root != tree(root)).
 * 
 * @param tree an allocated binary search tree object
 * @param root pointer to current working bst node object
 * @return bst_tree_node_t* pointer to minimum node value from bst
 */
static bst_tree_node_t* bst_min_node(const bst_tree_t * const __restrict__ tree, bst_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

/**
 * @brief Function to get the maximum data value from bst.
 * Function will search the maximum data considering subroot
 * data node as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated binary search tree object
 * @param subroot_data pointer to a data value that represents a node
 * to start searcing for maximum node
 * @return const void* pointer to maximum data value from bst tree
 */
const void* bst_max_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get maximum data from binary search tree or `NULL` is node is `nil` */
    return bst_max_node(tree, bst_find_node(tree, subroot_data))->data;
}

/**
 * @brief Function to get the minimum data value from bst.
 * Function will search the minimum data considering sub root
 * data node as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated binary search tree object
 * @param subroot_data pointer to a data value that represents a node
 * to start searcing for minimum node
 * @return const void* pointer to minimum data value from bst tree
 */
const void* bst_min_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get minimum data from binary search or `NULL` is node is `nil` */
    return bst_min_node(tree, bst_find_node(tree, subroot_data))->data;
}

/**
 * @brief Function to delete one generic data from a bst.
 * Function may fail if input data is not valid or if
 * changing the data fails. You can delete one data at a time
 * and MUST specify a valid bst tree and a valid data pointer
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data to be deleted
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_delete(bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_BST;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Find current node (root) in binary search tree */
    bst_tree_node_t *delete_node = bst_find_node(tree, data);

    /* Bst node was not found exit process */
    if (tree->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Delete selected node */
    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {

        /* Selected node has two children */

        /* Find a replacement for selected node */
        bst_tree_node_t *delete_succecessor = bst_min_node(tree, delete_node->right);
                
        /* Replace the selected bst node and remove the dublicate */
        scl_error_t err = bst_swap_nodes(tree, delete_node, delete_succecessor);
        
        if (SCL_OK != err) {
            return err;
        }
    }

    /* Selected node has one or no chlid */
    if (tree->nil != delete_node->left) {

        /* Selected node has a left child */

        /* Update child-grandparent links */
        delete_node->left->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {

            /* Update grandparent-child links */

            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node->left;
            } else {
                delete_node->parent->left = delete_node->left;
            }
        } else {

            /*
                * Selected node was root
                * Update a new root
                */
            tree->root = delete_node->left;
        }
    } else if (tree->nil != delete_node->right) {
        /* Selected node has a right child */

        /* Update child-grandparent links */
        delete_node->right->parent = delete_node->parent;

        if (tree->nil != delete_node->parent) {

            /* Update grandparent-child links */

            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node->right;
            } else {
                delete_node->parent->left = delete_node->right;
            }
        } else {

            /*
                * Selected node was root
                * Update a new root
                */
            tree->root = delete_node->right;
        }
    } else {

        /* Selected node has no children */

        /* Update grandparent links */
        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = tree->nil;
            } else {
                delete_node->parent->left = tree->nil;
            }
        } else {

            /*
                * Selected node was root
                * Update new root to `nil`
                */
            tree->root = tree->nil;
        }
    }

    /* Free content of the data pointer */
    if ((NULL != tree->frd) && (NULL != delete_node->data)) {
        tree->frd(delete_node->data);
    }

    /* Free data pointer of selected node */
    if (NULL != delete_node->data) {
        free(delete_node->data);
    }

    /* Set data pointer as `NULL` */
    delete_node->data = NULL;

    /* Free selected bst node pointer */
    if (tree->nil != delete_node) {
        free(delete_node);
    }

    /* Set selected bst node as `nil` */
    delete_node = tree->nil;

    /* Deacrease tree size  */
    --(tree->size); 

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to search the inorder predecessor for
 * a specified data type value. Function may fail if
 * bst tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the bst tree does not contain specified data pointer
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return bst_tree_node_t* nil or inorder predecessor of the
 * node containing (void *data) value.
 */
static bst_tree_node_t* bst_predecessor_node(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    bst_tree_node_t *iterator = bst_find_node(tree, data);

    /* If node is not in bst than return `nil` */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a left child than
     * find predecessor in left subtree
     */
    if (tree->nil != iterator->left) {
        return bst_max_node(tree, iterator->left);
    }

    /* Set parent iterator */
    bst_tree_node_t *parent_iterator = iterator->parent;

    /* Find predecessor node */
    while ((tree->nil != parent_iterator) && (parent_iterator->left == iterator)) {
        iterator = parent_iterator;
        parent_iterator = parent_iterator->parent;
    }

    /* Return predecessor node of the data value node */
    return parent_iterator;
}

/**
 * @brief Function to search the inorder successor for
 * a specified data type value. Function may fail if
 * bst tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the bst tree does not contain specified data pointer
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return bst_tree_node_t* nil or inorder successor of the
 * node containing (void *data) value.
 */
static bst_tree_node_t* bst_successor_node(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    bst_tree_node_t *iterator = bst_find_node(tree, data);

    /* If node is not in bst than return `nil` */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a right child than
     * find successor in right subtree
     */
    if (tree->nil != iterator->right) {
        return bst_min_node(tree, iterator->right);
    }

    /* Set parent iterator */
    bst_tree_node_t *parent_iterator = iterator->parent;

    /* Find successor node */
    while ((tree->nil != parent_iterator) && (parent_iterator->right == iterator)) {
        iterator = parent_iterator;
        parent_iterator = parent_iterator->parent;
    }

    /* Return successor node of the data value node */
    return parent_iterator;
}

/**
 * @brief Function to search the inorder predecessor for
 * a specified data type value. Function may fail if
 * bst tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the bst tree does not contain specified data pointer
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return const void* NULL or pointer to data of inorder 
 * predecessor of the node containing (void *data) value.
 */
const void* bst_predecessor_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the predecessor data or `NULL` if node is `nil` */
    return bst_predecessor_node(tree, data)->data;
}

/**
 * @brief Function to search the inorder successor for
 * a specified data type value. Function may fail if
 * bst tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the bst tree does not contain specified data pointer
 * 
 * @param tree an allocated binary search tree object
 * @param data pointer to an address of a generic data type
 * @return const void* NULL or pointer to data of inorder
 * successor of the node containing (void *data) value.
 */
const void* bst_succecessor_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the successor data or `NULL` if node is `nil` */
    return bst_successor_node(tree, data)->data;
}

/**
 * @brief Function to get the lowest common ancestor
 * node of the two specified nodes that contain
 * as data types value (data1 and data2). Function may
 * fail if tree and both data pointers are not allocated
 * and also function may fail if the nodes are not from the
 * current working tree.
 * 
 * @param tree an allocated binary search tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return bst_tree_node_t* pointer to a bst node object that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
static bst_tree_node_t* bst_lowest_common_ancestor_node(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return tree->nil;
    }

    /* Check if both nodes are in the current working binary search tree */
    if ((tree->nil == bst_find_node(tree, data1)) || (tree->nil == bst_find_node(tree, data2))) {
        return tree->nil;
    }

     /* Set iterator pointer */
    bst_tree_node_t *iterator = tree->root;

    /* Find the lowest common ancestor */
    while (tree->nil != iterator) {
        if ((tree->cmp(iterator->data, data1) >= 1) && (tree->cmp(iterator->data, data2) >= 1)) {
            iterator = iterator->left;
        } else if ((tree->cmp(iterator->data, data1) <= -1) && (tree->cmp(iterator->data, data2) <= -1)) {
            iterator = iterator->right;
        } else {

            /* Ancestor found */
            return iterator;
        }
    }

    /* Function failed */
    return tree->nil;
}

/**
 * @brief Function to get the lowest common ancestor
 * data value of the two specified nodes that contain
 * as data types value (data1 and data2). Function may
 * fail if tree and both data pointers are not allocated
 * and also function may fail if the nodes are not from the
 * current working tree.
 * 
 * @param tree an allocated binary search tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return const void* pointer to a bst node object data that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
const void* bst_lowest_common_ancestor_data(const bst_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return NULL;
    }

    /* Get the lowest common ancestor data or `NULL` if node is `nil` */
    return bst_lowest_common_ancestor_node(tree, data1, data2)->data;
}

/**
 * @brief Helper function for bst_traverse_inorder function.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param tree an allocated binary search tree object
 * @param root starting point of the binary search tree traversal
 * @param action a pointer function to perform an action on one bst node object
 */
static void bst_traverse_inorder_helper(const bst_tree_t * const __restrict__ tree, const bst_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working bst node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    bst_traverse_inorder_helper(tree, root->left, action);
    
    /* Call action function */
    action(root->data);

    /* Traverse in the right sub-tree */
    bst_traverse_inorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in inorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the bst prroperty)
 * 
 * @param tree current working binary search tree object
 * @param action a pointer to a function that will perform an action
 * on every bst node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_traverse_inorder(const bst_tree_t * const __restrict__ tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        bst_traverse_inorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for bst_traverse_preorder function.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param tree an allocated binary search tree object
 * @param root starting point of the binary search tree traversal
 * @param action a pointer function to perform an action on one bst node object
 */
static void bst_traverse_preorder_helper(const bst_tree_t * const __restrict__ tree, const bst_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working bst node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Call action function */
    action(root->data);

    /* Traverse in the left sub-tree */
    bst_traverse_preorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    bst_traverse_preorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in preorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the bst property)
 * 
 * @param tree current working binary search tree object
 * @param action a pointer to a function that will perform an action
 * on every bst node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_traverse_preorder(const bst_tree_t * const __restrict__ tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        bst_traverse_preorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for bst_traverse_postorder function.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param tree an allocated binary search tree object
 * @param root starting point of the binary search tree traversal
 * @param action a pointer function to perform an action on one bst node object
 */
static void bst_traverse_postorder_helper(const bst_tree_t * const __restrict__ tree, const bst_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working bst node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    bst_traverse_postorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    bst_traverse_postorder_helper(tree, root->right, action);

    /* Call action function */
    action(root->data);
}

/**
 * @brief Function that will traverse all nodes in postorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the bst property)
 * 
 * @param tree current working binary search tree object
 * @param action a pointer to a function that will perform an action
 * on every bst node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_traverse_postorder(const bst_tree_t * const __restrict__ tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        bst_traverse_postorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Function that will traverse all nodes by level
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the bst property)
 * 
 * @param tree current working binary search tree object
 * @param action a pointer to a function that will perform an action
 * on every bst node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bst_traverse_level(const bst_tree_t * const __restrict__ tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Create a queue for bfs tree traversal */
        queue_t * const level_queue = create_queue(NULL, sizeof(tree->root));

        /* Check if queue was created successfully */
        if (NULL != level_queue) {

            scl_error_t err = SCL_OK;

            /* Push pointer to root node into qeuue */
            err = queue_push(level_queue, &tree->root);

            if (SCL_OK != err) {
                return err;
            }

            /* Traverse all nodes */
            while (!is_queue_empty(level_queue)) {

                /* Get front node from queue */
                const bst_tree_node_t * const front_node = *(const bst_tree_node_t ** const)queue_front(level_queue);

                /* Remove front node from queue */
                err = queue_pop(level_queue);

                if (SCL_OK != err) {
                    return err;
                }

                /* Call action function on front node */
                action(front_node->data);

                /* Push on queue front left child if it exists */
                if (tree->nil != front_node->left) {
                    err = queue_push(level_queue, &front_node->left);

                    if (SCL_OK != err) {
                        return err;
                    }
                }
                
                /* Push on queue front right child if it exists */
                if (tree->nil != front_node->right) {
                    err = queue_push(level_queue, &front_node->right);

                    if (SCL_OK != err) {
                        return err;
                    } 
                }
            }

            /* Free queue object from heap */
            return free_queue(level_queue);
        }
    }

    return SCL_NULL_QUEUE;
}