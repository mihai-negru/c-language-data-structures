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
 * @param compare_data pointer to a function to compare two sets of data
 * @param free_data pointer to a function to free content of one data
 * @return rbk_tree_t* a new allocated red-black tree object or NULL (if function failed)
 */
rbk_tree_t* create_rbk(int (*compare_data)(const void*, const void*), void (*free_data)(void*)) {
    /* Check if compareData function is valid */
    if (NULL == compare_data) {
        errno = EINVAL;
        perror("Compare function undefined for red-black tree");
        return NULL;
    }

    /* Allocate a new red-black tree object on heap */
    rbk_tree_t* new_tree = malloc(sizeof(*new_tree));

    /* Check if red-black tree object was allocated */
    if (NULL != new_tree) {

        /* Set function pointers */
        new_tree->compare_data = compare_data;
        new_tree->free_data = free_data;

        /* Set root and size of the red-black tree */
        new_tree->root = NULL;
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for red-black allocation");
    }
    
    /* Return a new allocated red-black tree object or NULL */
    return new_tree;
}

/**
 * @brief Create an red-black node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return NULL and an exception
 * will be thrown.
 * 
 * @param data pointer to an address of a generic data
 * @param data_size size of one generic data
 * @return rbk_tree_node_t* a new allocated red-black tree node object or NULL
 */
