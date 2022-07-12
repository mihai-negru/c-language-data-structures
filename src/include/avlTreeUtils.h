/**
 * @file avlTreeUtils.h
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

#ifndef AVLTREE_UTILS_H_
#define AVLTREE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Adelson-Velsky-Landis Tree Node object definition
 * 
 */
typedef struct avlNode {
    void *data;                                         // Pointer to data
    struct avlNode *parent;                             // Pointer to parent node
    struct avlNode *left;                               // Pointer to left child node
    struct avlNode *right;                              // Pointer to right child node
    int count;                                          // Number of nodes with the same data value
    int height;                                         // Height of a node
} avlTreeNode;

/**
 * @brief Adelson-Velsky-Landis Tree object definition
 * 
 */
typedef struct {
    avlTreeNode *root;                                  // Pointer to tree root
    int (*compareData)(const void *, const void *);     // Function to compare two elements
    void (*freeData)(void *);                           // Function to free content of data
    size_t size;                                        // Size of the avl tree
} avlTree;

avlTree* create_avl(
    int (*compareData)(const void *, const void *),
    void (*freeData)(void *)
);

void free_avl(
    avlTree *tree
);

int is_avl_empty(
    avlTree *tree
);

int avl_insert(
    avlTree *tree,
    const void *data,
    size_t dataSize
);

avlTreeNode* avl_find_data(
    avlTree *tree,
    const void *data
);

int avl_node_level(
    avlTreeNode *baseNode
);

avlTreeNode* get_avl_root(
    avlTree *tree
);

size_t get_avl_size(
    avlTree *tree
);

avlTreeNode* avl_max_node(
    avlTreeNode *root
);

avlTreeNode* avl_min_node(
    avlTreeNode *root
);

void* avl_max_data(
    avlTreeNode *root
);

void* avl_min_data(
    avlTreeNode *root
);

int avl_delete(
    avlTree *tree,
    void *data,
    size_t dataSize
);

avlTreeNode* avl_predecessor_node(
    avlTree *tree,
    const void *data
);

avlTreeNode* avl_successor_node(
    avlTree *tree,
    const void *data
);

void* avl_predecessor_data(
    avlTree *tree,
    const void *data
);

void* avl_succecessor_data(
    avlTree *tree,
    const void *data
);

avlTreeNode* avl_lowest_common_ancestor_node(
    avlTree *tree,
    const void *data1,
    const void *data2
);

void* avl_lowest_common_ancestor_data(
    avlTree *tree,
    const void *data1,
    const void *data2
);

void avl_traverse_inorder(
    avlTree *tree,
    void (*action)(const avlTreeNode *)
);

void avl_traverse_preorder(
    avlTree *tree,
    void (*action)(const avlTreeNode *)
);

void avl_traverse_postorder(
    avlTree *tree,
    void (*action)(const avlTreeNode *)
);

void avl_traverse_level(
    avlTree *tree,
    void (*action)(const avlTreeNode *)
);

#endif // AVLTREE_UTILS_H_