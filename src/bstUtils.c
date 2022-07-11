/**
 * @file bstUtils.c
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
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "./include/bstUtils.h"
#include "./include/queueUtils.h"

bstTree* create_bst(int (*compareData)(const void *, const void *), void (*freeData)(void *)) {
    if (compareData == NULL) {
        errno = EINVAL;
        perror("Compare function undefined for binary search tree");
        return NULL;
    }

    bstTree* newTree = (bstTree* )malloc(sizeof(bstTree));

    if (newTree) {
        newTree->compareData = compareData;
        newTree->freeData = freeData;
        newTree->root = NULL;
        newTree->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for bst allocation");
    }
    
    return newTree;
}

static TBstNode* create_bst_node(const void *data, size_t dataSize) {
    if (data == NULL)
        return NULL;

    TBstNode *newNode = (TBstNode *)malloc(sizeof(TBstNode));

    if (newNode) {
        newNode->right = newNode->left = NULL;
        newNode->parent = NULL;
        newNode->count = 1;

        // Allocate heap memory for data
        newNode->data = malloc(dataSize);

        // Check if 
        if (newNode->data)
            // Copy all bytes from data pointer
            // to memory allocated on heap
            memcpy(newNode->data, data, dataSize);
        else {
            errno = ENOMEM;
            perror("Not enough memory for node bst data allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node bst allocation");
    }

    return newNode;
}

static void free_bst_helper(bstTree *tree, TBstNode *root) {
    if (root == NULL)
        return;

    free_bst_helper(tree, root->left);
    free_bst_helper(tree, root->right);

    if (tree->freeData != NULL && root->data != NULL)
        tree->freeData(root->data);

    if (root->data != NULL)
        free(root->data);

    root->data = NULL;

    if (root != NULL)
        free(root);

    root = NULL;
}

void free_bst(bstTree *tree) {
    if (tree) {
        free_bst_helper(tree, tree->root);
        
        free(tree);
        tree = NULL;
    }
}

int is_bst_empty(bstTree *tree) {
    if (tree == NULL || tree->root == NULL || tree->size == 0)
        return 1;

    return 0;
}

int bst_insert(bstTree *tree, const void *data, size_t dataSize) {
    if (tree == NULL || data == NULL || dataSize == 0)
        return 1;
    
    TBstNode *iterator = tree->root;
    TBstNode *parent_iterator = NULL;

    while (iterator != NULL) {
        parent_iterator = iterator;

        if (tree->compareData(iterator->data, data) >= 1)
            iterator = iterator->left;
        else if (tree->compareData(iterator->data, data) <= -1)
            iterator = iterator->right;
        else {
            ++(iterator->count);
            return 0;
        }
    }

    TBstNode *newNode = create_bst_node(data, dataSize);

    if (newNode) {
        if (parent_iterator != NULL) {
            newNode->parent = parent_iterator;

            if (tree->compareData(parent_iterator->data, newNode->data) >= 1)
                parent_iterator->left = newNode;
            else
                parent_iterator->right = newNode; 
        } else {
            tree->root = newNode;
        }

        ++(tree->size);
    } else {
        return 1;
    }

    return 0;
}

TBstNode* bst_find_data(bstTree *tree, const void *data) {
    if (tree == NULL || tree->root == NULL)
        return NULL;

    TBstNode *iterator = tree->root;

    while (iterator) {
        if (tree->compareData(iterator->data, data) <= -1)
            iterator = iterator->right;
        else if (tree->compareData(iterator->data, data) >= 1)
            iterator = iterator->left;
        else
            return iterator;
    }

    return NULL;
}

static void bst_change_data(TBstNode *dest_node, const TBstNode *src_node, size_t dataSize) {
    if (dest_node == NULL || src_node == NULL || dataSize == 0)
        return;

    if (dest_node->data == NULL || src_node->data == NULL)
        return;

    memmove(dest_node->data, src_node->data, dataSize);

    dest_node->count = src_node->count;
}

int bst_node_level(TBstNode *baseNode) {
    if (baseNode == NULL)
        return -1;

    int level_count = -1;

    while (baseNode != NULL) {
        baseNode = baseNode->parent;
        ++level_count;
    }

    return level_count;
}

TBstNode* get_bst_root(bstTree *tree) {
    if (tree == NULL)
        return NULL;

    return tree->root;
}

size_t get_bst_size(bstTree *tree) {
    if (tree == NULL)
        return -1;

    return tree->size;
}

TBstNode* bst_max_node(TBstNode *root) {
    if (root) {
        while (root->right != NULL)
            root = root->right;
    }

    return root;
}

TBstNode* bst_min_node(TBstNode *root) {
    if (root) {
        while (root->left != NULL)
            root = root->left;
    }

    return root;
}

void* bst_max_data(TBstNode *root) {
    if (root == NULL)
        return NULL;

    TBstNode *max_node = bst_max_node(root);

    if (max_node)
        return max_node->data;
    else
        return NULL;
}

void* bst_min_data(TBstNode *root) {
    if (root == NULL)
        return NULL;

    TBstNode *min_node = bst_min_node(root);

    if (min_node)
        return min_node->data;
    else
        return NULL;
}

static void bst_delete_data_helper(bstTree *tree, TBstNode *root, void *data, size_t dataSize) {
    if (tree == NULL || root == NULL)
        return;

    TBstNode *delete_node = root;

    while (delete_node) {
        if (tree->compareData(delete_node->data, data) <= -1)
            delete_node = delete_node->right;
        else if (tree->compareData(delete_node->data, data) >= 1)
            delete_node = delete_node->left;
        else {
            if (delete_node->left != NULL && delete_node->right != NULL) {
                
                TBstNode *delete_succecessor = bst_min_node(delete_node->right);
                bst_change_data(delete_node, delete_succecessor, dataSize);

                bst_delete_data_helper(tree, delete_node->right, delete_succecessor->data, dataSize);
            } else {
                if (delete_node->left != NULL) {
                    delete_node->left->parent = delete_node->parent;

                    if (delete_node->parent != NULL) {
                        if (delete_node->parent->right == delete_node)
                            delete_node->parent->right = delete_node->left;
                        else
                            delete_node->parent->left = delete_node->left;
                    } else {
                        tree->root = delete_node->left;
                    }
                } else if (delete_node->right != NULL) {
                    delete_node->right->parent = delete_node->parent;

                    if (delete_node->parent != NULL) {
                        if (delete_node->parent->right == delete_node)
                            delete_node->parent->right = delete_node->right;
                        else
                            delete_node->parent->left = delete_node->right;
                    } else {
                        tree->root = delete_node->right;
                    }
                } else {
                    if (delete_node->parent != NULL) {
                        if (delete_node->parent->right == delete_node)
                            delete_node->parent->right = NULL;
                        else
                            delete_node->parent->left = NULL;
                    } else {
                        tree->root = NULL;
                    }
                }

                if (tree->freeData != NULL && delete_node->data != NULL)
                    tree->freeData(delete_node->data);

                if (delete_node->data != NULL)
                    free(delete_node->data);

                delete_node->data = NULL;

                if (delete_node != NULL)
                    free(delete_node);

                delete_node = NULL;}
        }
    }
}

int bst_delete_data(bstTree *tree, void *data, size_t dataSize) {
    if (tree == NULL || tree->root == NULL)
        return 1;

    bst_delete_data_helper(tree, tree->root, data, dataSize);

    return 0;
}

TBstNode* bst_predecessor_node(bstTree *tree, const void *data);

TBstNode* bst_successor_node(bstTree *tree, const void *data);

void* bst_predecessor_data(bstTree *tree, const void *data);

void* bst_succecessor_data(bstTree *tree, const void *data);

TBstNode* bst_lowest_common_ancestor_node(bstTree *tree, const void *data1, const void *data2);

TBstNode* bst_lowest_common_ancestor_data(bstTree *tree, const void *data1, const void *data2);

static void bst_traverse_inorder_helper(TBstNode *root, void (*action)(const TBstNode *)) {
    if (root == NULL)
        return;

    bst_traverse_inorder_helper(root->left, action);
    action(root);
    bst_traverse_inorder_helper(root->right, action);
}

void bst_traverse_inorder(bstTree *tree, void (*action)(const TBstNode *)) {
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        printf("[ ]\n");
    else
        bst_traverse_inorder_helper(tree->root, action);
}

static void bst_traverse_preorder_helper(TBstNode *root, void (*action)(const TBstNode *)) {
    if (root == NULL)
        return;

    action(root);
    bst_traverse_preorder_helper(root->left, action);
    bst_traverse_preorder_helper(root->right, action);
}

void bst_traverse_preorder(bstTree *tree, void (*action)(const TBstNode *)) {
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        printf("[ ]\n");
    else
        bst_traverse_preorder_helper(tree->root, action);
}

static void bst_traverse_postorder_helper(TBstNode *root, void (*action)(const TBstNode *)) {
    if (root == NULL)
        return;

    bst_traverse_postorder_helper(root->left, action);
    bst_traverse_postorder_helper(root->right, action);
    action(root);
}

void bst_traverse_postorder(bstTree *tree, void (*action)(const TBstNode *)) {
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        printf("[ ]\n");
    else
        bst_traverse_postorder_helper(tree->root, action);
}

void bst_traverse_level(bstTree *tree, void (*action)(const TBstNode *)) {
    if (tree == NULL || action == NULL)
        return;

    if (tree->root == NULL)
        printf("[ ]\n");
    else {
        TQueue *level_queue = create_queue(0);

        if (level_queue) {
            queue_push(level_queue, &tree->root, sizeof(TBstNode *));

            while (!is_queue_empty(level_queue)) {
                TBstNode *front_node = *(TBstNode **)queue_front(level_queue);
                queue_pop(level_queue);

                action(front_node);

                if (front_node->left != NULL)
                    queue_push(level_queue, &front_node->left, sizeof(TBstNode *));
                
                if (front_node->right != NULL)
                    queue_push(level_queue, &front_node->right, sizeof(TBstNode *));
            }

            free_queue(level_queue);
        }
    }
}