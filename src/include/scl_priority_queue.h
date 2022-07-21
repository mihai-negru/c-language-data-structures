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
#include <errno.h>

/**
 * @brief Priority Queue Node Object definition
 * 
 */
typedef struct pri_node{
    void* pri;                                              /* Priority element definition */
    void* data;                                             /* Data type element definition */
} pri_node_t;

/**
 * @brief Priority Queue Object definition
 * 
 */
typedef struct priority_queue{
    pri_node_t** nodes;                                     /* Array of binary-heap nodes */
    int (*compare_data)(const void*, const void*);          /* Function to compare two sets of data */
    int (*compare_priority)(const void*, const void*);      /* Function to compare two sets of priority */
    void (*free_data)(void*);                               /* Function to free memory of a single data element */
    void (*free_priority)(void*);                           /* Function to free memory of a single priority element */
    size_t capacity;                                        /* Maximum capacity of the priority queue */
    size_t size;                                            /* Current size of the priority queue */
} priority_queue_t;

priority_queue_t*   create_priority_queue       (size_t init_capacity, int (*compare_data)(const void*, const void*), int (*compare_priority)(const void*, const void*), void (*free_data)(void*), void (*free_priority)(void*));
void                free_priority_queue         (priority_queue_t* pqueue);
priority_queue_t*   heapify                     (const void* data, const void* priority, size_t data_size, size_t pri_size, size_t number_of_data, int (*compare_data)(const void*, const void*), int (*compare_priority)(const void*, const void*), void (*free_data)(void*), void (*free_priority)(void*));

int                 change_node_priority        (priority_queue_t* pqueue, size_t node_index, const void* new_pri, size_t pri_size);
int                 change_node_data            (priority_queue_t* pqueue, size_t node_index, const void* new_data, size_t data_size);

size_t              pri_find_data_index         (priority_queue_t* pqueue, const void* data);
size_t              pri_find_pri_index          (priority_queue_t* pqueue, const void* priority);

int                 pri_queue_push              (priority_queue_t* pqueue, const void* data, const void* priority, size_t data_size, size_t pri_size);
const void*         pri_queue_top               (priority_queue_t* pqueue);
const void*         pri_queue_top_pri           (priority_queue_t* pqueue);
int                 pri_queue_pop               (priority_queue_t* pqueue);
void                pri_queue_traverse          (priority_queue_t* pqueue,void (*action)(const pri_node_t*));

size_t              pri_queue_size              (priority_queue_t* pqueue);
int                 is_priq_empty               (priority_queue_t* pqueue);

void                heap_sort                   (void* arr, size_t number_of_arr, size_t arr_elem_size, int (*compare_arr)(const void*, const void*));

#endif /* PRIORITY_QUEUE_UTILS_H_ */