/**
 * @file scl_red_black_tree.c
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

#include "./include/scl_red_black_tree.h"
#include "./include/scl_queue.h"

/**
 * @brief Create an red-black object. Allocation may fail if there
 * is not enough memory on heap or compareData function is not valid
 * (data arranges in red-black tree by comparation) in this case an exception will be thrown.
 * 
 * @param cmp pointer to a function to compare two sets of data
 * @param frd pointer to a function to free content of one data
 * @return rbk_tree_t* a new allocated red-black tree object or NULL (if function failed)
 */
rbk_tree_t* create_rbk(compare_func cmp, free_func frd) {
    /* Check if compareData function is valid */
    if (NULL == cmp) {
        errno = EINVAL;
        perror("Compare function undefined for red-black tree");
        return NULL;
    }

    /* Allocate a new red-black tree object on heap */
    rbk_tree_t *new_tree = malloc(sizeof(*new_tree));

    /* Check if red-black tree object was allocated */
    if (NULL != new_tree) {

        /* Set function pointers */
        new_tree->cmp = cmp;
        new_tree->frd = frd;

        /* Create nil node */
        new_tree->nil = malloc(sizeof(*new_tree->nil));

        /* Set default values for a nil cell*/
        if (NULL != new_tree->nil) {
            new_tree->nil->data = NULL;
            new_tree->nil->color = BLACK;
            new_tree->nil->count = 1;
            new_tree->nil->left = new_tree->nil->right = new_tree->nil;
            new_tree->nil->parent = new_tree->nil;
        } else {
            errno = ENOMEM;
            perror("Not enough memory for nil red-black allocation");
        }

        /* Set root and size of the red-black tree */
        new_tree->root = new_tree->nil;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for red-black allocation");
    }
    
    /* Return a new allocated red-black tree object or nil */
    return new_tree;
}

/**
 * @brief Create an red-black node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return NULL and an exception
 * will be thrown.
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data
 * @param data_size size of one generic data
 * @return rbk_tree_node_t* a new allocated red-black tree node object or nil
 */
static rbk_tree_node_t* create_rbk_node(const rbk_tree_t * const tree, const void * const data, size_t data_size) {
    /* Check if data address is valid */
    if (NULL == data) {
        return tree->nil;
    }

    /* Allocate a new node on the heap */
    rbk_tree_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if allocation went successfully */
    if (NULL != new_node) {

        /* Set default node data */
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;
        new_node->color = RED;

        /* Allocate heap memory for data */
        new_node->data = malloc(data_size);

        /* Check if memory allocation went right */
        if (NULL != new_node->data) {

            /*
             * Copy all bytes from data pointer
             * to memory allocated on heap
             */
            memcpy(new_node->data, data, data_size);
        } else {
            errno = ENOMEM;
            perror("Not enough memory for node red-black data allocation");
        }
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node red-black allocation");
    }

    /* return a new red-black tree node object or nil */
    return new_node;
}

/**
 * @brief A helper function for free_rbk function.
 * Function will iterate through all nodes recursively
 * by Left-Right-Root principle.
 * 
 * @param tree an allocated red-black tree object
 * @param root pointer to current red-black node object
 */
