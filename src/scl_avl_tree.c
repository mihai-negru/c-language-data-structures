/**
 * @file scl_avl_tree.c
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

#include "./include/scl_avl_tree.h"
#include "./include/scl_queue.h"

/**
 * @brief Max function directive for countable numbers
 * of the same type
 * 
 */
#define _MAX(A, B) (((A) >= (B))?(A):(B))

/**
 * @brief Create an avl object. Allocation may fail if there
 * is not enough memory on heap or cmp function is not valid
 * (data arranges in avl tree by comparation) in this case an exception will be thrown.
 * 
 * @param cmp pointer to a function to compare two sets of data
 * @param frd pointer to a function to free content of one data
 * @param data_size length in bytes of the data data type
 * @return avl_tree_t* a new allocated avl tree object or NULL (if function failed)
 */
avl_tree_t* create_avl(compare_func cmp, free_func frd, size_t data_size) {
    /* Check if compareData function is valid */
    if (NULL == cmp) {
        errno = EINVAL;
        perror("Compare function undefined for avl tree");
        return NULL;
    }

    /* Check if size fo the data is valid */
    if (0 == data_size) {
        errno = EINVAL;
        perror("Data size at creation is zero");
        return NULL;
    }

    /* Allocate a new avl tree object on heap */
    avl_tree_t *new_tree = malloc(sizeof(*new_tree));

    /* Check if avl tree object was allocated */
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
            new_tree->nil->height = 0;
            new_tree->nil->left = new_tree->nil->right = new_tree->nil;
            new_tree->nil->parent = new_tree->nil;
        } else {
            errno = ENOMEM;
            perror("Not enough memory for nil red-black allocation");
        }

        /* Set root and size of the avl tree */
        new_tree->root = new_tree->nil;
        new_tree->data_size = data_size;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for avl allocation");
    }
    
    /* Return a new allocated avl tree object or `NULL` */
    return new_tree;
}

/**
 * @brief Create an avl node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return `nil` and an exception
 * will be thrown.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data
 * @return avl_tree_node_t* a new allocated avl tree node object or `nil`
 */
static avl_tree_node_t* create_avl_node(const avl_tree_t * const __restrict__ tree, const void * __restrict__ data) {
    /* Check if data address is valid */
    if (NULL == data) {
        return tree->nil;
    }

    /* Allocate a new node on the heap */
    avl_tree_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if allocation went successfully */
    if (NULL != new_node) {

        /* Set default node data */
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;
        new_node->height = 1;

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
            perror("Not enough memory for node avl data allocation");
        }
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node avl allocation");
    }

    /* return a new avl tree node object or `NULL` */
    return new_node;
}

/**
 * @brief A helper function for free_avl function.
 * Function will iterate through all nodes recursively
 * by Left-Right-Root principle.
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to pointer of current avl node object
 */
