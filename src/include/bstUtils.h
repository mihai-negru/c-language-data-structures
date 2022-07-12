/**
 * @file bstUtils.h
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

#ifndef BST_UTILS_H_
#define BST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Binary Search Tree Node object definition
 * 
 */
typedef struct bstNode {
    void *data;                                         // Pointer to data
    struct bstNode *parent;                             // Pointer to parent node
    struct bstNode *left;                               // Pointer to left child node
    struct bstNode *right;                              // Pointer to right child node
    int count;                                          // Number of nodes with the same data value
} TBstNode;

/**
 * @brief Binary Search Tree object definition
 * 
 */
typedef struct {
    TBstNode *root;                                     // Pointer to tree root
    int (*compareData)(const void *, const void *);     // Function to compare two elements
    void (*freeData)(void *);                           // Function to free content of data
    size_t size;                                        // Size of the binary search tree
} bstTree;

bstTree* create_bst(
    int (*compareData)(const void *, const void *),
    void (*freeData)(void *)
);

void free_bst(
    bstTree *tree
);

int is_bst_empty(
    bstTree *tree
);

int bst_insert(
    bstTree *tree,
    const void *data,
    size_t dataSize
);

TBstNode* bst_find_data(
    bstTree *tree,
    const void *data
);

int bst_node_level(
    TBstNode *baseNode
);

TBstNode* get_bst_root(
    bstTree *tree
);

size_t get_bst_size(
    bstTree *tree
);

TBstNode* bst_max_node(
    TBstNode *root
);

TBstNode* bst_min_node(
    TBstNode *root
);

void* bst_max_data(
    TBstNode *root
);

void* bst_min_data(
    TBstNode *root
);

int bst_delete(
    bstTree *tree,
    void *data,
    size_t dataSize
);

TBstNode* bst_predecessor_node(
    bstTree *tree,
    const void *data
);

TBstNode* bst_successor_node(
    bstTree *tree,
    const void *data
);

void* bst_predecessor_data(
    bstTree *tree,
    const void *data
);

void* bst_succecessor_data(
    bstTree *tree,
    const void *data
);

TBstNode* bst_lowest_common_ancestor_node(
    bstTree *tree,
    const void *data1,
    const void *data2
);

void* bst_lowest_common_ancestor_data(
    bstTree *tree,
    const void *data1,
    const void *data2
);

void bst_traverse_inorder(
    bstTree *tree,
    void (*action)(const TBstNode *)
);

void bst_traverse_preorder(
    bstTree *tree,
    void (*action)(const TBstNode *)
);

void bst_traverse_postorder(
    bstTree *tree,
    void (*action)(const TBstNode *)
);

void bst_traverse_level(
    bstTree *tree,
    void (*action)(const TBstNode *)
);

#endif // BST_UTILS_H_