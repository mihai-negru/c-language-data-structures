/**
 * @file scl_dlist.h
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

#ifndef DOUBLE_LIST_UTILS_H_
#define DOUBLE_LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Double Linked List Node object definition
 * 
 */
typedef struct dlist_node {
    void *data;                                         /* Pointer to data */
    struct dlist_node *prev;                            /* Pointer to previous node */
    struct dlist_node *next;                            /* Pointer to next node */
} dlist_node_t;

/**
 * @brief Double Linked List object definition
 * 
 */
typedef struct {
    dlist_node_t *head;                                 /* head of linked list */
    dlist_node_t *tail;                                 /* tail of linked list */
    compare_func cmp;                                   /* function to compare items */
    const_action_func print;                            /* function to print item */
    free_func frd;                                      /* function to free item */
    size_t size;                                        /* size of linked list */
} dlist_t;

dlist_t*          create_dlist            (compare_func cmp, const_action_func print, free_func frd);
scl_error_t       print_front_dlist       (const dlist_t * const list);
scl_error_t       print_back_dlist        (const dlist_t * const list);
scl_error_t       free_dlist              (dlist_t * const list);

uint8_t           is_dlist_empty          (const dlist_t * const list);
size_t            get_dlist_size          (const dlist_t * const list);
void*             get_dlist_head          (const dlist_t * const list);
void*             get_dlist_tail          (const dlist_t * const list);

scl_error_t       dlist_swap_data         (const dlist_t * const list, void * const first_data, void * const second_data, size_t data_size);
scl_error_t       dlist_change_data       (const dlist_t * const list, void * const base_data, const void * const new_data, size_t data_sizee);
scl_error_t       dlist_insert            (dlist_t * const list, const void * const data, size_t data_size);
scl_error_t       dlist_insert_order      (dlist_t * const list, const void * const data, size_t data_size);
scl_error_t       dlist_insert_front      (dlist_t * const list, const void * const data, size_t data_size);
scl_error_t       dlist_insert_index      (dlist_t * const list, const void * const data, size_t data_size, size_t data_index);

void*             dlist_find_index        (const dlist_t * const list, size_t data_index);
void*             dlist_find_data         (const dlist_t * const list, const void * const data);

scl_error_t       dlist_delete_data       (dlist_t * const list, const void * const data);
scl_error_t       dlist_delete_index      (dlist_t * const list, size_t data_index);
scl_error_t       dlist_erase             (dlist_t * const list, size_t left_index, size_t right_index);

dlist_t*          dlist_filter            (const dlist_t * const list, filter_func filter, size_t data_size);
scl_error_t       dlist_map               (const dlist_t * const list, map_func map, size_t data_size);

#endif /* DOUBLE_LIST_UTILS_H_ */