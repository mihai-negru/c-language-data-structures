/**
 * @file scl_red_black_tree.h
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

#ifndef _RED_BLACK_TREE_UTILS_H_
#define _RED_BLACK_TREE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "scl_config.h"

/**
 * @brief Color of one red-black tree node
 * 
 */
typedef enum rbk_tree_node_color_s {
    RED,                                                        /* Red color of the node */
    BLACK                                                       /* Black color of the node */
} rbk_tree_node_color_t;

/**
 * @brief Red-Black Tree Node object definition
 * 
 */
typedef struct rbk_tree_node_s {
    void *data;                                                 /* Pointer to data */
    struct rbk_tree_node_s *parent;                             /* Pointer to parent node */
    struct rbk_tree_node_s *left;                               /* Pointer to left child node */
    struct rbk_tree_node_s *right;                              /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    rbk_tree_node_color_t color;                                /* Color of a node */
} rbk_tree_node_t;

/**
 * @brief Red-Black Tree object definition
 * 
 */
typedef struct rbk_tree_s {
    rbk_tree_node_t *root;                                      /* Pointer to tree root */
    rbk_tree_node_t *nil;                                       /* Black hole pointer */
    compare_func cmp;                                           /* Function to compare two elements */
    free_func frd;                                              /* Function to free content of data */
    size_t data_size;                                           /* Length in bytes of the data data type */
    size_t size;                                                /* Size of the red-black tree */
} rbk_tree_t;

rbk_tree_t*             create_rbk                          (compare_func cmp, free_func frd, size_t data_size);
scl_error_t             free_rbk                            (rbk_tree_t * const __restrict__ tree);

scl_error_t             rbk_insert                          (rbk_tree_t * const __restrict__ tree, const void * __restrict__ data);
const void*             rbk_find_data                       (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data);
int32_t                 rbk_data_level                      (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data);

uint8_t                 is_rbk_empty                        (const rbk_tree_t * const __restrict__ tree);
const void*             get_rbk_root                        (const rbk_tree_t * const __restrict__ tree);
size_t                  get_rbk_size                        (const rbk_tree_t * const __restrict__ tree);

const void*             rbk_max_data                        (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data);
const void*             rbk_min_data                        (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data);

scl_error_t             rbk_delete                          (rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data);

const void*             rbk_predecessor_data                (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data);
const void*             rbk_successor_data                  (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data);
const void*             rbk_lowest_common_ancestor_data     (const rbk_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2);

scl_error_t             rbk_traverse_inorder                (const rbk_tree_t * const __restrict__ tree, action_func action);
scl_error_t             rbk_traverse_preorder               (const rbk_tree_t * const __restrict__ tree, action_func action);
scl_error_t             rbk_traverse_postorder              (const rbk_tree_t * const __restrict__ tree, action_func action);
scl_error_t             rbk_traverse_level                  (const rbk_tree_t * const __restrict__ tree, action_func action);

#endif /* _RED_BLACK_TREE_UTILS_H_ */