static void free_rbk_helper(const rbk_tree_t * const tree, rbk_tree_node_t ** const root) {
    /* Check if current node is valid */
    if (tree->nil == *root) {
        return;
    }

    /* Recursive calls */
    free_rbk_helper(tree, &(*root)->left);
    free_rbk_helper(tree, &(*root)->right);

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

    /* Free red-black node pointer */
    if (tree->nil != *root) {
        free(*root);

        *root = tree->nil;
    }
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * red-black tree object. The function will iterate through all nodes
 * and will free the data content according to freeData function provided
 * by user at creation of red-black tree, however if no free function
 * was provided it means that data pointer does not contain any dynamically
 * allocated elements.
 * 
 * @param tree an allocated red-black tree object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_rbk(rbk_tree_t * const tree) {
    /* Check if tree needs to be freed */
    if (NULL != tree) {

        /* Free every node from red-black -> tree */
        free_rbk_helper(tree, &tree->root);
        
        /* Free nil cell*/
        free(tree->nil);

        tree->nil = NULL;

        /* Free red-black tree object */
        free(tree);

        return SCL_OK;
    }

    return SCL_NULL_RBK;
}

/**
 * @brief Function to rotate to left a subtree starting 
 * from fix_node red-black tree node object. Function may fail
 * if red-black tree object is not allocated or red-black tree node
 * object is NULL.
 * 
 * @param tree an allocated red-black tree object
 * @param fix_node pointer to red-black tree node object to rotate
 */
static void rbk_rotate_left(rbk_tree_t * const tree, rbk_tree_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->right) {
        return;
    }

    /* Set new rotated sub-root */
    rbk_tree_node_t * const rotate_node = fix_node->right;

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
}

/**
 * @brief Function to rotate to right a subtree starting 
 * from fix_node red-black tree node object. Function may fail
 * if red-black tree object is not allocated or red-black tree node
 * object is NULL.
 * 
 * @param tree an allocated red-black tree object
 * @param fix_node pointer to red-black tree node object to rotate
 */
static void rbk_rotate_right(rbk_tree_t * const tree, rbk_tree_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->left) {
        return;
    }

    /* Set new rotated sub-root */
    rbk_tree_node_t * const rotate_node = fix_node->left;

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
}

/**
 * @brief Helper function to fix up the balance of a rbk_tree_t
 * after insertion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated red-black tree object
 * @param fix_node a pointer to a red-black tree node object to start
 * fixing the balance
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t rbk_insert_fix_node_up(rbk_tree_t * const tree, rbk_tree_node_t *fix_node) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set parent node pointer as default value */
    rbk_tree_node_t *parent_fix_node = tree->nil;

    /* Fix up the red black tree */
    while ((tree->root != fix_node) && (BLACK != fix_node->color) && (BLACK != fix_node->parent->color)) {

        /* Selected node is not root so check brother color */
        
        /* Set initial data */
        parent_fix_node = fix_node->parent;
        rbk_tree_node_t *brother_node = tree->nil;

        /* Find brother node */
        if (parent_fix_node->parent->left == parent_fix_node) {
            brother_node = parent_fix_node->parent->right;
        } else {
            brother_node = parent_fix_node->parent->left;
        }

        /* Fix tree according to brother's color */
        if (BLACK == brother_node->color) {

            /* Brother's color is black check what rotations we should make */
            
            if (parent_fix_node->left == fix_node) {
                if (parent_fix_node->parent->left == parent_fix_node) {

                    /* Left-Left rotation case*/

                    /* Recolouring nodes */
                    parent_fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    /* Rotation */
                    rbk_rotate_right(tree, parent_fix_node->parent);

                    /* Repoint selected node*/
                    fix_node = parent_fix_node;
                } else {

                    /* Right-Left Rotation */

                    /* Recolouring nodes */
                    fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;

                    /* Rotation */
                    rbk_rotate_right(tree, parent_fix_node);
                    rbk_rotate_left(tree, fix_node->parent);
                }
            } else {
                if (parent_fix_node->parent->left == parent_fix_node) {

                    /* Left-Right Rotation */

                    /* Recolouring nodes */
                    fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    /* Rotation */
                    rbk_rotate_left(tree, parent_fix_node);
                    rbk_rotate_right(tree, fix_node->parent);
                } else {

                    /* Right-Right Rotation */

                    /* Recolouring nodes */
                    parent_fix_node->color = BLACK;
                    parent_fix_node->parent->color = RED;
                    
                    /* Rotation */
                    rbk_rotate_left(tree, parent_fix_node->parent);

                    /* Repoint selected node */
                    fix_node = parent_fix_node;
                }
            }
        } else if (RED == brother_node->color) {

            /* Brother's color is red so recolor the nodes */
            parent_fix_node->parent->color = RED;
            brother_node->color = BLACK;
            parent_fix_node->color = BLACK;

            /* Repoint selected node */
            fix_node = parent_fix_node->parent;
        } else {

            /* Color is not RED or BLACK something went wrong */
            return SCL_UNKNOWN_RBK_COLOR;
        }
    }

    /* Make sure root is black */
    tree->root->color = BLACK;

    return SCL_OK;
}

