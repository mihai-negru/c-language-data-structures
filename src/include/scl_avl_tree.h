/**
 * @file scl_avl_tree.h
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
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Adelson-Velsky-Landis Tree Node object definition
 * 
 */
typedef struct avl_tree_node_s {
    void *data;                                                 /* Pointer to data */
    struct avl_tree_node_s *parent;                             /* Pointer to parent node */
    struct avl_tree_node_s *left;                               /* Pointer to left child node */
    struct avl_tree_node_s *right;                              /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    uint32_t height;                                            /* Height of a node */
} avl_tree_node_t;

/**
 * @brief Adelson-Velsky-Landis Tree object definition
 * 
 */
typedef struct avl_tree_s {
    avl_tree_node_t *root;                                      /* Pointer to tree root */
    avl_tree_node_t *nil;                                       /* Black hole pointer */
    compare_func cmp;                                           /* Function to compare two elements */
    free_func frd;                                              /* Function to free content of data */
    size_t data_size;                                           /* Length in bytes of the data data type */
    size_t size;                                                /* Size of the avl tree */
} avl_tree_t;

avl_tree_t*             create_avl                          (compare_func cmp, free_func frd, size_t data_size);
scl_error_t             free_avl                            (avl_tree_t * const __restrict__ tree);

scl_error_t             avl_insert                          (avl_tree_t * const __restrict__ tree, const void * __restrict__ data);
const void*             avl_find_data                       (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data);
int32_t                 avl_data_level                      (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data);

uint8_t                 is_avl_empty                        (const avl_tree_t * const __restrict__ tree);
const void*             get_avl_root                        (const avl_tree_t * const __restrict__ tree);
size_t                  get_avl_size                        (const avl_tree_t * const __restrict__ tree);

const void*             avl_max_data                        (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data);
const void*             avl_min_data                        (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ subroot_data);

scl_error_t             avl_delete                          (avl_tree_t * const __restrict__ tree, const void * const __restrict__ data);

const void*             avl_predecessor_data                (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data);
const void*             avl_succecessor_data                (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data);
const void*             avl_lowest_common_ancestor_data     (const avl_tree_t * const __restrict__ tree, const void * const __restrict__ data1, const void * const __restrict__ data2);

scl_error_t             avl_traverse_inorder                (const avl_tree_t * const __restrict__ tree, action_func action);
scl_error_t             avl_traverse_preorder               (const avl_tree_t * const __restrict__ tree, action_func action);
scl_error_t             avl_traverse_postorder              (const avl_tree_t * const __restrict__ tree, action_func action);
scl_error_t             avl_traverse_level                  (const avl_tree_t * const __restrict__ tree, action_func action);

#endif /* AVLTREE_UTILS_H_ */