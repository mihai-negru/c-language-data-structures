/**
 * @file scl_priority_queue.h
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

#ifndef PRIORITY_QUEUE_UTILS_H_
#define PRIORITY_QUEUE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Priority Queue Node Object definition
 * 
 */
typedef struct pri_node_s {
    void *pri;                                              /* Priority element definition */
    void *data;                                             /* Data type element definition */
} pri_node_t;

/**
 * @brief Priority Queue Object definition
 * 
 */
typedef struct priority_queue_s {
    pri_node_t **nodes;                                     /* Array of binary-heap nodes */
    compare_func cmp_dt;                                    /* Function to compare two sets of data */
    compare_func cmp_pr;                                    /* Function to compare two sets of priority */
    free_func frd_dt;                                       /* Function to free memory of a single data element */
    free_func frd_pr;                                       /* Function to free memory of a single priority element */
    size_t capacity;                                        /* Maximum capacity of the priority queue */
    size_t pri_size;                                        /* Length in bytes of the priority data type */
    size_t data_size;                                       /* Length in bytes of the data data type */
    size_t size;                                            /* Current size of the priority queue */
} priority_queue_t;

priority_queue_t*   create_priority_queue       (size_t init_capacity, compare_func cmp_pr, compare_func cmp_dt, free_func frd_pr, free_func frd_dt, size_t pri_size, size_t data_size);
scl_error_t         free_priority_queue         (priority_queue_t * const __restrict__ pqueue);
scl_error_t         heapify                     (priority_queue_t * const __restrict__ empty_pqueue, const void *priority, const void *data);

scl_error_t         change_node_priority        (const priority_queue_t * const __restrict__ pqueue, size_t node_index, const void * __restrict__ new_pri);
scl_error_t         change_node_data            (const priority_queue_t * const __restrict__ pqueue, size_t node_index, const void * __restrict__ new_data);

size_t              pri_find_data_index         (const priority_queue_t * const __restrict__ pqueue, const void * const __restrict__ data);
size_t              pri_find_pri_index          (const priority_queue_t * const __restrict__ pqueue, const void * const __restrict__ priority);

scl_error_t         pri_queue_push              (priority_queue_t * const __restrict__ pqueue, const void *priority, const void *data);
const void*         pri_queue_top               (const priority_queue_t * const __restrict__ pqueue);
const void*         pri_queue_top_pri           (const priority_queue_t * const __restrict__ pqueue);
scl_error_t         pri_queue_pop               (priority_queue_t * const __restrict__ pqueue);
scl_error_t         pri_queue_traverse          (const priority_queue_t * const __restrict__ pqueue, action_func action);

size_t              pri_queue_size              (const priority_queue_t * const __restrict__ pqueue);
uint8_t             is_priq_empty               (const priority_queue_t * const __restrict__ pqueue);

scl_error_t         heap_sort                   (void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);

#endif /* PRIORITY_QUEUE_UTILS_H_ */
