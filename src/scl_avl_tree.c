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
 * is not enough memory on heap or compareData function is not valid
 * (data arranges in avl tree by comparation) in this case an exception will be thrown.
 * 
 * @param cmp pointer to a function to compare two sets of data
 * @param frd pointer to a function to free content of one data
 * @return avl_tree_t* a new allocated avl tree object or NULL (if function failed)
 */
avl_tree_t* create_avl(compare_func cmp, free_func frd) {
    /* Check if compareData function is valid */
    if (NULL == cmp) {
        errno = EINVAL;
        perror("Compare function undefined for avl tree");
        return NULL;
    }

    /* Allocate a new avl tree object on heap */
    avl_tree_t* new_tree = malloc(sizeof(*new_tree));

    /* Check if avl tree object was allocated */
    if (NULL != new_tree) {

        /* Set function pointers */
        new_tree->cmp = cmp;
        new_tree->frd = frd;

        /* Create nil node */
        new_tree->nil = malloc(sizeof(*new_tree->nil));

        /* Set default values for a nil cell*/
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
        new_tree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for avl allocation");
    }
    
    /* Return a new allocated avl tree object or NULL */
    return new_tree;
}

/**
 * @brief Create an avl node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return NULL and an exception
 * will be thrown.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data
 * @param data_size size of one generic data
 * @return avl_tree_node_t* a new allocated avl tree node object or NULL
 */
static avl_tree_node_t* create_avl_node(avl_tree_t* tree, const void* data, size_t data_size) {
    /* Check if data address is valid */
    if (NULL == data) {
        return tree->nil;
    }

    /* Allocate a new node on the heap */
    avl_tree_node_t* new_node = malloc(sizeof(*new_node));

    /* Check if allocation went successfully */
    if (NULL != new_node) {

        /* Set default node data */
        new_node->right = new_node->left = tree->nil;
        new_node->parent = tree->nil;
        new_node->count = 1;
        new_node->height = 1;

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
            perror("Not enough memory for node avl data allocation");
        }
    } else {
        new_node = tree->nil;
        errno = ENOMEM;
        perror("Not enough memory for node avl allocation");
    }

    /* return a new avl tree node object or NULL */
    return new_node;
}

/**
 * @brief A helper function for free_avl function.
 * Function will iterate through all nodes recursively
 * by Left-Right-Root principle.
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current avl node object
 */
