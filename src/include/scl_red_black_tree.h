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
typedef enum {
    RED,                                                        /* Red color of the node */
    BLACK                                                       /* Black color of the node */
} node_color;

/**
 * @brief Red-Black Tree Node object definition
 * 
 */
typedef struct rbk_tree_node {
    void *data;                                                 /* Pointer to data */
    struct rbk_tree_node *parent;                               /* Pointer to parent node */
    struct rbk_tree_node *left;                                 /* Pointer to left child node */
    struct rbk_tree_node *right;                                /* Pointer to right child node */
    uint32_t count;                                             /* Number of nodes with the same data value */
    node_color color;                                           /* Color of a node */
} rbk_tree_node_t;

/**
 * @brief Red-Black Tree object definition
 * 
 */
typedef struct {
    rbk_tree_node_t *root;                                      /* Pointer to tree root */
    rbk_tree_node_t *nil;                                       /* Black hole pointer */
    compare_func cmp;                                           /* Function to compare two elements */
    free_func frd;                                              /* Function to free content of data */
    size_t size;                                                /* Size of the red-black tree */
} rbk_tree_t;

rbk_tree_t*             create_rbk                          (compare_func cmp, free_func frd);
scl_error_t             free_rbk                            (rbk_tree_t *tree);

scl_error_t             rbk_insert                          (rbk_tree_t *tree, const void *data, size_t data_size);
rbk_tree_node_t*        rbk_find_data                       (rbk_tree_t *tree, const void *data);
int32_t                 rbk_node_level                      (rbk_tree_t *tree, const rbk_tree_node_t *base_node);

uint8_t                 is_rbk_empty                        (rbk_tree_t *tree);
rbk_tree_node_t*        get_rbk_root                        (rbk_tree_t *tree);
size_t                  get_rbk_size                        (rbk_tree_t *tree);

rbk_tree_node_t*        rbk_max_node                        (rbk_tree_t *tree, rbk_tree_node_t *root);
rbk_tree_node_t*        rbk_min_node                        (rbk_tree_t *tree, rbk_tree_node_t *root);
void*                   rbk_max_data                        (rbk_tree_t *tree, rbk_tree_node_t *root);
void*                   rbk_min_data                        (rbk_tree_t *tree, rbk_tree_node_t *root);

scl_error_t             rbk_delete                          (rbk_tree_t *tree, void *data, size_t data_size);

rbk_tree_node_t*        rbk_predecessor_node                (rbk_tree_t *tree, const void *data);
rbk_tree_node_t*        rbk_successor_node                  (rbk_tree_t *tree, const void *data);
void*                   rbk_predecessor_data                (rbk_tree_t *tree, const void *data);
void*                   rbk_succecessor_data                (rbk_tree_t *tree, const void *data);
rbk_tree_node_t*        rbk_lowest_common_ancestor_node     (rbk_tree_t *tree, const void *data1, const void *data2);
void*                   rbk_lowest_common_ancestor_data     (rbk_tree_t *tree, const void *data1, const void *data2);

scl_error_t             rbk_traverse_inorder                (rbk_tree_t *tree, action_func action);
scl_error_t             rbk_traverse_preorder               (rbk_tree_t *tree, action_func action);
scl_error_t             rbk_traverse_postorder              (rbk_tree_t *tree, action_func action);
scl_error_t             rbk_traverse_level                  (rbk_tree_t *tree, action_func action);

#endif /* _RED_BLACK_TREE_UTILS_H_ */