/**
 * @file scl_bst_tree.h
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
typedef struct bst_tree_node {
    void* data;                                             /* Pointer to data */
    struct bst_tree_node* parent;                           /* Pointer to parent node */
    struct bst_tree_node* left;                             /* Pointer to left child node */
    struct bst_tree_node* right;                            /* Pointer to right child node */
    int count;                                              /* Number of nodes with the same data value */
} bst_tree_node_t;

/**
 * @brief Binary Search Tree object definition
 * 
 */
typedef struct {
    bst_tree_node_t* root;                                  /* Pointer to tree root */
    int (*compare_data)(const void*, const void*);          /* Function to compare two elements */
    void (*free_data)(void*);                               /* Function to free content of data */
    size_t size;                                            /* Size of the binary search tree */
} bst_tree_t;

bst_tree_t*             create_bst                          (int (*compare_data)(const void*, const void*), void (*free_data)(void*));
void                    free_bst                            (bst_tree_t* tree);

int                     bst_insert                          (bst_tree_t* tree, const void* data, size_t data_size);
bst_tree_node_t*        bst_find_data                       (bst_tree_t* tree, const void* data);
int                     bst_node_level                      (bst_tree_node_t* base_node);

int                     is_bst_empty                        (bst_tree_t* tree);
bst_tree_node_t*        get_bst_root                        (bst_tree_t* tree);
size_t                  get_bst_size                        (bst_tree_t* tree);

bst_tree_node_t*        bst_max_node                        (bst_tree_node_t* root);
bst_tree_node_t*        bst_min_node                        (bst_tree_node_t* root);
void*                   bst_max_data                        (bst_tree_node_t* root);
void*                   bst_min_data                        (bst_tree_node_t* root);

int                     bst_delete                          (bst_tree_t* tree, void* data, size_t data_size);

bst_tree_node_t*        bst_predecessor_node                (bst_tree_t* tree, const void* data);
bst_tree_node_t*        bst_successor_node                  (bst_tree_t* tree, const void* data);
void*                   bst_predecessor_data                (bst_tree_t* tree, const void* data);
void*                   bst_succecessor_data                (bst_tree_t* tree, const void* data);
bst_tree_node_t*        bst_lowest_common_ancestor_node     (bst_tree_t* tree, const void* data1, const void* data2);
void*                   bst_lowest_common_ancestor_data     (bst_tree_t* tree, const void* data1, const void* data2);

void                    bst_traverse_inorder                (bst_tree_t* tree, void (*action)(const bst_tree_node_t*));
void                    bst_traverse_preorder               (bst_tree_t* tree, void (*action)(const bst_tree_node_t*));
void                    bst_traverse_postorder              (bst_tree_t* tree, void (*action)(const bst_tree_node_t*));
void                    bst_traverse_level                  (bst_tree_t* tree, void (*action)(const bst_tree_node_t*));

#endif /* BST_UTILS_H_ */