static void free_avl_helper(const avl_tree_t * const __restrict__ tree, avl_tree_node_t ** const __restrict__ root) {
    /* Check if current node is valid */
    if (tree->nil == *root) {
        return;
    }

    /* Recursive calls */
    free_avl_helper(tree, &(*root)->left);
    free_avl_helper(tree, &(*root)->right);

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

    /* Free avl node pointer */
    if (tree->nil != *root) {
        free(*root);

        *root = tree->nil;
    }
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * avl tree object. The function will iterate through all nodes
 * and will free the data content according to frd function provided
 * by user at creation of avl tree, however if no free function
 * was provided it means that data pointer does not contain any dynamically
 * allocated elements.
 * 
 * @param tree an allocated avl tree object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_avl(avl_tree_t * const __restrict__ tree) {
    /* Check if tree needs to be freed */
    if (NULL != tree) {

        /* Free every node from avl -> tree */
        free_avl_helper(tree, &tree->root);
        
        /* Free `nil` cell*/
        free(tree->nil);

        tree->nil = NULL;        

        /* Free avl tree object */
        free(tree);

        return SCL_OK;
    }

    return SCL_NULL_AVL;
}

/**
 * @brief Function to update the height of a node that is broken.
 * Function may fail if the selected node is `nil`.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node an avl tree node object to update its height
 */
static void avl_update_node_height(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * const __restrict__ fix_node) {
    /* Check if input data is valid */
    if (tree->nil != fix_node) {

        /* Update node height */
        fix_node->height = _MAX(fix_node->left->height, fix_node->right->height) + 1;
    }
}

/**
 * @brief Function to rotate to left a subtree starting 
 * from fix_node avl tree node object. Function may fail
 * if avl tree object is not allocated or avl tree node
 * object is `nil`.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node pointer to avl tree node object to rotate
 */
static void avl_rotate_left(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->right) {
        return;
    }

    /* Set new rotated sub-root */
    avl_tree_node_t * const rotate_node = fix_node->right;

    /* Update child of fix_node */
    fix_node->right = rotate_node->left;

    /* Update child parent to fix_node */
    if (tree->nil != rotate_node->left) {
        rotate_node->left->parent = fix_node;
    }

    /* Rotation to left */
    rotate_node->left = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (tree->nil != rotate_node->parent) {
        if (tree->cmp(rotate_node->data, rotate_node->parent->data) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        tree->root = rotate_node;
    }

    /* Update the height of rotated avl tree node objects */
    avl_update_node_height(tree, fix_node);
    avl_update_node_height(tree, rotate_node);
}

/**
 * @brief Function to rotate to right a subtree starting 
 * from fix_node avl tree node object. Function may fail
 * if avl tree object is not allocated or avl tree node
 * object is `nil`.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node pointer to avl tree node object to rotate
 */
static void avl_rotate_right(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->left) {
        return;
    }

    /* Set new rotated sub-root */
    avl_tree_node_t * const rotate_node = fix_node->left;

    /* Update child of fix_node */
    fix_node->left = rotate_node->right;

    /* Update child parent to fix_node */
    if (tree->nil != rotate_node->right) {
        rotate_node->right->parent = fix_node;
    }

    /* Rotation to right */
    rotate_node->right = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (tree->nil != rotate_node->parent) {
        if (tree->cmp(rotate_node->data, rotate_node->parent->data) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        tree->root = rotate_node;
    }

    /* Update the height of rotated avl tree node objects */
    avl_update_node_height(tree, fix_node);
    avl_update_node_height(tree, rotate_node);
}

/**
 * @brief Function to get the balance factor of an
 * avl tree node object from current working tree
 * 
 * @param fix_node pointer to avl tree node object
 * @return int32_t balance factor of the fix_node
 */
static int32_t avl_get_node_balance(const avl_tree_node_t * const __restrict__ fix_node) {
    /* Return balance factor of the node */
    return (fix_node->left->height - fix_node->right->height);
}

/**
 * @brief Helper function to fix up the balance of an avl_tree_t
 * after insertion of one node. Function may fail if current
 * working tree and node is `nil`.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 * @return scl_error_t enum object for handling errors 
 */
static scl_error_t avl_insert_fix_node_up(avl_tree_t * const __restrict__ tree, avl_tree_node_t *fix_node) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Fix avl tree */
    while (tree->nil != fix_node) {

        /* Update height of the current node */
        avl_update_node_height(tree, fix_node);

        /* Get balance factors of the current node */
        int32_t avl_node_balance_factor = avl_get_node_balance(fix_node);
        int32_t avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int32_t avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        /* Left-Left rotation case */
        if ((2 == avl_node_balance_factor) && (1 == avl_node_left_balance_factor)) {
            avl_rotate_right(tree, fix_node);
        }

        /* Right-Right rotation case */
        if ((-2 == avl_node_balance_factor) && (-1 == avl_node_right_balance_factor)) {
            avl_rotate_left(tree, fix_node);
        }

        /* Left-Right rotation case */
        if ((2 == avl_node_balance_factor) && (-1 == avl_node_left_balance_factor)) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        /* Right-Left rotation case */
        if ((-2 == avl_node_balance_factor) && (1 == avl_node_right_balance_factor)) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        /* Fix next node */
        fix_node = fix_node->parent;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to insert one generic data to an avl.
 * Function may fail if avl or data are not valid (have
 * address `NULL`) or not enough heap memory is left. You
 * CANNOT insert different data types into avl tree, this
 * will evolve into an uknown behavior or segmentation fault.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t avl_insert(avl_tree_t * const __restrict__ tree, const void * __restrict__ data) {
    /* Check if tree and data are valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Set iterator pointers */
    avl_tree_node_t *iterator = tree->root;
    avl_tree_node_t *parent_iterator = tree->nil;

    /* Find a valid position for insertion */
    while (tree->nil != iterator) {
        parent_iterator = iterator;

        if (tree->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else if (tree->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else {

            /*
             * Node already exists in current avl tree
             * increment count value of node
             */
            ++(iterator->count);
            return 0;
        }
    }

    /* Create a new avl node object */
    avl_tree_node_t *new_node = create_avl_node(tree, data);

    /* Check if new avl node was created */
    if (tree->nil == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
        
    scl_error_t err = SCL_OK;

    if (tree->nil != parent_iterator) {

        /* Update parent links */
        new_node->parent = parent_iterator;

        /* Update children links */
        if (tree->cmp(parent_iterator->data, new_node->data) >= 1) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        /* Fix tree is needed */
        err = avl_insert_fix_node_up(tree, parent_iterator);
    } else {

        /* Created node is root node */
        tree->root = new_node;
    }

    /* Increase avl tree size */
    ++(tree->size);

    /* Insertion in avl went successfully */
    return err;
}

/**
 * @brief Function to search data in avl tree O(log N).
 * Function will start searching from avl tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return avl_tree_node_t* avl tree node object containing
 * data value or `nil` in case no such node exists
 */
static avl_tree_node_t* avl_find_node(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    /* Set iterator pointer */
    avl_tree_node_t *iterator = tree->root;

    /* Search for input data (void *data) in all tree */
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
 * @brief Function to search data in avl tree O(log N).
 * Function will start searching from avl tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return const void* pointer to location of data type found in 
 * the tree or `NULL` if data was not found
 */
const void* avl_find_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get data from found node or `NULL` if node is `nil` */
    return avl_find_node(tree, data)->data;
}

/**
 * @brief Function to swap two nodes from an avl tree object.
 * This function MUST NOT be used by users, because it will
 * break the proprety of avl tree, it is used by program
 * to delete nodes from avl tree and must be used just
 * in delete function provided by the program.
 * 
 * @param tree an allocated avl tree object
 * @param dest_node avl node object to rewrite data bytes from src_node
 * @param src_node avl node object to copy data bytes
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t avl_swap_nodes(avl_tree_t * const __restrict__ tree, avl_tree_node_t * const __restrict__ dest_node, avl_tree_node_t * __restrict__ const src_node) {
    /* Check if swap is posible */
    if ((tree->nil == dest_node) || (tree->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    /* Interchange the right child */

    avl_tree_node_t *temp = dest_node->right;

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
 * a node in avl tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param tree an allocated avl tree object
 * @param base_node avl node object to calculate its level
 * @return int32_t level of input avl object node
 */
static int32_t avl_node_level(const avl_tree_t * const __restrict__ tree, const avl_tree_node_t * __restrict__ base_node) {
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
 * a data node in avl tree. Function may fail if input data
 * is not in the current working tree or it's address is `NULL`.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to a value type to find level of node
 * containing current data
 * @return int32_t level of input avl object data node
 */
int32_t avl_data_level(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return -1;
    }

    /* Return the level of the data node according to data */
    return avl_node_level(tree, avl_find_node(tree, data));
}

/**
 * @brief Function to check if an avl
 * tree object is empty or not.
 * 
 * @param tree an allocated avl tree
 * @return uint8_t 1 if avl tree is empty or not allocated
 * 0 if it is not empty
 */
uint8_t is_avl_empty(const avl_tree_t * const __restrict__ tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Function to get root data node of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return const void* the root node data of the current avl tree
 */
const void* get_avl_root(const avl_tree_t * const __restrict__ tree) {
    if (NULL == tree) {
        return NULL;
    }

    return tree->root->data;
}

/**
 * @brief Function to get size of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return size_t size of the current avl tree
 */
size_t get_avl_size(const avl_tree_t * const __restrict__ tree) {
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
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @return avl_tree_node_t* pointer to maximum node value from avl
 */
static avl_tree_node_t* avl_max_node(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ root) {
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
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @return avl_tree_node_t* pointer to minimum node value from avl
 */
static avl_tree_node_t* avl_min_node(const avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

/**
 * @brief Function to get the maximum data value from avl.
 * Function will search the maximum data considering subroot data
 * node as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated avl tree object
 * @param subroot_data pointer to a data value that represents a node
 * to start searcing for maximum data node
 * @return const void* pointer to maximum data value from avl tree or `NULL`
 */
const void* avl_max_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get maximum data from avl tree or `NULL` is node is `nil` */
    return avl_max_node(tree, avl_find_node(tree, subroot_data))->data;
}

/**
 * @brief Function to get the minimum data value from avl.
 * Function will search the minimum data considering subroot data
 * node as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated avl tree object
 * @param subroot_data pointer to a data value that represents a node
 * to start searcing for minimum node
 * @return const void* pointer to minimum data value from avl tree
 */
const void* avl_min_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get minimum data from avl tree or `NULL` is node is `nil` */
    return avl_min_node(tree, avl_find_node(tree, subroot_data))->data;
}

/**
 * @brief Helper function to fix up the balance of a avl_tree_t
 * after deletion of one node. Function may fail if current
 * working tree and node are `NULL`.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t avl_delete_fix_node_up(avl_tree_t * const __restrict__ tree, avl_tree_node_t * __restrict__ fix_node) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Fix avl tree */
    while (tree->nil != fix_node) {

        /* Update height of the current node */
        avl_update_node_height(tree, fix_node);

        /* Get balance factors of the current node */
        int32_t avl_node_balance_factor = avl_get_node_balance(fix_node);
        int32_t avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int32_t avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        /* Left-Left rotation case */
        if ((avl_node_balance_factor > 1) && (avl_node_left_balance_factor >= 0)) {
            avl_rotate_right(tree, fix_node);
        }

        /* Right-Right rotation case */
        if ((avl_node_balance_factor < -1) && (avl_node_right_balance_factor <= 0)) {
            avl_rotate_left(tree, fix_node);
        }

        /* Left-Right rotation case */
        if ((avl_node_balance_factor > 1) && (avl_node_left_balance_factor < 0)) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        /* Right-Left rotation case */
        if ((avl_node_balance_factor < -1) && (avl_node_right_balance_factor > 0)) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        /* Fix next node */
        fix_node = fix_node->parent;
    }

    return SCL_OK;
}

/**
 * @brief Function to delete one generic data from a avl.
 * Function may fail if input data is not valid or if
 * changing the data fails. You can delete one data at a time
 * and MUST specify a valid avl tree and a valid data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data to be deleted
 * @return scl_error_t enum object for handling errors
 */
scl_error_t avl_delete(avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_AVL;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Find current node (root) in avl tree */
    avl_tree_node_t *delete_node = avl_find_node(tree, data);

    /* Bst node was not found exit process */
    if (tree->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Delete selected node */
    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {

        /* Selected node has two children */

        /* Find a replacement for selected node */
        avl_tree_node_t *delete_successor = avl_min_node(tree, delete_node->right);
                
        /* Replace the selected avl node and remove the dublicate */
        scl_error_t err = avl_swap_nodes(tree, delete_node, delete_successor);

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
                * Update new root to NULL
                */
            tree->root = tree->nil;
        }
    }

    avl_tree_node_t *parent_delete_node = delete_node->parent;

    /* Free content of the data pointer */
    if ((NULL != tree->frd) && (NULL != delete_node->data)) {
        tree->frd(delete_node->data);
    }

    /* Free data pointer of selected node */
    if (NULL != delete_node->data) {
        free(delete_node->data);
    }

    /* Set data pointer as NULL */
    delete_node->data = NULL;

    /* Free selected avl node pointer */
    if (tree->nil != delete_node) {
        free(delete_node);
    }

    /* Set selected avl node as NULL */
    delete_node = tree->nil;

    /* Deacrease tree size  */
    --(tree->size);
    
    /* Fix avl tree if needed */
    return avl_delete_fix_node_up(tree, parent_delete_node);
}

/**
 * @brief Function to search the inorder predecessor for
 * a specified data type value. Function may fail if
 * avl tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the avl tree does not contain specified data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return avl_tree_node_t* `nil` or inorder predecessor of the
 * node containing (void *data) value.
 */
static avl_tree_node_t* avl_predecessor_node(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    avl_tree_node_t *iterator = avl_find_node(tree, data);

    /* If node is not in avl than return `nil` */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a left child than
     * find predecessor in left subtree
     */
    if (tree->nil != iterator->left) {
        return avl_max_node(tree, iterator->left);
    }

    /* Set parent iterator */
    avl_tree_node_t *parent_iterator = iterator->parent;

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
 * avl tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the avl tree does not contain specified data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return avl_tree_node_t* `nil` or inorder successor of the
 * node containing (void *data) value.
 */
static avl_tree_node_t* avl_successor_node(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    avl_tree_node_t *iterator = avl_find_node(tree, data);

    /* If node is not in avl than return `nil` */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a right child than
     * find successor in right subtree
     */
    if (tree->nil != iterator->right) {
        return avl_min_node(tree, iterator->right);
    }

    /* Set parent iterator */
    avl_tree_node_t *parent_iterator = iterator->parent;

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
 * avl tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the avl tree does not contain specified data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return const void* NULL or data of inorder predecessor of the
 * node containing (void *data) value.
 */
const void* avl_predecessor_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the predecessor data or `NULL` if node is `nil` */
    return avl_predecessor_node(tree, data)->data;
}

/**
 * @brief Function to search the inorder successor for
 * a specified data type value. Function may fail if
 * avl tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the avl tree does not contain specified data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return const void* NULL or data of inorder successor of the
 * node containing (void *data) value.
 */
const void* avl_successor_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the successor data or `NULL` if node is `nil` */
    return avl_successor_node(tree, data)->data;
}

/**
 * @brief Function to get the lowest common ancestor
 * node of the two specified nodes that contain
 * as data types value (data1 and data2). Function may
 * fail if tree and both data pointers are not allocated
 * and also function may fail if the nodes are not from the
 * current working tree.
 * 
 * @param tree an allocated avl tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return avl_tree_node_t* pointer to a avl node object that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
static avl_tree_node_t* avl_lowest_common_ancestor_node(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || NULL == data2) {
        return tree->nil;
    }

    /* Check if both nodes are in the current working avl tree */
    if ((tree->nil == avl_find_node(tree, data1)) || (tree->nil == avl_find_node(tree, data2))) {
        return tree->nil;
    }

    /* Set iterator pointer */
    avl_tree_node_t *iterator = tree->root;

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
 * @param tree an allocated avl tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return const void* pointer to a avl node object data that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
const void* avl_lowest_common_ancestor_data(const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return NULL;
    }

    /* Get the lowest common ancestor data or `NULL` if node is `nil` */
    return avl_lowest_common_ancestor_node(tree, data1, data2)->data;
}

/**
 * @brief Helper function for avl_traverse_inorder function.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param tree an allocated avl tree object
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_inorder_helper(const avl_tree_t * const __restrict__ tree, const avl_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working avl node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    avl_traverse_inorder_helper(tree, root->left, action);
    
    /* Call action function */
    action(root->data);

    /* Traverse in the right sub-tree */
    avl_traverse_inorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in inorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl property)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 * @return scl_error_t enum object for handling errors
 * 
 */
scl_error_t avl_traverse_inorder(const avl_tree_t * const __restrict__ tree, action_func action) {
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
    }
    else {

        /* Call helper function and traverse all nodes */
        avl_traverse_inorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for avl_traverse_preorder function.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param tree an allocated avl tree object
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_preorder_helper(const avl_tree_t * const __restrict__ tree, const avl_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working avl node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Call action function */
    action(root->data);

    /* Traverse in the left sub-tree */
    avl_traverse_preorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    avl_traverse_preorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in preorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl prroperty)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t avl_traverse_preorder(const avl_tree_t * const __restrict__ tree, action_func action) {
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
        avl_traverse_preorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for avl_traverse_postorder function.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param tree an allocated avl tree object
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_postorder_helper(const avl_tree_t * const __restrict__ tree, const avl_tree_node_t * const __restrict__ root, action_func action) {
    /* Check if current working avl node is not `nil` */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    avl_traverse_postorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    avl_traverse_postorder_helper(tree, root->right, action);

    /* Call action function */
    action(root->data);
}

/**
 * @brief Function that will traverse all nodes in postorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl property)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t avl_traverse_postorder(const avl_tree_t * const __restrict__ tree, action_func action) {
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
        avl_traverse_postorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Function that will traverse all nodes by level
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl property)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t avl_traverse_level(const avl_tree_t * const __restrict__ tree, action_func action) {
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
                const avl_tree_node_t * const front_node = *(const avl_tree_node_t ** const)queue_front(level_queue);

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
