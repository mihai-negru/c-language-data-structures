/**
 * @file avlTreeUtils.c
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

#include "./include/avlTreeUtils.h"
#include "./include/queueUtils.h"

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
 * @param compareData pointer to a function to compare two sets of data
 * @param freeData pointer to a function to free content of one data
 * @return avlTree* a new allocated avl tree object or NULL (if function failed)
 */
avlTree* create_avl(int (*compareData)(const void *, const void *), void (*freeData)(void *)) {
    // Check if compareData function is valid
    if (compareData == NULL) {
        errno = EINVAL;
        perror("Compare function undefined for avl tree");
        return NULL;
    }

    // Allocate a new avl tree object on heap
    avlTree* newTree = (avlTree* )malloc(sizeof(avlTree));

    // Check if avl tree object was allocated
    if (newTree) {
        // Set function pointers
        newTree->compareData = compareData;
        newTree->freeData = freeData;

        // Set root and size of the avl tree
        newTree->root = NULL;
        newTree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for avl allocation");
    }
    
    // Return a new allocated avl tree object or NULL
    return newTree;
}

/**
 * @brief Create an avl node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough memory
 * is left on heap, in this case function will return NULL and an exception
 * will be thrown.
 * 
 * @param data pointer to an address of a generic data
 * @param dataSize size of one generic data
 * @return avlTreeNode* a new allocated avl tree node object or NULL
 */
