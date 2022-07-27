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
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Binary Search Tree Node object definition
 * 
 */
typedef struct bst_tree_node {
    void* data;                                             /* Pointer to data */
    struct bst_tree_node* parent;                           /* Pointer to parent node */
    struct bst_tree_node* left;                             /* Pointer to left child node */
    struct bst_tree_node* right;                            /* Pointer to right child node */
    uint32_t count;                                         /* Number of nodes with the same data value */
} bst_tree_node_t;

/**
 * @brief Binary Search Tree object definition
 * 
 */
typedef struct {
    bst_tree_node_t* root;                                  /* Pointer to tree root */
    bst_tree_node_t* nil;                                   /* Black hole pointer */
    compare_func cmp;                                       /* Function to compare two elements */
    free_func frd;                                          /* Function to free content of data */
    size_t size;                                            /* Size of the binary search tree */
} bst_tree_t;

typedef void (*bst_action)(bst_tree_t*tree, const bst_tree_node_t*);

bst_tree_t*             create_bst                          (compare_func cmp, free_func frd);
scl_error_t             free_bst                            (bst_tree_t* tree);

scl_error_t             bst_insert                          (bst_tree_t* tree, const void* data, size_t data_size);
bst_tree_node_t*        bst_find_data                       (bst_tree_t* tree, const void* data);
scl_error_t             bst_node_level                      (bst_tree_t* tree, const bst_tree_node_t* base_node);

uint8_t                 is_bst_empty                        (bst_tree_t* tree);
bst_tree_node_t*        get_bst_root                        (bst_tree_t* tree);
size_t                  get_bst_size                        (bst_tree_t* tree);

bst_tree_node_t*        bst_max_node                        (bst_tree_t* tree, bst_tree_node_t* root);
bst_tree_node_t*        bst_min_node                        (bst_tree_t* tree, bst_tree_node_t* root);
void*                   bst_max_data                        (bst_tree_t* tree, bst_tree_node_t* root);
void*                   bst_min_data                        (bst_tree_t* tree, bst_tree_node_t* root);

scl_error_t             bst_delete                          (bst_tree_t* tree, void* data, size_t data_size);

bst_tree_node_t*        bst_predecessor_node                (bst_tree_t* tree, const void* data);
bst_tree_node_t*        bst_successor_node                  (bst_tree_t* tree, const void* data);
void*                   bst_predecessor_data                (bst_tree_t* tree, const void* data);
void*                   bst_succecessor_data                (bst_tree_t* tree, const void* data);
bst_tree_node_t*        bst_lowest_common_ancestor_node     (bst_tree_t* tree, const void* data1, const void* data2);
void*                   bst_lowest_common_ancestor_data     (bst_tree_t* tree, const void* data1, const void* data2);

scl_error_t             bst_traverse_inorder                (bst_tree_t* tree, bst_action action);
scl_error_t             bst_traverse_preorder               (bst_tree_t* tree, bst_action action);
scl_error_t             bst_traverse_postorder              (bst_tree_t* tree, bst_action action);
scl_error_t             bst_traverse_level                  (bst_tree_t* tree, bst_action action);

#endif /* BST_UTILS_H_ */