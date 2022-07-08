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

bstTree* create_bst(int (*compareData)(const void *, const void *), void (*freeData)(void *));
void free_bst(bstTree *tree);
int is_bst_empty(bstTree *tree);
int bst_insert(bstTree *tree, const void *data, size_t dataSize);
TBstNode* bst_find_data(bstTree *tree, const void *data);
void bst_change_node_data(bstTree *tree, TBstNode *baseNode, const void *newData, size_t dataSize);
int bst_node_level(bstTree *tree, TBstNode *baseNode);
TBstNode* get_bst_root(bstTree *tree);
size_t get_bst_size(bstTree *tree);
TBstNode* bst_max_node(bstTree *tree);
TBstNode* bst_min_node(bstTree *tree);
void* bst_max_data(bstTree *tree);
void* bst_min_data(bstTree *tree);
int bst_delete_data(bstTree *tree, const void *data);
TBstNode* bst_predecessor_node(bstTree *tree, const void *data);
TBstNode* bst_successor_node(bstTree *tree, const void *data);
TBstNode* bst_lowest_common_ancestor_node(bstTree *tree, const void *data1, const void *data2);
TBstNode* bst_lowest_common_ancestor_data(bstTree *tree, const void *data1, const void *data2);
void* bst_predecessor_data(bstTree *tree, const void *data);
void* bst_succecessor_data(bstTree *tree, const void *data);
void bst_print_inorder(bstTree *tree, void (*printData)(const void *));
void bst_print_preorder(bstTree *tree, void (*printData)(const void *));
void bst_print_postorder(bstTree *tree, void (*printData)(const void *));
void bst_print_level(bstTree *tree, void (*printData)(const void *));