static avlTreeNode* create_avl_node(const void *data, size_t dataSize) {
    // Check if data address is valid
    if (data == NULL)
        return NULL;

    // Allocate a new node on the heap
    avlTreeNode *newNode = (avlTreeNode *)malloc(sizeof(avlTreeNode));

    // Check if allocation went successfully
    if (newNode) {
        // Set default node data
        newNode->right = newNode->left = NULL;
        newNode->parent = NULL;
        newNode->count = 1;
        newNode->height = 1;

        // Allocate heap memory for data
        newNode->data = malloc(dataSize);

        // Check if memory allocation went right
        if (newNode->data)
            // Copy all bytes from data pointer
            // to memory allocated on heap
            memcpy(newNode->data, data, dataSize);
        else {
            errno = ENOMEM;
            perror("Not enough memory for node avl data allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node avl allocation");
    }

    // return a new avl tree node object or NULL
    return newNode;
}

/**
 * @brief A helper function for free_avl function.
 * Function will iterate through all nodes recursively
 * by Left-Right-Root principle.
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current avl node object
 */
static void free_avl_helper(avlTree *tree, avlTreeNode *root) {
    // Check if current node is valid
    if (root == NULL)
        return;

    // Recursive calls
    free_avl_helper(tree, root->left);
    free_avl_helper(tree, root->right);

    // Free content of the data pointer
    if (tree->freeData != NULL && root->data != NULL)
        tree->freeData(root->data);

    // Free data pointer
    if (root->data != NULL)
        free(root->data);

    // Set data pointer as NULL
    root->data = NULL;

    // Free avl node pointer
    if (root != NULL)
        free(root);

    // Set avl node pointer as NULL
    root = NULL;
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
void free_avl(avlTree *tree) {
    // Check if tree needs to be freed
    if (tree) {
        // Free every node from avl -> tree
        free_avl_helper(tree, tree->root);
        
        // Free avl tree object
        free(tree);

        // Set tree pointer as NULL
        tree = NULL;
    }
}

/**
 * @brief Function to check whether an avl
 * tree object is empty or not.
 * 
 * @param tree an allocated avl tree
 * @return int 1 if avl tree is empty or not allocated
 * 0 if it is not empty
 */
int is_avl_empty(avlTree *tree) {
    if (tree == NULL || tree->root == NULL || tree->size == 0)
        return 1;

    return 0;
}

/**
 * @brief Function to update the height of a node that is broken.
 * Function may fail if the selected node is NULL
 * 
 * @param fix_node an avl tree node object to update its height
 */
static void avl_update_node_height(avlTreeNode *fix_node) {
    // Check if input data is valid
    if (fix_node != NULL) {
        // Set default heights
        int left_node_height = 0;
        int right_node_height = 0;

        // Update left height
        if (fix_node->left != NULL)
            left_node_height = fix_node->left->height;

        // Update right height
        if (fix_node->right != NULL)
            right_node_height = fix_node->right->height;

        // Update node height
        fix_node->height = _MAX(left_node_height, right_node_height) + 1;
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
static void avl_rotate_left(avlTree *tree, avlTreeNode *fix_node) {
    // Check if input data is valid
    if (tree == NULL || fix_node == NULL)
        return;

    // Check if rotation may happen
    if (fix_node->right == NULL)
        return;

    // Set new rotated sub-root
    avlTreeNode *rotate_node = fix_node->right;

    // Update child of fix_node
    fix_node->right = rotate_node->left;

    // Update child parent to fix_node
    if (rotate_node->left)
        rotate_node->left->parent = fix_node;

    // Rotation to left
    rotate_node->left = fix_node;

    // Update new sub-root parent
    rotate_node->parent = fix_node->parent;

    // Update fix_node parent to new sub-root
    fix_node->parent = rotate_node;

    // Update new sub-root links to the rest of tree
    if (rotate_node->parent != NULL) {
        if (tree->compareData(rotate_node->data, rotate_node->parent->data) >= 1)
            rotate_node->parent->right = rotate_node;
        else
            rotate_node->parent->left = rotate_node;
    } else {
        tree->root = rotate_node;
    }

    // Update the height of rotated avl tree node objects
    avl_update_node_height(fix_node);
    avl_update_node_height(rotate_node);
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
static void avl_rotate_right(avlTree *tree, avlTreeNode *fix_node) {
    // Check if input data is valid
    if (tree == NULL || fix_node == NULL)
        return;

    // Check if rotation may happen
    if (fix_node->left == NULL)
        return;

    // Set new rotated sub-root
    avlTreeNode *rotate_node = fix_node->left;

    // Update child of fix_node
    fix_node->left = rotate_node->right;

    // Update child parent to fix_node
    if (rotate_node->right)
        rotate_node->right->parent = fix_node;

    // Rotation to right
    rotate_node->right = fix_node;

    // Update new sub-root parent
    rotate_node->parent = fix_node->parent;

    // Update fix_node parent to new sub-root
    fix_node->parent = rotate_node;

    // Update new sub-root links to the rest of tree
    if (rotate_node->parent != NULL) {
        if (tree->compareData(rotate_node->data, rotate_node->parent->data) >= 1)
            rotate_node->parent->right = rotate_node;
        else
            rotate_node->parent->left = rotate_node;
    } else {
        tree->root = rotate_node;
    }

    // Update the height of rotated avl tree node objects
    avl_update_node_height(fix_node);
    avl_update_node_height(rotate_node);
}

/**
 * @brief Function to get the balance factor of an
 * avl tree node object from current working tree
 * 
 * @param fix_node pointer to avl tree node object
 * @return int balance factor of the fix_node avlTreeNode
 */
static int avl_get_node_balance(avlTreeNode *fix_node) {
    // A NULL node is balanced
    if (fix_node == NULL)
        return 0;
    
    // A node with no child is balanced
    if (fix_node->left == NULL && fix_node->right == NULL)
        return 0;
    
    // Node is left balanced
    if (fix_node->left != NULL && fix_node->right == NULL)
        return fix_node->left->height;
    
    // Node is right balanced
    if (fix_node->left == NULL && fix_node->right != NULL)
        return -fix_node->right->height;
    
    // Return balance factor of the node
    return fix_node->left->height - fix_node->right->height;
}

/**
 * @brief Helper function to fix up the balance of a avlTree
 * after insertion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 */
static void avl_insert_fix_node_up(avlTree *tree, avlTreeNode *fix_node) {
    // Check if input data is valid
    if (tree == NULL || fix_node == NULL)
        return;

    // Fix avl tree
    while (fix_node != NULL) {
        // Update height of the current node
        avl_update_node_height(fix_node);

        // Get balance factors of the current node
        int avl_node_balance_factor = avl_get_node_balance(fix_node);
        int avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        // Left-Left rotation case
        if (avl_node_balance_factor == 2 && avl_node_left_balance_factor == 1) {
            avl_rotate_right(tree, fix_node);
        }

        // Right-Right rotation case
        if (avl_node_balance_factor == -2 && avl_node_right_balance_factor == -1) {
            avl_rotate_left(tree, fix_node);
        }

        // Left-Right rotation case
        if (avl_node_balance_factor == 2 && avl_node_left_balance_factor == -1) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        // Right-Left rotation case
        if (avl_node_balance_factor == -2 && avl_node_right_balance_factor == 1) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        // Fix next node
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
 * @param dataSize size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * inserting in avl went successfully
 */
int avl_insert(avlTree *tree, const void *data, size_t dataSize) {
    // Check if tree and data are valid
    if (tree == NULL || data == NULL || dataSize == 0)
        return 1;

    // Set iterator pointers  
    avlTreeNode *iterator = tree->root;
    avlTreeNode *parent_iterator = NULL;

    // Find a valid position for insertion
    while (iterator != NULL) {
        parent_iterator = iterator;

        if (tree->compareData(iterator->data, data) >= 1)
            iterator = iterator->left;
        else if (tree->compareData(iterator->data, data) <= -1)
            iterator = iterator->right;
        else {
            // Node already exists in current avl tree
            // increment count value of node
            ++(iterator->count);
            return 0;
        }
    }

    // Create a new avl node object
    avlTreeNode *newNode = create_avl_node(data, dataSize);

    // Check if new avl node was created
    if (newNode == NULL) 
        return 1;
        
    if (parent_iterator != NULL) {
        // Update parent links
        newNode->parent = parent_iterator;

        // Update children links
        if (tree->compareData(parent_iterator->data, newNode->data) >= 1)
            parent_iterator->left = newNode;
        else
            parent_iterator->right = newNode;

        avl_insert_fix_node_up(tree, parent_iterator);
    } else {
        // Created node is root node
        tree->root = newNode;
    }

    // Increase avl tree size
    ++(tree->size);

    // Insertion in avl went successfully
    return 0;
}

/**
 * @brief Function to search data in avl tree O(log N).
 * Function will start searching from root node specified in 
 * parameters list of function.
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current working avl node object
 * @param data pointer to an address of a generic data type
 * @return avlTreeNode* avl tree node object containing
 * data value or NULL in case no such node exists
 */
static avlTreeNode* avl_find_data_set_root(avlTree *tree, avlTreeNode *root, const void *data) {
    // Check if input data is valid
    if (tree == NULL || root == NULL)
        return NULL;

    // Set iterator pointer
    avlTreeNode *iterator = root;

    // Search for input data (void *data),
    // from root - subtree
    while (iterator) {
        if (tree->compareData(iterator->data, data) <= -1)
            iterator = iterator->right;
        else if (tree->compareData(iterator->data, data) >= 1)
            iterator = iterator->left;
        else
            return iterator;
    }

    // Data was not found
    return NULL;
}

/**
 * @brief Function to search data in avl tree O(log N).
 * Function will start searching from avl tree root and will
 * search the data value in all tree.
 * 
 * @param tree an allocated avl tree object
 * @param data pointer to an address of a generic data type
 * @return avlTreeNode* avl tree node object containing
 * data value or NULL in case no such node exists
 */
avlTreeNode* avl_find_data(avlTree *tree, const void *data) {
    // Check if input data is valid
    if (tree == NULL || tree->root == NULL)
        return NULL;

    // Set iterator pointer
    avlTreeNode *iterator = tree->root;

    // Search for imput data (void *data) in all tree
    while (iterator) {
        if (tree->compareData(iterator->data, data) <= -1)
            iterator = iterator->right;
        else if (tree->compareData(iterator->data, data) >= 1)
            iterator = iterator->left;
        else
            return iterator;
    }

    // Data was not found
    return NULL;
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
 * @param dataSize size of a generic data type element
 */
static void avl_change_data(avlTreeNode *dest_node, const avlTreeNode *src_node, size_t dataSize) {
    // Check if input data is valid
    if (dest_node == NULL || src_node == NULL || dataSize == 0)
        return;

    // Check if data pointers are allocated
    if (dest_node->data == NULL || src_node->data == NULL)
        return;

    // Rewrite bytes into dest_node from src_node
    memmove(dest_node->data, src_node->data, dataSize);

    // Update count parameter
    dest_node->count = src_node->count;
    dest_node->height = src_node->height;
}

/**
 * @brief Function to calculate the level(depth) of
 * a node in avl tree. Function may fail if input node
 * is not valid (allocated).
 * 
 * @param baseNode avl node object to calculate its level
 * @return int level of input avl object node
 */
int avl_node_level(avlTreeNode *baseNode) {
    // Check if input data is valid
    if (baseNode == NULL)
        return -1;

    // Set level of node as -1
    int level_count = -1;

    // Compute level of input node
    while (baseNode != NULL) {
        baseNode = baseNode->parent;
        ++level_count;
    }

    // Return node level
    return level_count;
}

/**
 * @brief Function to get root node of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return avlTreeNode* the root node of the current avl tree
 */
avlTreeNode* get_avl_root(avlTree *tree) {
    if (tree == NULL)
        return NULL;

    return tree->root;
}

/**
 * @brief Function to get size of the avl tree.
 * 
 * @param tree an allocated avl tree object
 * @return size_t size of the current avl tree
 */
size_t get_avl_size(avlTree *tree) {
    if (tree == NULL)
        return -1;

    return tree->size;
}

/**
 * @brief Function to get node with maximum data value.
 * Function will search the maximum considering root node
 * as the beginning of the tree (root != tree(root)).
 * 
 * @param root pointer to current working avl node object
 * @return avlTreeNode* pointer to maximum node value from avl
 */
avlTreeNode* avl_max_node(avlTreeNode *root) {
    if (root) {
        while (root->right != NULL)
            root = root->right;
    }

    return root;
}

/**
 * @brief Function to get node with minimum data value.
 * Function will search the minimum considering root node
 * as the beginning of the tree (root != tree(root)).
 * 
 * @param root pointer to current working avl node object
 * @return avlTreeNode* pointer to minimum node value from avl
 */
avlTreeNode* avl_min_node(avlTreeNode *root) {
    if (root) {
        while (root->left != NULL)
            root = root->left;
    }

    return root;
}

/**
 * @brief Function to get the maximum data value from avl.
 * Function will search the maximum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param root pointer to current working avl node object
 * @return void* pointer to maximum data value from avl tree
 */
void* avl_max_data(avlTreeNode *root) {
    // Check if input data is valid
    if (root == NULL)
        return NULL;

    // Get maximum node from avl
    avlTreeNode *max_node = avl_max_node(root);

    // Return data pointer if node is not NULL
    if (max_node)
        return max_node->data;
    
    // Function failed
    return NULL;
}

/**
 * @brief Function to get the minimum data value from avl.
 * Function will search the minimum data considering root node
 * as the beginning of the tree (root != tree(root))
 * 
 * @param root pointer to current working avl node object
 * @return void* pointer to minimum data value from avl tree
 */
void* avl_min_data(avlTreeNode *root) {
    // Check if input data is valid
    if (root == NULL)
        return NULL;

    // Get minimum node from avl
    avlTreeNode *min_node = avl_min_node(root);

    // Return data pointer if node is not NULL
    if (min_node)
        return min_node->data;
    
    // Function failed
    return NULL;
}

/**
 * @brief Helper function to fix up the balance of a avlTree
 * after deletion of one node. Function may fail if current
 * working tree and node are NULL.
 * 
 * @param tree an allocated avl tree object
 * @param fix_node a pointer to a avl tree node object to start
 * fixing the balance
 */
static void avl_delete_fix_node_up(avlTree *tree, avlTreeNode *fix_node) {
    // Check if input data is valid
    if (tree == NULL || fix_node == NULL)
        return;

    // Fix avl tree
    while (fix_node != NULL) {
        // Update height of the current node
        avl_update_node_height(fix_node);

        // Get balance factors of the current node
        int avl_node_balance_factor = avl_get_node_balance(fix_node);
        int avl_node_left_balance_factor = avl_get_node_balance(fix_node->left);
        int avl_node_right_balance_factor = avl_get_node_balance(fix_node->right);

        // Left-Left rotation case
        if (avl_node_balance_factor > 1 && avl_node_left_balance_factor >= 0) {
            avl_rotate_right(tree, fix_node);
        }

        // Right-Right rotation case
        if (avl_node_balance_factor < -1 && avl_node_right_balance_factor <= 0) {
            avl_rotate_left(tree, fix_node);
        }

        // Left-Right rotation case
        if (avl_node_balance_factor > 1 && avl_node_left_balance_factor < 0) {
            avl_rotate_left(tree, fix_node->left);
            avl_rotate_right(tree, fix_node);
        }

        // Right-Left rotation case
        if (avl_node_balance_factor < -1 && avl_node_right_balance_factor > 0) {
            avl_rotate_right(tree, fix_node->right);
            avl_rotate_left(tree, fix_node);
        }

        // Fix next node
        fix_node = fix_node->parent;
    }
}

/**
 * @brief Helper function for avl_delete_data function.
 * Function will remove one data at a time and will preserve
 * the proprety of a avl tree.
 * 
 * @param tree an allocated avl tree object
 * @param root pointer to current working sub-tree
 * @param data pointer to an address of a generic data to be deleted
 * @param dataSize size of one generic data
 */
static void avl_delete_helper(avlTree *tree, avlTreeNode *root, void *data, size_t dataSize) {
    // Check if input data is valid
    if (tree == NULL || root == NULL)
        return;

    // Find current node (root) in avl tree
    avlTreeNode *delete_node = avl_find_data_set_root(tree, root, data);

    // Bst node was not found exit process
    if (delete_node == NULL)
        return;

    // Delete selected node
    if (delete_node->left != NULL && delete_node->right != NULL) {
        // Selected node has two children

        // Find a replacement for selected node
        avlTreeNode *delete_succecessor = avl_min_node(delete_node->right);
                
        // Replace the selected avl node and remove the dublicate
        avl_change_data(delete_node, delete_succecessor, dataSize);
        avl_delete_helper(tree, delete_node->right, delete_succecessor->data, dataSize);
    } else {
        // Selected node has one or no chlid

        if (delete_node->left != NULL) {
            // Selected node has a left child

            // Update child-grandparent links
            delete_node->left->parent = delete_node->parent;

            if (delete_node->parent != NULL) {
                // Update grandparent-child links

                if (delete_node->parent->right == delete_node)
                    delete_node->parent->right = delete_node->left;
                else
                    delete_node->parent->left = delete_node->left;
            } else {
                // Selected node was root
                // Update a new root
                tree->root = delete_node->left;
            }
        } else if (delete_node->right != NULL) {
            // Selected node has a right child

            // Update child-grandparent links
            delete_node->right->parent = delete_node->parent;

            if (delete_node->parent != NULL) {
                // Update grandparent-child links

                if (delete_node->parent->right == delete_node)
                    delete_node->parent->right = delete_node->right;
                else
                    delete_node->parent->left = delete_node->right;
            } else {
                // Selected node was root
                // Update a new root
                tree->root = delete_node->right;
            }
        } else {
            // Selected node has no children

            // Update grandparent links
            if (delete_node->parent != NULL) {
                if (delete_node->parent->right == delete_node)
                    delete_node->parent->right = NULL;
                else
                    delete_node->parent->left = NULL;
                } else {
                    // Selected node was root
                    // Update new root to NULL
                    tree->root = NULL;
                }
        }

        avlTreeNode *parent_delete_node = delete_node->parent;

        // Free content of the data pointer
        if (tree->freeData != NULL && delete_node->data != NULL)
            tree->freeData(delete_node->data);

        // Free data pointer of selected node
        if (delete_node->data != NULL)
            free(delete_node->data);

        // Set data pointer as NULL
        delete_node->data = NULL;

        // Free selected avl node pointer
        if (delete_node != NULL)
            free(delete_node);

        // Set selected avl node as NULL
        delete_node = NULL;

        avl_delete_fix_node_up(tree, parent_delete_node);

        // Deacrease tree size 
        --(tree->size);
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
 * @param dataSize size of one generic data
 * @return int 1(Fail) if function failed or 0(Success) if
 * deletion from avl went successfully
 */
int avl_delete(avlTree *tree, void *data, size_t dataSize) {
    // Check if input data is valid
    if (tree == NULL || tree->root == NULL || dataSize == 0)
        return 1;

    // Call helper function for deletion
    avl_delete_helper(tree, tree->root, data, dataSize);

    // Deletion went successfully
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
 * @return avlTreeNode* NULL or inorder predecessor of the
 * node containing (void *data) value.
 */
avlTreeNode* avl_predecessor_node(avlTree *tree, const void *data) {
    // Check if input data is valid
    if (tree == NULL || tree->root == NULL || data == NULL)
        return NULL;

    // Find node containing the data value
    avlTreeNode *iterator = avl_find_data(tree, data);

    // If node is not in avl than return NULL
    if (iterator == NULL)
        return NULL;

    // If node has a left child than
    // find predecessor in left subtree
    if (iterator->left != NULL)
        return avl_max_node(iterator->left);

    // Set parent iterator
    avlTreeNode *parent_iterator = iterator->parent;

    // Find predecessor node
    while (parent_iterator != NULL && parent_iterator->left == iterator) {
        iterator = parent_iterator;
        parent_iterator = parent_iterator->parent;
    }

    // Return predecessor node of the data value node
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
 * @return avlTreeNode* NULL or inorder successor of the
 * node containing (void *data) value.
 */
avlTreeNode* avl_successor_node(avlTree *tree, const void *data) {
    // Check if input data is valid
    if (tree == NULL || tree->root == NULL || data == NULL)
        return NULL;

    // Find node containing the data value
    avlTreeNode *iterator = avl_find_data(tree, data);

    // If node is not in avl than return NULL
    if (iterator == NULL)
        return NULL;

    // If node has a right child than
    // find successor in right subtree
    if (iterator->right != NULL)
        return avl_min_node(iterator->right);

    // Set parent iterator
    avlTreeNode *parent_iterator = iterator->parent;

    // Find successor node
    while (parent_iterator != NULL && parent_iterator->right == iterator) {
        iterator = parent_iterator;
        parent_iterator = parent_iterator->parent;
    }

    // Return successor node of the data value node
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
void* avl_predecessor_data(avlTree *tree, const void *data) {
    // Check if input data is valid
    if (tree == NULL || data == NULL)
        return NULL;

    // Get the predecessor node
    avlTreeNode *predecessor_node = avl_predecessor_node(tree, data);

    // Return data pointer if node is not NULL
    if (predecessor_node != NULL)
        return predecessor_node->data;

    // Function failed
    return NULL;
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
void* avl_succecessor_data(avlTree *tree, const void *data) {
    // Check if input data is valid
    if (tree == NULL || data == NULL)
        return NULL;

    // Get the successor node
    avlTreeNode *successor_node = avl_successor_node(tree, data);

    // Return data pointer if nodse is not NULL
    if (successor_node != NULL)
        return successor_node->data;

    // Function failed
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
 * @param tree an allocated avl tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return avlTreeNode* pointer to a avl node object that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
avlTreeNode* avl_lowest_common_ancestor_node(avlTree *tree, const void *data1, const void *data2) {
    // Check if input data is valid
    if (tree == NULL || data1 == NULL || data2 == NULL)
        return NULL;

    // Check if both nodes are in the current working avl tree
    if (avl_find_data(tree, data1) == NULL || avl_find_data(tree, data2) == NULL)
        return NULL;

    // Set iterator pointer
    avlTreeNode *iterator = tree->root;

    // Find the lowest common ancestor
    while (iterator != NULL) {
        if ((tree->compareData(iterator->data, data1) >= 1) && (tree->compareData(iterator->data, data2) >= 1))
            iterator = iterator->left;
        else if ((tree->compareData(iterator->data, data1) <= -1) && (tree->compareData(iterator->data, data2) <= -1))
            iterator = iterator->right;
        else
            // Ancestor found
            return iterator;
    }

    // Function failed
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
 * @param tree an allocated avl tree object
 * @param data1 pointer to an address of a generic data
 * @param data2 pointer to an address of a generic data
 * @return void* pointer to a avl node object data that is the lowest
 * common ancestor node of the two nodes containing data1 and data2
 */
void* avl_lowest_common_ancestor_data(avlTree *tree, const void *data1, const void *data2) {
    // Check if input data is valid
    if (tree == NULL || data1 == NULL || data2 == NULL)
        return NULL;

    // Get the lowest common ancestor node
    avlTreeNode *common_ancestor = avl_lowest_common_ancestor_node(tree, data1, data2);

    // Return data pointer if node is not NULL
    if (common_ancestor != NULL)
        return common_ancestor->data;

    // Function failed
    return NULL;
}

/**
 * @brief Helper function for avl_traverse_inorder function.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_inorder_helper(avlTreeNode *root, void (*action)(const avlTreeNode *)) {
    // Check if current working avl node is not NULL
    if (root == NULL)
        return;

    // Traverse in the left sub-tree
    avl_traverse_inorder_helper(root->left, action);
    
    // Call action function
    action(root);

    // Traverse in the right sub-tree
    avl_traverse_inorder_helper(root->right, action);
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
void avl_traverse_inorder(avlTree *tree, void (*action)(const avlTreeNode *)) {
    // Check if input data is valid
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        // Tree is empty no node to traverse
        printf("(Null)\n");
    else
        // Call helper function and traverse all nodes
        avl_traverse_inorder_helper(tree->root, action);
}

/**
 * @brief Helper function for avl_traverse_preorder function.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_preorder_helper(avlTreeNode *root, void (*action)(const avlTreeNode *)) {
    // Check if current working avl node is not NULL
    if (root == NULL)
        return;

    // Call action function
    action(root);

    // Traverse in the left sub-tree
    avl_traverse_preorder_helper(root->left, action);

    // Traverse in the right sub-tree
    avl_traverse_preorder_helper(root->right, action);
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
void avl_traverse_preorder(avlTree *tree, void (*action)(const avlTreeNode *)) {
    // Check if input data is valid
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        // Tree is empty no node to traverse
        printf("(Null)\n");
    else
        // Call helper function and traverse all nodes
        avl_traverse_preorder_helper(tree->root, action);
}

/**
 * @brief Helper function for avl_traverse_postorder function.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param root starting point of the avl tree traversal
 * @param action a pointer function to perform an action on one avl node object
 */
static void avl_traverse_postorder_helper(avlTreeNode *root, void (*action)(const avlTreeNode *)) {
    // Check if current working avl node is not NULL
    if (root == NULL)
        return;

    // Traverse in the left sub-tree
    avl_traverse_postorder_helper(root->left, action);

    // Traverse in the right sub-tree
    avl_traverse_postorder_helper(root->right, action);

    // Call action function
    action(root);
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
void avl_traverse_postorder(avlTree *tree, void (*action)(const avlTreeNode *)) {
    // Check if input data is valid
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        // Tree is empty no node to traverse
        printf("(Null)\n");
    else
        // Call helper function and traverse all nodes
        avl_traverse_postorder_helper(tree->root, action);
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
void avl_traverse_level(avlTree *tree, void (*action)(const avlTreeNode *)) {
    // Check if input data is valid
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        // Tree is empty no node to traverse
        printf("(Null)\n");
    else {
        // Create a queue for bfs tree traversal
        TQueue *level_queue = create_queue(0);

        // Check if queue was created successfully
        if (level_queue) {
            // Push pointer to root node into qeuue
            queue_push(level_queue, &tree->root, sizeof(avlTreeNode *));

            // Traverse all nodes
            while (!is_queue_empty(level_queue)) {
                // Get front node from queue
                avlTreeNode *front_node = *(avlTreeNode **)queue_front(level_queue);

                // Remove front node from queue
                queue_pop(level_queue);

                // Call action function on front node
                action(front_node);

                // Push on queue front left child if it exists
                if (front_node->left != NULL)
                    queue_push(level_queue, &front_node->left, sizeof(avlTreeNode *));
                
                // Push on queue front right child if it exists
                if (front_node->right != NULL)
                    queue_push(level_queue, &front_node->right, sizeof(avlTreeNode *));
            }

            // Free queue object from heap
            free_queue(level_queue);
        }
    }
}