/**
 * @brief Function to insert one generic data to a red-black.
 * Function may fail if red-black or data os not valid (have
 * address NULL) or not enough heap memory is left. You
 * CANNOT insert different data types into red-black tree, this
 * will evolve into an uknown behavior or segmentation fault.
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @param data_size size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * inserting in red-black went successfully
 */
int rbk_insert(rbk_tree_t * const tree, const void * const data, size_t data_size) {
    /* Check if tree and data are valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    /* Set iterator pointers */
    rbk_tree_node_t *iterator = tree->root;
    rbk_tree_node_t *parent_iterator = tree->nil;

    /* Find a valid position for insertion */
    while (tree->nil != iterator) {
        parent_iterator = iterator;

        if (tree->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else if (tree->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else {

            /*
             * Node already exists in current red-black tree
             * increment count value of node
             */
            ++(iterator->count);
            return 0;
        }
    }

    /* Create a new red-black node object */
    rbk_tree_node_t *new_node = create_rbk_node(tree, data, data_size);

    /* Check if new red-black node was created */
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

        /* Fix the red black tree*/
        err = rbk_insert_fix_node_up(tree, new_node);
    } else {

        /* Created node is root node */
        tree->root = new_node;
        new_node->color = BLACK;
    }

    /* Increase red-black tree size */
    ++(tree->size);

    /* Insertion in red-black went successfully */
    return err;
}

/**
 * @brief Function to search data in red-black tree O(log N).
 * Function will start searching from red-black tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @return rbk_tree_node_t* red-black tree node object containing
 * data value or NULL in case no such node exists
 */
static rbk_tree_node_t* rbk_find_node(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    /* Set iterator pointer */
    rbk_tree_node_t *iterator = tree->root;

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

const void* rbk_find_data(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the predecessor data or NULL if node is nil */
    return rbk_find_node(tree, data)->data;
}

/**
 * @brief Function to change data bytes of destination pointer.
 * This function MUST NOT be used by users, because it will
 * break the proprety of red-black tree, it is used by program
 * to delete nodes from red-black tree and must be used just
 * in delete function provided by the program.
 * 
 * @param dest_node red-black node object to rewrite data bytes from src_node
 * @param src_node red-black node object to copy data bytes
 * @param data_size size of a generic data type element
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t rbk_change_data(rbk_tree_node_t * const dest_node, const rbk_tree_node_t * const src_node, size_t data_size) {
    /* Check if data pointers are allocated */
    if ((NULL == dest_node->data) || (NULL == src_node->data)) {
        return SCL_CANNOT_CHANGE_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    /* Rewrite bytes into dest_node from src_node */
    memmove(dest_node->data, src_node->data, data_size);

    /* Update count parameter */
    dest_node->count = src_node->count;

    return SCL_OK;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in red-black tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param tree an allocated red-black tree object
 * @param base_node red-black node object to calculate its level
 * @return int32_t level of input red-black object node
 */
static int32_t rbk_node_level(const rbk_tree_t * const tree, const rbk_tree_node_t *base_node) {
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

int32_t rbk_data_level(const rbk_tree_t * const tree, const void * const data) {
    if ((NULL == tree) || (NULL == data)) {
        return -1;
    }

    return rbk_node_level(tree, rbk_find_node(tree, data));
}

/**
 * @brief Function to check whether an red-black
 * tree object is empty or not.
 * 
 * @param tree an allocated red-black tree
 * @return uint8_t 1 if red-black tree is empty or not allocated
 * 0 if it is not empty
 */
uint8_t is_rbk_empty(const rbk_tree_t * const tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Function to get root node of the red-black tree.
 * 
 * @param tree an allocated red-black tree object
 * @return rbk_tree_node_t* the root node of the current red-black tree
 */
const void* get_rbk_root(const rbk_tree_t * const tree) {
    if (NULL == tree) {
        return NULL;
    }

    return tree->root->data;
}

/**
 * @brief Function to get size of the red-black tree.
 * 
 * @param tree an allocated red-black tree object
 * @return size_t size of the current red-black tree
 */
size_t get_rbk_size(const rbk_tree_t * const tree) {
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
 * @param tree an allocated red-black tree object
 * @param root pointer to current working red-black node object
 * @return rbk_tree_node_t* pointer to maximum node value from red-black
 */
static rbk_tree_node_t* rbk_max_node(const rbk_tree_t * const tree, rbk_tree_node_t *root) {
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
 * @param tree an allocated red-black tree object
 * @param root pointer to current working red-black node object
 * @return rbk_tree_node_t* pointer to minimum node value from red-black
 */
static rbk_tree_node_t* rbk_min_node(const rbk_tree_t * const tree, rbk_tree_node_t *root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

/**
 * @brief Function to get the maximum data value from red-black.
 * Function will search the maximum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated red-black tree object
 * @param root pointer to current working red-black node object
 * @return void* pointer to maximum data value from red-black tree
 */
const void* rbk_max_data(const rbk_tree_t * const tree, const void * const subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get maximum data from red-black or NULL is node is nil*/
    return rbk_max_node(tree, rbk_find_node(tree, subroot_data))->data;
}

/**
 * @brief Function to get the minimum data value from red-black.
 * Function will search the minimum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated red-black tree object
 * @param root pointer to current working red-black node object
 * @return void* pointer to minimum data value from red-black tree
 */
const void* rbk_min_data(const rbk_tree_t * const tree, const void * const subroot_data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == subroot_data)) {
        return NULL;
    }

    /* Get minimum data from red-black or NULL is node is nil*/
    return rbk_min_node(tree, rbk_find_node(tree, subroot_data))->data;
}

/**
 * @brief Helper function to fix up the balance of a rbk_tree_t
 * after deletion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated red-black tree object
 * @param fix_node a pointer to a red-black tree node object that is a double black
 * @param parent_fix_node a pointer to a red-black tree node object, parent of double black node
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t rbk_delete_fix_node_up(rbk_tree_t * const tree, rbk_tree_node_t *fix_node, rbk_tree_node_t *parent_fix_node) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == parent_fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set the brother of the double black node */
    rbk_tree_node_t *brother_node = tree->nil;

    /* Fix the red-black tree */
    while ((tree->root != fix_node) && (BLACK == fix_node->color)) {
        if (parent_fix_node->left == fix_node) {

            /* Double black node is a left child */

            /* Find the brother node */
            brother_node = parent_fix_node->right;

            if (RED == brother_node->color) {

                /* Case 1: brother is a red node */

                /* Recolor the nodes */
                brother_node->color = BLACK;
                parent_fix_node->color = RED;

                /* Rotate the parent to the left */
                rbk_rotate_left(tree, parent_fix_node);

                /* Update the brother node */
                brother_node = parent_fix_node->right;
            }

            if ((BLACK == brother_node->left->color) && (BLACK == brother_node->right->color)) {

                /* Case 2: brother is a black node and its children are black */

                /* Recolor the brother node */
                brother_node->color = RED;

                /* Propagate the double black problem in higher hierarchy */
                fix_node = parent_fix_node;
            } else {
                if (BLACK == brother_node->right->color) {

                    /* Case 3: brother node is black, left child is red and right is black */

                    /* Recolor nodes */
                    brother_node->left->color = BLACK;
                    brother_node->color = RED;

                    /* Rotate brother to the right */
                    rbk_rotate_right(tree, brother_node);
                    
                    /* Update the new brother */
                    brother_node = parent_fix_node->right;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = BLACK;
                brother_node->right->color = BLACK;

                /* Rotate parent node to left */
                rbk_rotate_left(tree, parent_fix_node);

                /* Tree is fixed */
                fix_node = tree->root;
            }
        } else {

            /* Double black node is a right child */

            /* Find the brother node */
            brother_node = parent_fix_node->left;

            if (RED == brother_node->color) {

                /* Case 1: brother is a red node */

                /* Recolor the nodes */
                brother_node->color = BLACK;
                parent_fix_node->color = RED;

                /* Rotate the parent to the right */
                rbk_rotate_right(tree, parent_fix_node);

                /* Update the brother node */
                brother_node = parent_fix_node->left;
            }

            if ((BLACK == brother_node->right->color) && (BLACK == brother_node->left->color)) {

                /* Case 2: brother is a black node and its children are black */

                /* Recolor the brother node */
                brother_node->color = RED;

                /* Propagate the double black problem in higher hierarchy */
                fix_node = parent_fix_node;
            } else {
                if (BLACK == brother_node->left->color) {

                    /* Case 3: brother node is black, right child is red and left is black */

                    /* Recolor nodes */
                    brother_node->right->color = BLACK;
                    brother_node->color = RED;

                    /* Rotate brother to the left */
                    rbk_rotate_left(tree, brother_node);

                    /* Update the new brother */
                    brother_node = parent_fix_node->left;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = BLACK;
                brother_node->left->color = BLACK;

                /* Rotate parent node to right */
                rbk_rotate_right(tree, parent_fix_node);

                /* Tree is fixed */
                fix_node = tree->root;
            }
        }
    }

    /* Recolor the root as BLACK */
    fix_node->color = BLACK;

    return SCL_OK;
}

/**
 * @brief Function to delete one generic data from a red-black.
 * Function may fail if input data is not valid or if
 * changing the data fails. You can delete one data at a time
 * and MUST specify a valid red-black tree and a valid data pointer
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data to be deleted
 * @param data_size size of one generic data
 * @return scl_error_t enum object for handling errors
 */
scl_error_t rbk_delete(rbk_tree_t * const tree, const void * const data, size_t data_size) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (tree->nil == tree->root) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    /* Find node to delete */
    rbk_tree_node_t *delete_node = rbk_find_node(tree, data);

    /* Delete node is not in the current working tree */
    if (tree->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Node has two children swap with it's inorder successor and delete successor */
    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {

        /* Find a replacement for selected node */
        rbk_tree_node_t *delete_succecessor = rbk_min_node(tree, delete_node->right);
                
        /* Replace the selected red-black node and remove the dublicate */
        scl_error_t err = rbk_change_data(delete_node, delete_succecessor, data_size);

        if (SCL_OK != err) {
            return err;
        }

        /* Delete node now is inorder successor */
        delete_node = delete_succecessor;
    }

    /* Variable to check if fixing is needed */
    uint8_t need_fixing_tree = 1;

    /* Set the child of the deleted node */
    rbk_tree_node_t *delete_node_child = tree->nil;

    /* delete node has only one child */
    if (tree->nil != delete_node->left) {
        delete_node_child = delete_node->left;

        /* Check if deletion will not make a double black exception */
        if ((RED == delete_node_child->color) && (BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */            
            tree->root = delete_node_child;
        }
    } else if (tree->nil != delete_node->right) {
        delete_node_child = delete_node->right;

        /* Check if deletion will not make a double black exception */
        if ((RED == delete_node_child->color) && (BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            tree->root = delete_node_child;
        }
    } else {

        /* Check if deletion will not make a double black exception */ 
        if (RED == delete_node->color) {
            need_fixing_tree = 0;
        }

        // Update parent's links to nil node
        if (tree->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = tree->nil;
            } else {
                delete_node->parent->left = tree->nil;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            tree->root = tree->nil;
        }
    }

    rbk_tree_node_t *parent_delete_node = delete_node->parent;

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

    /* Free selected red-black node pointer */
    if (tree->nil != delete_node) {
        free(delete_node);
    }

    /* Set selected red-black node as NULL */
    delete_node = tree->nil;

    /* Deacrease tree size  */
    --(tree->size);

    /* Check if fixing is needed */
    if (0 != need_fixing_tree) {
        return rbk_delete_fix_node_up(tree, delete_node_child, parent_delete_node);
    }

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to search the inorder predecessor for
 * a specified data type value. Function may fail if
 * red-black tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the red-black tree does not contain specified data pointer
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @return rbk_tree_node_t* NULL or inorder predecessor of the
 * node containing (void *data) value.
 */
static rbk_tree_node_t* rbk_predecessor_node(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    rbk_tree_node_t *iterator = rbk_find_node(tree, data);

    /* If node is not in red-black than return NULL */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a left child than
     * find predecessor in left subtree
     */
    if (tree->nil != iterator->left) {
        return rbk_max_node(tree, iterator->left);
    }

    /* Set parent iterator */
    rbk_tree_node_t *parent_iterator = iterator->parent;

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
 * red-black tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the red-black tree does not contain specified data pointer
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @return rbk_tree_node_t* NULL or inorder successor of the
 * node containing (void *data) value.
 */
static rbk_tree_node_t* rbk_successor_node(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    rbk_tree_node_t *iterator = rbk_find_node(tree, data);

    /* If node is not in red-black than return NULL */
    if (tree->nil == iterator) {
        return tree->nil;
    }

    /*
     * If node has a right child than
     * find successor in right subtree
     */
    if (tree->nil != iterator->right) {
        return rbk_min_node(tree, iterator->right);
    }

    /* Set parent iterator */
    rbk_tree_node_t *parent_iterator = iterator->parent;

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
 * red-black tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the red-black tree does not contain specified data pointer
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @return void* NULL or data of inorder predecessor of the
 * node containing (void *data) value.
 */
const void* rbk_predecessor_data(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the predecessor data or NULL if node is nil */
    return rbk_predecessor_node(tree, data)->data;
}

/**
 * @brief Function to search the inorder successor for
 * a specified data type value. Function may fail if
 * red-black tree is not allocated, if it is empty or if data
 * type pointer is not valid. Also function may fail if
 * the red-black tree does not contain specified data pointer
 * 
 * @param tree an allocated red-black tree object
 * @param data pointer to an address of a generic data type
 * @return void* NULL or data of inorder successor of the
 * node containing (void *data) value.
 */
const void* rbk_succecessor_data(const rbk_tree_t * const tree, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the successor data or NULL if node is nil */
    return rbk_successor_node(tree, data)->data;
}

/**
 * @brief Function to get the lowest common ancestor
 * node of the two specified nodes that contain
 * as data types value (data1 and data2). Function may
 * fail if tree and both data pointers are not allocated
 * and also function may fail if the nodes are not from the
 * current working tree.
 * 
 * @param tree an allocated red-black tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return rbk_tree_node_t* pointer to a red-black node object that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
static rbk_tree_node_t* rbk_lowest_common_ancestor_node(const rbk_tree_t * const tree, const void * const data1, const void * const data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || NULL == data2) {
        return tree->nil;
    }

    /* Check if both nodes are in the current working red-black tree */
    if ((tree->nil == rbk_find_node(tree, data1)) || (tree->nil == rbk_find_node(tree, data2))) {
        return tree->nil;
    }

    /* Set iterator pointer */
    rbk_tree_node_t *iterator = tree->root;

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
 * @param tree an allocated red-black tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return void* pointer to a red-black node object data that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
const void* rbk_lowest_common_ancestor_data(const rbk_tree_t * const tree, const void * const data1, const void * const data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return NULL;
    }

    /* Get the lowest common ancestor data or NULL if node is nil */
    return rbk_lowest_common_ancestor_node(tree, data1, data2)->data;
}

/**
 * @brief Helper function for rbk_traverse_inorder function.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param tree an allocated red-black tree object
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_inorder_helper(const rbk_tree_t * const tree, rbk_tree_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    rbk_traverse_inorder_helper(tree, root->left, action);
    
    /* Call action function */
    action(root->data);

    /* Traverse in the right sub-tree */
    rbk_traverse_inorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in inorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the red-black prorpety)
 * 
 * @param tree current working red-black tree object
 * @param action a pointer to a function that will perform an action
 * on every red-black node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t rbk_traverse_inorder(const rbk_tree_t * const tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
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
        rbk_traverse_inorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for rbk_traverse_preorder function.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param tree an allocated red-black tree object
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_preorder_helper(const rbk_tree_t * const tree, rbk_tree_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Call action function */
    action(root->data);

    /* Traverse in the left sub-tree */
    rbk_traverse_preorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    rbk_traverse_preorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in preorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the red-black prorpety)
 * 
 * @param tree current working red-black tree object
 * @param action a pointer to a function that will perform an action
 * on every red-black node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t rbk_traverse_preorder(const rbk_tree_t * const tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        rbk_traverse_preorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Helper function for rbk_traverse_postorder function.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param tree an allocated red-black tree object
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_postorder_helper(const rbk_tree_t * const tree, rbk_tree_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    rbk_traverse_postorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    rbk_traverse_postorder_helper(tree, root->right, action);

    /* Call action function */
    action(root->data);
}

/**
 * @brief Function that will traverse all nodes in postorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the red-black prorpety)
 * 
 * @param tree current working red-black tree object
 * @param action a pointer to a function that will perform an action
 * on every red-black node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t rbk_traverse_postorder(const rbk_tree_t * const tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        rbk_traverse_postorder_helper(tree, tree->root, action);
    }

    return SCL_OK;
}

/**
 * @brief Function that will traverse all nodes by level
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the red-black prorpety)
 * 
 * @param tree current working red-black tree object
 * @param action a pointer to a function that will perform an action
 * on every red-black node object from current working tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t rbk_traverse_level(const rbk_tree_t * const tree, action_func action) {
    /* Check if input data is valid */
    if (NULL == tree) {
        return SCL_NULL_RBK;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Create a queue for bfs tree traversal */
        queue_t * const level_queue = create_queue(NULL);

        /* Check if queue was created successfully */
        if (NULL != level_queue) {

            scl_error_t err = SCL_OK;

            /* Push pointer to root node into qeuue */
            err = queue_push(level_queue, &tree->root, sizeof(tree->root));

            if (SCL_OK != err) {
                return err;
            }

            /* Traverse all nodes */
            while (!is_queue_empty(level_queue)) {

                /* Get front node from queue */
                rbk_tree_node_t * const front_node = *(rbk_tree_node_t ** const)queue_front(level_queue);

                /* Remove front node from queue */
                err = queue_pop(level_queue);

                if (SCL_OK != err) {
                    return err;
                }

                /* Call action function on front node */
                action(front_node->data);

                /* Push on queue front left child if it exists */
                if (tree->nil != front_node->left) {
                    err = queue_push(level_queue, &front_node->left, sizeof(front_node->left));

                    if (SCL_OK != err) {
                        return err;
                    }
                }
                
                /* Push on queue front right child if it exists */
                if (tree->nil != front_node->right) {
                    err = queue_push(level_queue, &front_node->right, sizeof(front_node->right));

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