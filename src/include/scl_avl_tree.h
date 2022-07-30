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
typedef struct avl_tree_node {
    void *data;                                                 /* Pointer to data */
    struct avl_tree_node *parent;                               /* Pointer to parent node */
    struct avl_tree_node *left;                                 /* Pointer to left child node */
    struct avl_tree_node *right;                                /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    uint32_t height;                                            /* Height of a node */
} avl_tree_node_t;

/**
 * @brief Adelson-Velsky-Landis Tree object definition
 * 
 */
typedef struct {
    avl_tree_node_t *root;                                      /* Pointer to tree root */
    avl_tree_node_t *nil;                                       /* Black hole pointer */
    compare_func cmp;                                           /* Function to compare two elements */
    free_func frd;                                              /* Function to free content of data */
    size_t size;                                                /* Size of the avl tree */
} avl_tree_t;

avl_tree_t*             create_avl                          (compare_func cmp, free_func frd);
scl_error_t             free_avl                            (avl_tree_t *tree);

scl_error_t             avl_insert                          (avl_tree_t *tree, const void *data, size_t data_size);
avl_tree_node_t*        avl_find_data                       (avl_tree_t *tree, const void *data);
int32_t                 avl_node_level                      (avl_tree_t *tree, const avl_tree_node_t *base_node);

uint8_t                 is_avl_empty                        (avl_tree_t *tree);
avl_tree_node_t*        get_avl_root                        (avl_tree_t *tree);
size_t                  get_avl_size                        (avl_tree_t *tree);

avl_tree_node_t*        avl_max_node                        (avl_tree_t *tree, avl_tree_node_t *root);
avl_tree_node_t*        avl_min_node                        (avl_tree_t *tree, avl_tree_node_t *root);
void*                   avl_max_data                        (avl_tree_t *tree, avl_tree_node_t *root);
void*                   avl_min_data                        (avl_tree_t *tree, avl_tree_node_t *root);

scl_error_t             avl_delete                          (avl_tree_t *tree, void *data, size_t data_size);

avl_tree_node_t*        avl_predecessor_node                (avl_tree_t *tree, const void *data);
avl_tree_node_t*        avl_successor_node                  (avl_tree_t *tree, const void *data);
void*                   avl_predecessor_data                (avl_tree_t *tree, const void *data);
void*                   avl_succecessor_data                (avl_tree_t *tree, const void *data);
avl_tree_node_t*        avl_lowest_common_ancestor_node     (avl_tree_t *tree, const void *data1, const void *data2);
void*                   avl_lowest_common_ancestor_data     (avl_tree_t *tree, const void *data1, const void *data2);

scl_error_t             avl_traverse_inorder                (avl_tree_t *tree, action_func action);
scl_error_t             avl_traverse_preorder               (avl_tree_t *tree, action_func action);
scl_error_t             avl_traverse_postorder              (avl_tree_t *tree, action_func action);
scl_error_t             avl_traverse_level                  (avl_tree_t *tree, action_func action);

#endif /* AVLTREE_UTILS_H_ */