static void free_avl_helper(avl_tree_t* tree, avl_tree_node_t* root) {
    /* Check if current node is valid */
    if (tree->nil == root) {
        return;
    }

    /* Recursive calls */
    free_avl_helper(tree, root->left);
    free_avl_helper(tree, root->right);

    /* Free content of the data pointer */
    if ((NULL != tree->frd) && (NULL != root->data)) {
        tree->frd(root->data);
    }

    /* Free data pointer */
    if (NULL != root->data) {
        free(root->data);
    }

    /* Set data pointer as NULL */
    root->data = NULL;

    /* Free avl node pointer */
    if (tree->nil != root) {
        free(root);
    }

    /* Set avl node pointer as NULL */
    root = tree->nil;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * avl tree object. The function will iterate through all nodes
 * and will free the data content according to freeData function provided
 * by user at creation of avl tree, however if no free function
 * was provided it means that data pointer does not contain any dynamically
 * allocated elements.
 * 
 * @param tree an allocated avl tree object
 */
void free_avl(avl_tree_t* tree) {
    /* Check if tree needs to be freed */
    if (NULL != tree) {

        /* Free every node from avl -> tree */
        free_avl_helper(tree, tree->root);
        
        /* Free nil cell*/
        free(tree->nil);

        tree->nil = NULL;        

        /* Free avl tree object */
        free(tree);

        /* Set tree pointer as NULL */
        tree = NULL;
    }
}

/**
 * @brief Function to update the height of a node that is broken.
 * Function may fail if the selected node is NULL
 * 
 * @param tree an allocated avl tree object
 * @param fix_node an avl tree node object to update its height
 */
static void avl_update_node_height(avl_tree_t* tree, avl_tree_node_t* fix_node) {
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
 * object is NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node pointer to avl tree node object to rotate
 */
static void avl_rotate_left(avl_tree_t* tree, avl_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->right) {
        return;
    }

    /* Set new rotated sub-root */
    avl_tree_node_t* rotate_node = fix_node->right;

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
 * object is NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node pointer to avl tree node object to rotate
 */
static void avl_rotate_right(avl_tree_t* tree, avl_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (tree->nil == fix_node->left) {
        return;
    }

    /* Set new rotated sub-root */
    avl_tree_node_t* rotate_node = fix_node->left;

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
 * @return int balance factor of the fix_node avl_tree_node_t
 */
static int avl_get_node_balance(avl_tree_node_t* fix_node) {
    /* Return balance factor of the node */
    return (fix_node->left->height - fix_node->right->height);
}

/**
 * @brief Helper function to fix up the balance of a avl_tree_t
 * after insertion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 */
static void avl_insert_fix_node_up(avl_tree_t* tree, avl_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Fix avl tree */
    while (tree->nil != fix_node) {

        /* Update height of the current node */
        avl_update_node_height(tree, fix_node);

        /* Get balance factors of the current node */
        int avl_node_balance_factor = avl_get_node_balance(fix_node);
        int avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

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
}

/**
 * @brief Function to insert one generic data to a avl.
 * Function may fail if avl or data os not valid (have
 * address NULL) or not enough heap memory is left. You
 * CANNOT insert different data types into avl tree, this
 * will evolve into an uknown behavior or segmentation fault.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @param data_size size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * inserting in avl went successfully
 */
int avl_insert(avl_tree_t* tree, const void* data, size_t data_size) {
    /* Check if tree and data are valid */
    if ((NULL == tree) || (NULL == data) || (0 == data_size)) {
        return 1;
    }

    /* Set iterator pointers */
    avl_tree_node_t* iterator = tree->root;
    avl_tree_node_t* parent_iterator = tree->nil;

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
    avl_tree_node_t* new_node = create_avl_node(tree, data, data_size);

    /* Check if new avl node was created */
    if (tree->nil == new_node) {
        return 1;
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

        avl_insert_fix_node_up(tree, parent_iterator);
    } else {

        /* Created node is root node */
        tree->root = new_node;
    }

    /* Increase avl tree size */
    ++(tree->size);

    /* Insertion in avl went successfully */
    return 0;
}

/**
 * @brief Function to search data in avl tree O(log N).
 * Function will start searching from avl tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return avl_tree_node_t* avl tree node object containing
 * data value or NULL in case no such node exists
 */
avl_tree_node_t* avl_find_data(avl_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root)) {
        return tree->nil;
    }

    /* Set iterator pointer */
    avl_tree_node_t* iterator = tree->root;

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
 * @brief Function to change data bytes of destination pointer.
 * This function MUST NOT be used by users, because it will
 * break the proprety of avl tree, it is used by program
 * to delete nodes from avl tree and must be used just
 * in delete function provided by the program.
 * 
 * @param dest_node avl node object to rewrite data bytes from src_node
 * @param src_node avl node object to copy data bytes
 * @param data_size size of a generic data type element
 */
static void avl_change_data(avl_tree_node_t* dest_node, const avl_tree_node_t* src_node, size_t data_size) {
    /* Check if data pointers are allocated */
    if ((NULL == dest_node->data) || (NULL == src_node->data) || (0 == data_size)) {
        return;
    }

    /* Rewrite bytes into dest_node from src_node */
    memmove(dest_node->data, src_node->data, data_size);

    /* Update count parameter */
    dest_node->count = src_node->count;
    dest_node->height = src_node->height;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in avl tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param tree an allocated avl tree object
 * @param base_node avl node object to calculate its level
 * @return int level of input avl object node
 */
int avl_node_level(avl_tree_t* tree, const avl_tree_node_t* base_node) {
    /* Check if input data is valid */
    if (tree->nil == base_node) {
        return -1;
    }

    /* Set level of node as -1 */
    int level_count = -1;

    /* Compute level of input node */
    while (tree->nil != base_node) {
        base_node = base_node->parent;
        ++level_count;
    }

    /* Return node level */
    return level_count;
}

/**
 * @brief Function to check whether an avl
 * tree object is empty or not.
 * 
 * @param tree an allocated avl tree
 * @return int 1 if avl tree is empty or not allocated
 * 0 if it is not empty
 */
int is_avl_empty(avl_tree_t* tree) {
    if ((NULL == tree) || (tree->nil == tree->root) || (0 == tree->size)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Function to get root node of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return avl_tree_node_t* the root node of the current avl tree
 */
avl_tree_node_t* get_avl_root(avl_tree_t* tree) {
    if (NULL == tree) {
        return tree->nil;
    }

    return tree->root;
}

/**
 * @brief Function to get size of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return size_t size of the current avl tree
 */
size_t get_avl_size(avl_tree_t* tree) {
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
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @return avl_tree_node_t* pointer to maximum node value from avl
 */
avl_tree_node_t* avl_max_node(avl_tree_t* tree, avl_tree_node_t* root) {
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
avl_tree_node_t* avl_min_node(avl_tree_t* tree, avl_tree_node_t* root) {
    if (tree->nil != root) {
        while (tree->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

/**
 * @brief Function to get the maximum data value from avl.
 * Function will search the maximum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @return void* pointer to maximum data value from avl tree
 */
void* avl_max_data(avl_tree_t* tree, avl_tree_node_t* root) {
    /* Check if input data is valid */
    if (tree->nil == root) {
        return NULL;
    }

    /* Return maximum data or NULL if node is nil */
    return avl_max_node(tree, root)->data;
}

/**
 * @brief Function to get the minimum data value from avl.
 * Function will search the minimum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @return void* pointer to minimum data value from avl tree
 */
void* avl_min_data(avl_tree_t* tree, avl_tree_node_t* root) {
    /* Check if input data is valid */
    if (tree->nil == root) {
        return NULL;
    }

    /* Return minimum data or NULL if node is nil */
    return avl_min_node(tree, root)->data;
}

/**
 * @brief Helper function to fix up the balance of a avl_tree_t
 * after deletion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 */
static void avl_delete_fix_node_up(avl_tree_t* tree, avl_tree_node_t* fix_node) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == fix_node)) {
        return;
    }

    /* Fix avl tree */
    while (tree->nil != fix_node) {

        /* Update height of the current node */
        avl_update_node_height(tree, fix_node);

        /* Get balance factors of the current node */
        int avl_node_balance_factor = avl_get_node_balance(fix_node);
        int avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

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
}

/**
 * @brief Function to delete one generic data from a avl.
 * Function may fail if input data is not valid or if
 * changing the data fails. You can delete one data at a time
 * and MUST specify a valid avl tree and a valid data pointer
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data to be deleted
 * @param data_size size of one generic data
 * @return int 1(Fail) if function failed or 0(Success) if
 * deletion from avl went successfully
 */
int avl_delete(avl_tree_t* tree, void* data, size_t data_size) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data) || (0 == data_size)) {
        return 1;
    }

    /* Find current node (root) in avl tree */
    avl_tree_node_t* delete_node = avl_find_data(tree, data);

    /* Bst node was not found exit process */
    if (tree->nil == delete_node) {
        return 1;
    }

    /* Delete selected node */
    if ((tree->nil != delete_node->left) && (tree->nil != delete_node->right)) {

        /* Selected node has two children */

        /* Find a replacement for selected node */
        avl_tree_node_t* delete_succecessor = avl_min_node(tree, delete_node->right);
                
        /* Replace the selected avl node and remove the dublicate */
        avl_change_data(delete_node, delete_succecessor, data_size);

        /* Set the new node to delete */
        delete_node = delete_succecessor;
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

    avl_tree_node_t* parent_delete_node = delete_node->parent;

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

    avl_delete_fix_node_up(tree, parent_delete_node);

    /* Deacrease tree size  */
    --(tree->size);

    /* Deletion went successfully */
    return 0;
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
 * @return avl_tree_node_t* NULL or inorder predecessor of the
 * node containing (void *data) value.
 */
avl_tree_node_t* avl_predecessor_node(avl_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    avl_tree_node_t* iterator = avl_find_data(tree, data);

    /* If node is not in avl than return NULL */
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
    avl_tree_node_t* parent_iterator = iterator->parent;

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
 * @return avl_tree_node_t* NULL or inorder successor of the
 * node containing (void *data) value.
 */
avl_tree_node_t* avl_successor_node(avl_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (tree->nil == tree->root) || (NULL == data)) {
        return tree->nil;
    }

    /* Find node containing the data value */
    avl_tree_node_t* iterator = avl_find_data(tree, data);

    /* If node is not in avl than return NULL */
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
    avl_tree_node_t* parent_iterator = iterator->parent;

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
 * @return void* NULL or data of inorder predecessor of the
 * node containing (void *data) value.
 */
void* avl_predecessor_data(avl_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return tree->nil;
    }

    /* Get the predecessor data or NULL if node is nil */
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
 * @return void* NULL or data of inorder successor of the
 * node containing (void *data) value.
 */
void* avl_succecessor_data(avl_tree_t* tree, const void* data) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data)) {
        return tree->nil;
    }

    /* Get the successor data or NULL if node is nil */
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
avl_tree_node_t* avl_lowest_common_ancestor_node(avl_tree_t* tree, const void* data1, const void* data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || NULL == data2) {
        return tree->nil;
    }

    /* Check if both nodes are in the current working avl tree */
    if ((tree->nil == avl_find_data(tree, data1)) || (tree->nil == avl_find_data(tree, data2))) {
        return tree->nil;
    }

    /* Set iterator pointer */
    avl_tree_node_t* iterator = tree->root;

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
 * @return void* pointer to a avl node object data that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
void* avl_lowest_common_ancestor_data(avl_tree_t* tree, const void* data1, const void* data2) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == data1) || (NULL == data2)) {
        return NULL;
    }

    /* Get the lowest common ancestor data or NULL if node is nil */
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
static void avl_traverse_inorder_helper(avl_tree_t* tree, avl_tree_node_t* root, avl_action action) {
    /* Check if current working avl node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    avl_traverse_inorder_helper(tree, root->left, action);
    
    /* Call action function */
    action(tree, root);

    /* Traverse in the right sub-tree */
    avl_traverse_inorder_helper(tree, root->right, action);
}

/**
 * @brief Function that will traverse all nodes in inorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl prorpety)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 */
void avl_traverse_inorder(avl_tree_t* tree, avl_action action) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    }
    else {

        /* Call helper function and traverse all nodes */
        avl_traverse_inorder_helper(tree, tree->root, action);
    }
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
static void avl_traverse_preorder_helper(avl_tree_t* tree, avl_tree_node_t* root, avl_action action) {
    /* Check if current working avl node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Call action function */
    action(tree, root);

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
 * the avl prorpety)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 */
void avl_traverse_preorder(avl_tree_t* tree, avl_action action) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        avl_traverse_preorder_helper(tree, tree->root, action);
    }
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
static void avl_traverse_postorder_helper(avl_tree_t* tree, avl_tree_node_t* root, avl_action action) {
    /* Check if current working avl node is not NULL */
    if (tree->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    avl_traverse_postorder_helper(tree, root->left, action);

    /* Traverse in the right sub-tree */
    avl_traverse_postorder_helper(tree, root->right, action);

    /* Call action function */
    action(tree, root);
}

/**
 * @brief Function that will traverse all nodes in postorder
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl prorpety)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 */
void avl_traverse_postorder(avl_tree_t* tree, avl_action action) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (tree->nil == tree->root) {

        /* Tree is empty no node to traverse */
        printf("(Null)\n");
    } else {

        /* Call helper function and traverse all nodes */
        avl_traverse_postorder_helper(tree, tree->root, action);
    }
}

/**
 * @brief Function that will traverse all nodes by level
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * the avl prorpety)
 * 
 * @param tree current working avl tree object
 * @param action a pointer to a function that will perform an action
 * on every avl node object from current working tree
 */
void avl_traverse_level(avl_tree_t* tree, avl_action action) {
    /* Check if input data is valid */
    if ((NULL == tree) || (NULL == action)) {
        return;
    }

    if (tree->nil == tree->root) {

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
                avl_tree_node_t* front_node = *(avl_tree_node_t **)queue_front(level_queue);

                /* Remove front node from queue */
                queue_pop(level_queue);

                /* Call action function on front node */
                action(tree, front_node);

                /* Push on queue front left child if it exists */
                if (tree->nil != front_node->left) {
                    queue_push(level_queue, &front_node->left, sizeof(front_node->left));
                }
                
                /* Push on queue front right child if it exists */
                if (tree->nil != front_node->right) {
                    queue_push(level_queue, &front_node->right, sizeof(front_node->right));
                }
            }

            /* Free queue object from heap */
            free_queue(level_queue);
        }
    }
}