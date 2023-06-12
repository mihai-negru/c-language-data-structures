/**
 * @file scl_list.h
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

#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Linked List Node object definition
 * 
 */
typedef struct list_node_s {
    void *data;                                         /* Pointer to data */
    struct list_node_s *next;                           /* Pointer to next node */
} list_node_t;

/**
 * @brief Linked List object definition
 * 
 */
typedef struct list_s {
    list_node_t *head;                                  /* head of linked list */
    list_node_t *tail;                                  /* tail of linked list */
    compare_func cmp;                                   /* function to compare items */
    free_func frd;                                      /* function to free item */
    size_t data_size;                                   /* Length in bytes of the data data type */
    size_t size;                                        /* size of linked list */
} list_t;

list_t*         create_list         (compare_func cmp, free_func frd, size_t data_size);
scl_error_t     free_list           (list_t * const __restrict__ list);

uint8_t         is_list_empty       (const list_t * const __restrict__ list);
size_t          get_list_size       (const list_t * const __restrict__ list);
const void*     get_list_head       (const list_t * const __restrict__ list);
const void*     get_list_tail       (const list_t * const __restrict__ list);

scl_error_t     list_swap_data      (const list_t * const __restrict__ list, const void * const __restrict__ first_data, const void * const __restrict__ second_data);
scl_error_t     list_change_data    (const list_t * const __restrict__ list, const void * const __restrict__ base_data, const void * __restrict__ new_data);
scl_error_t     list_insert         (list_t * const __restrict__ list, const void * __restrict__ data);
scl_error_t     list_insert_order   (list_t * const __restrict__ list, const void * __restrict__ data);
scl_error_t     list_insert_front   (list_t * const __restrict__ list, const void * __restrict__ data);
scl_error_t     list_insert_index   (list_t * const __restrict__ list, const void * __restrict__ data, size_t data_index);

const void*     list_find_index     (const list_t * const __restrict__ list, size_t data_index);
const void*     list_find_data      (const list_t * const __restrict__ list, const void * const __restrict__ data);

scl_error_t     list_delete_data    (list_t * const __restrict__ list, const void * const __restrict__ data);
scl_error_t     list_delete_index   (list_t * const __restrict__ list, size_t data_index);
scl_error_t     list_erase          (list_t * const __restrict__ list, size_t left_index, size_t right_index);

list_t*         list_filter         (const list_t * const __restrict__ list, filter_func filter);
scl_error_t     list_traverse       (const list_t * const __restrict__ list, action_func map);

#endif /* LIST_UTILS_H_ */