static rbk_tree_node_t* create_rbk_node(const void* data, size_t data_size) {
    /* Check if data address is valid */
    if (NULL == data) {
        return NULL;
    }

    /* Allocate a new node on the heap */
    rbk_tree_node_t* new_node = malloc(sizeof(*new_node));

    /* Check if allocation went successfully */
    if (NULL != new_node) {

        /* Set default node data */
        new_node->right = new_node->left = NULL;
        new_node->parent = NULL;
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
        errno = ENOMEM;
        perror("Not enough memory for node red-black allocation");
    }

    /* return a new red-black tree node object or NULL */
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
static void free_rbk_helper(rbk_tree_t* tree, rbk_tree_node_t* root) {
    /* Check if current node is valid */
    if (NULL == root) {
        return;
    }

    /* Recursive calls */
    free_rbk_helper(tree, root->left);
    free_rbk_helper(tree, root->right);

    /* Free content of the data pointer */
    if ((NULL != tree->free_data) && (NULL != root->data)) {
        tree->free_data(root->data);
    }

    /* Free data pointer */
    if (NULL != root->data) {
        free(root->data);
    }

    /* Set data pointer as NULL */
    root->data = NULL;

    /* Free red-black node pointer */
    if (NULL != root) {
        free(root);
    }

    /* Set red-black node pointer as NULL */
    root = NULL;
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
 */
void free_rbk(rbk_tree_t* tree) {
    /* Check if tree needs to be freed */
    if (NULL != tree) {

        /* Free every node from red-black -> tree */
        free_rbk_helper(tree, tree->root);
        
        /* Free red-black tree object */
        free(tree);

        /* Set tree pointer as NULL */
        tree = NULL;
    }
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
static void rbk_rotate_left(rbk_tree_t* tree, rbk_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (NULL == fix_node->right) {
        return;
    }

    /* Set new rotated sub-root */
    rbk_tree_node_t* rotate_node = fix_node->right;

    /* Update child of fix_node */
    fix_node->right = rotate_node->left;

    /* Update child parent to fix_node */
    if (NULL != rotate_node->left) {
        rotate_node->left->parent = fix_node;
    }

    /* Rotation to left */
    rotate_node->left = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (NULL != rotate_node->parent) {
        if (tree->compare_data(rotate_node->data, rotate_node->parent->data) >= 1) {
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
static void rbk_rotate_right(rbk_tree_t* tree, rbk_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (NULL == fix_node->left) {
        return;
    }

    /* Set new rotated sub-root */
    rbk_tree_node_t* rotate_node = fix_node->left;

    /* Update child of fix_node */
    fix_node->left = rotate_node->right;

    /* Update child parent to fix_node */
    if (NULL != rotate_node->right) {
        rotate_node->right->parent = fix_node;
    }

    /* Rotation to right */
    rotate_node->right = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (NULL != rotate_node->parent) {
        if (tree->compare_data(rotate_node->data, rotate_node->parent->data) >= 1) {
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
 */
static void rbk_insert_fix_node_up(rbk_tree_t* tree, rbk_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == fix_node)) {
        return;
    }

    /* Set parent node pointer as default value */
    rbk_tree_node_t* parent_fix_node = NULL;

    /* Fix up the red black tree */
    while ((tree->root != fix_node) && (BLACK != fix_node->color) && (BLACK != fix_node->parent->color)) {

        /* Selected node is not root so check brother color */
        
        /* Set initial data */
        parent_fix_node = fix_node->parent;
        rbk_tree_node_t* brother_node = NULL;

        /* Find brother node */
        if (parent_fix_node->parent->left == parent_fix_node) {
            brother_node = parent_fix_node->parent->right;
        } else {
            brother_node = parent_fix_node->parent->left;
        }

        /* Fix tree according to brother's color */
        if ((NULL == brother_node) || (BLACK == brother_node->color)) {

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
            return;
        }
    }

    /* Make sure root is black */
    tree->root->color = BLACK;
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
int rbk_insert(rbk_tree_t* tree, const void* data, size_t data_size) {
    /* Check if tree and data are valid */
    if ((NULL == tree) || (NULL == data) || (0 == data_size)) {
        return 1;
    }

    /* Set iterator pointers */
    rbk_tree_node_t* iterator = tree->root;
    rbk_tree_node_t* parent_iterator = NULL;

    /* Find a valid position for insertion */
    while (NULL != iterator) {
        parent_iterator = iterator;

        if (tree->compare_data(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else if (tree->compare_data(iterator->data, data) <= -1) {
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
    rbk_tree_node_t* new_node = create_rbk_node(data, data_size);

    /* Check if new red-black node was created */
    if (NULL == new_node) {
        return 1;
    }
        
    if (NULL != parent_iterator) {

        /* Update parent links */
        new_node->parent = parent_iterator;

        /* Update children links */
        if (tree->compare_data(parent_iterator->data, new_node->data) >= 1) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        /* Fix the red black tree*/
        rbk_insert_fix_node_up(tree, new_node);
    } else {

        /* Created node is root node */
        tree->root = new_node;
        new_node->color = BLACK;
    }

    /* Increase red-black tree size */
    ++(tree->size);

    /* Insertion in red-black went successfully */
    return 0;
}

/**
 * @brief Function to search data in red-black tree O(log N).
 * Function will start searching from root node specified in 
 * parameters list of function.
 * 
 * @param tree an allocated red-black tree object
 * @param root pointer to current working red-black node object
 * @param data pointer to an address of a generic data type
 * @return rbk_tree_node_t* red-black tree node object containing
 * data value or NULL in case no such node exists
 */
static rbk_tree_node_t* rbk_find_data_set_root(rbk_tree_t* tree, rbk_tree_node_t* root, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == root)) {
        return NULL;
    }

    /* Set iterator pointer */
    rbk_tree_node_t* iterator = root;

    /*
     * Search for input data (void *data),
     * from root - subtree
     */
    while (NULL != iterator) {
        if (tree->compare_data(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else if (tree->compare_data(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    /* Data was not found */
    return NULL;
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
rbk_tree_node_t* rbk_find_data(rbk_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == tree->root)) {
        return NULL;
    }

    /* Set iterator pointer */
    rbk_tree_node_t* iterator = tree->root;

    /* Search for imput data (void *data) in all tree */
    while (NULL != iterator) {
        if (tree->compare_data(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else if (tree->compare_data(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    /* Data was not found */
    return NULL;
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
 */
static void rbk_change_data(rbk_tree_node_t* dest_node, const rbk_tree_node_t* src_node, size_t data_size) {
    /* Check if input data is valid */
    if ((NULL == dest_node) || (NULL == src_node) || (0 == data_size)) {
        return;
    }

    /* Check if data pointers are allocated */
    if ((NULL == dest_node->data) || (NULL == src_node->data)) {
        return;
    }

    /* Rewrite bytes into dest_node from src_node */
    memmove(dest_node->data, src_node->data, data_size);

    /* Update count parameter */
    dest_node->count = src_node->count;
    dest_node->color = src_node->color;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in red-black tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param base_node red-black node object to calculate its level
 * @return int level of input red-black object node
 */
int rbk_node_level(const rbk_tree_node_t* base_node) {
    /* Check if input data is valid */
    if (NULL == base_node) {
        return -1;
    }

    /* Set level of node as -1 */
    int level_count = -1;

    /* Compute level of input node */
    while (NULL != base_node) {
        base_node = base_node->parent;
        ++level_count;
    }

    /* Return node level */
    return level_count;
}

/**
 * @brief Function to check whether an red-black
 * tree object is empty or not.
 * 
 * @param tree an allocated red-black tree
 * @return int 1 if red-black tree is empty or not allocated
 * 0 if it is not empty
 */
int is_rbk_empty(rbk_tree_t* tree) {
    if ((NULL == tree) || (NULL == tree->root) || (0 == tree->size)) {
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
rbk_tree_node_t* get_rbk_root(rbk_tree_t* tree) {
    if (NULL == tree) {
        return NULL;
    }

    return tree->root;
}

/**
 * @brief Function to get size of the red-black tree.
 * 
 * @param tree an allocated red-black tree object
 * @return size_t size of the current red-black tree
 */
size_t get_rbk_size(rbk_tree_t* tree) {
    if (NULL == tree) {
        return __SIZE_MAX__;
    }

    return tree->size;
}

/**
 * @brief Function to get node with maximum data value.
 * Function will search the maximum considering root node
 * as the beginning of the tree (root != tree(root)).
 * 
 * @param root pointer to current working red-black node object
 * @return rbk_tree_node_t* pointer to maximum node value from red-black
 */
rbk_tree_node_t* rbk_max_node(rbk_tree_node_t* root) {
    if (NULL != root) {
        while (NULL != root->right) {
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
 * @param root pointer to current working red-black node object
 * @return rbk_tree_node_t* pointer to minimum node value from red-black
 */
rbk_tree_node_t* rbk_min_node(rbk_tree_node_t* root) {
    if (NULL != root) {
        while (NULL != root->left) {
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
 * @param root pointer to current working red-black node object
 * @return void* pointer to maximum data value from red-black tree
 */
void* rbk_max_data(rbk_tree_node_t* root) {
    /* Check if input data is valid */
    if (NULL == root) {
        return NULL;
    }

    /* Get maximum node from red-black */
    rbk_tree_node_t* max_node = rbk_max_node(root);

    /* Return data pointer if node is not NULL */
    if (NULL != max_node) {
        return max_node->data;
    }
    
    /* Function failed */
    return NULL;
}

/**
 * @brief Function to get the minimum data value from red-black.
 * Function will search the minimum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param root pointer to current working red-black node object
 * @return void* pointer to minimum data value from red-black tree
 */
void* rbk_min_data(rbk_tree_node_t* root) {
    /* Check if input data is valid */
    if (NULL == root) {
        return NULL;
    }

    /* Get minimum node from red-black */
    rbk_tree_node_t* min_node = rbk_min_node(root);

    /* Return data pointer if node is not NULL */
    if (NULL != min_node) {
        return min_node->data;
    }
    
    /* Function failed */
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Helper function to fix up the balance of a rbk_tree_t
 * after deletion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated red-black tree object
 * @param fix_node a pointer to a red-black tree node object to start
 * fixing the balance
 */
static void rbk_delete_fix_node_up(rbk_tree_t* tree, rbk_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == fix_node)) {
        return;
    }
}

/**
 * @brief Helper function for rbk_delete_data function.
 * Function will remove one data at a time and will preserve
 * the proprety of a red-black tree.
 * 
 * @param tree an allocated red-black tree object
 * @param root pointer to current working sub-tree
 * @param data pointer to an address of a generic data to be deleted
 * @param data_size size of one generic data
 */
static void rbk_delete_helper(rbk_tree_t* tree, rbk_tree_node_t* root, void* data, size_t data_size) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == root) || (NULL == data)) {
        return;
    }

    /* Find current node (root) in red-black tree */
    rbk_tree_node_t* delete_node = rbk_find_data_set_root(tree, root, data);

    /* Bst node was not found exit process */
    if (NULL == delete_node) {
        return;
    }

    /* Delete selected node */
    if ((NULL != delete_node->left) && (NULL != delete_node->right)) {

        /* Selected node has two children */

        /* Find a replacement for selected node */
        rbk_tree_node_t* delete_succecessor = rbk_min_node(delete_node->right);
                
        /* Replace the selected red-black node and remove the dublicate */
        rbk_change_data(delete_node, delete_succecessor, data_size);
        rbk_delete_helper(tree, delete_node->right, delete_succecessor->data, data_size);
    } else {

        /* Selected node has one or no chlid */

        if (NULL != delete_node->left) {

            /* Selected node has a left child */

            /* Update child-grandparent links */
            delete_node->left->parent = delete_node->parent;

            if (NULL != delete_node->parent) {

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
        } else if (NULL != delete_node->right) {

            /* Selected node has a right child */

            /* Update child-grandparent links */
            delete_node->right->parent = delete_node->parent;

            if (NULL != delete_node->parent) {

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
            if (NULL != delete_node->parent) {
                if (delete_node->parent->right == delete_node) {
                    delete_node->parent->right = NULL;
                } else {
                    delete_node->parent->left = NULL;
                }
            } else {

                /*
                 * Selected node was root
                 * Update new root to NULL
                 */
                tree->root = NULL;
            }
        }

        rbk_tree_node_t* parent_delete_node = delete_node->parent;

        /* Free content of the data pointer */
        if ((NULL != tree->free_data) && (NULL != delete_node->data)) {
            tree->free_data(delete_node->data);
        }

        /* Free data pointer of selected node */
        if (NULL != delete_node->data) {
            free(delete_node->data);
        }

        /* Set data pointer as NULL */
        delete_node->data = NULL;

        /* Free selected red-black node pointer */
        if (NULL != delete_node) {
            free(delete_node);
        }

        /* Set selected red-black node as NULL */
        delete_node = NULL;

        rbk_delete_fix_node_up(tree, parent_delete_node);

        /* Deacrease tree size  */
        --(tree->size);
    }
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
 * @return int 1(Fail) if function failed or 0(Success) if
 * deletion from red-black went successfully
 */
int rbk_delete(rbk_tree_t* tree, void* data, size_t data_size) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == tree->root) || (NULL == data) || (0 == data_size)) {
        return 1;
    }

    /* Call helper function for deletion */
    rbk_delete_helper(tree, tree->root, data, data_size);

    /* Deletion went successfully */
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
rbk_tree_node_t* rbk_predecessor_node(rbk_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == tree->root) || (NULL == data)) {
        return NULL;
    }

    /* Find node containing the data value */
    rbk_tree_node_t* iterator = rbk_find_data(tree, data);

    /* If node is not in red-black than return NULL */
    if (NULL == iterator) {
        return NULL;
    }

    /*
     * If node has a left child than
     * find predecessor in left subtree
     */
    if (NULL != iterator->left) {
        return rbk_max_node(iterator->left);
    }

    /* Set parent iterator */
    rbk_tree_node_t* parent_iterator = iterator->parent;

    /* Find predecessor node */
    while ((NULL != parent_iterator) && (parent_iterator->left == iterator)) {
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
rbk_tree_node_t* rbk_successor_node(rbk_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == tree->root) || (NULL == data)) {
        return NULL;
    }

    /* Find node containing the data value */
    rbk_tree_node_t* iterator = rbk_find_data(tree, data);

    /* If node is not in red-black than return NULL */
    if (NULL == iterator) {
        return NULL;
    }

    /*
     * If node has a right child than
     * find successor in right subtree
     */
    if (NULL != iterator->right) {
        return rbk_min_node(iterator->right);
    }

    /* Set parent iterator */
    rbk_tree_node_t* parent_iterator = iterator->parent;

    /* Find successor node */
    while ((NULL != parent_iterator) && (parent_iterator->right == iterator)) {
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
void* rbk_predecessor_data(rbk_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the predecessor node */
    rbk_tree_node_t* predecessor_node = rbk_predecessor_node(tree, data);

    /* Return data pointer if node is not NULL */
    if (NULL != predecessor_node) {
        return predecessor_node->data;
    }

    /* Function failed */
    return NULL;
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
void* rbk_succecessor_data(rbk_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return NULL;
    }

    /* Get the successor node */
    rbk_tree_node_t* successor_node = rbk_successor_node(tree, data);

    /* Return data pointer if nodse is not NULL */
    if (NULL != successor_node) {
        return successor_node->data;
    }

    /* Function failed */
    return NULL;
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
rbk_tree_node_t* rbk_lowest_common_ancestor_node(rbk_tree_t* tree, const void* data1, const void* data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || NULL == data2) {
        return NULL;
    }

    /* Check if both nodes are in the current working red-black tree */
    if ((NULL == rbk_find_data(tree, data1)) || (NULL == rbk_find_data(tree, data2))) {
        return NULL;
    }

    /* Set iterator pointer */
    rbk_tree_node_t* iterator = tree->root;

    /* Find the lowest common ancestor */
    while (NULL != iterator) {
        if ((tree->compare_data(iterator->data, data1) >= 1) && (tree->compare_data(iterator->data, data2) >= 1)) {
            iterator = iterator->left;
        } else if ((tree->compare_data(iterator->data, data1) <= -1) && (tree->compare_data(iterator->data, data2) <= -1)) {
            iterator = iterator->right;
        } else {

            /* Ancestor found */
            return iterator;
        }
    }

    /* Function failed */
    return NULL;
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
void* rbk_lowest_common_ancestor_data(rbk_tree_t* tree, const void* data1, const void* data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return NULL;
    }

    /* Get the lowest common ancestor node */
    rbk_tree_node_t* common_ancestor = rbk_lowest_common_ancestor_node(tree, data1, data2);

    /* Return data pointer if node is not NULL */
    if (NULL != common_ancestor) {
        return common_ancestor->data;
    }

    /* Function failed */
    return NULL;
}

/**
 * @brief Helper function for rbk_traverse_inorder function.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_inorder_helper(rbk_tree_node_t* root, void (*action)(const rbk_tree_node_t*)) {
    /* Check if current working red-black node is not NULL */
    if (NULL == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    rbk_traverse_inorder_helper(root->left, action);
    
    /* Call action function */
    action(root);

    /* Traverse in the right sub-tree */
    rbk_traverse_inorder_helper(root->right, action);
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
 */
void rbk_traverse_inorder(rbk_tree_t* tree, void (*action)(const rbk_tree_node_t*)) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (NULL == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    }
    else {

        /* Call helper function and traverse all nodes */
        rbk_traverse_inorder_helper(tree->root, action);
    }
}

/**
 * @brief Helper function for rbk_traverse_preorder function.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_preorder_helper(rbk_tree_node_t* root, void (*action)(const rbk_tree_node_t*)) {
    /* Check if current working red-black node is not NULL */
    if (NULL == root) {
        return;
    }

    /* Call action function */
    action(root);

    /* Traverse in the left sub-tree */
    rbk_traverse_preorder_helper(root->left, action);

    /* Traverse in the right sub-tree */
    rbk_traverse_preorder_helper(root->right, action);
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
 */
void rbk_traverse_preorder(rbk_tree_t* tree, void (*action)(const rbk_tree_node_t*)) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (NULL == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        rbk_traverse_preorder_helper(tree->root, action);
    }
}

/**
 * @brief Helper function for rbk_traverse_postorder function.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param root starting point of the red-black tree traversal
 * @param action a pointer function to perform an action on one red-black node object
 */
static void rbk_traverse_postorder_helper(rbk_tree_node_t* root, void (*action)(const rbk_tree_node_t*)) {
    /* Check if current working red-black node is not NULL */
    if (NULL == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    rbk_traverse_postorder_helper(root->left, action);

    /* Traverse in the right sub-tree */
    rbk_traverse_postorder_helper(root->right, action);

    /* Call action function */
    action(root);
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
 */
void rbk_traverse_postorder(rbk_tree_t* tree, void (*action)(const rbk_tree_node_t*)) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (NULL == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        rbk_traverse_postorder_helper(tree->root, action);
    }
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
 */
void rbk_traverse_level(rbk_tree_t* tree, void (*action)(const rbk_tree_node_t*)) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (NULL == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Create a queue for bfs tree traversal */
        queue_t* level_queue = create_queue(NULL);

        /* Check if queue was created successfully */
        if (NULL != level_queue) {

            /* Push pointer to root node into qeuue */
            queue_push(level_queue, &tree->root, sizeof(tree->root));

            /* Traverse all nodes */
            while (!is_queue_empty(level_queue)) {

                /* Get front node from queue */
                rbk_tree_node_t* front_node = *(rbk_tree_node_t **)queue_front(level_queue);

                /* Remove front node from queue */
                queue_pop(level_queue);

                /* Call action function on front node */
                action(front_node);

                /* Push on queue front left child if it exists */
                if ((NULL != front_node) && (NULL != front_node->left)) {
                    queue_push(level_queue, &front_node->left, sizeof(front_node->left));
                }
                
                /* Push on queue front right child if it exists */
                if ((NULL != front_node) && (NULL != front_node->right)) {
                    queue_push(level_queue, &front_node->right, sizeof(front_node->right));
                }
            }

            /* Free queue object from heap */
            free_queue(level_queue);
        }
    }
}