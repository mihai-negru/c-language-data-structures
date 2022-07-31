/**
 * @file scl_queue.h
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

#ifndef QUEUE_UTILS_H_
#define QUEUE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief queue Node object definition
 * 
 */
typedef struct queue_node {
    void *data;                     /* Pointer to data */
    struct queue_node *next;        /* Pointer to next data node */
} queue_node_t;

/**
 * @brief queue object definition
 * 
 */
typedef struct {
    queue_node_t *front;            /* Pointer to beginning of queue */
    queue_node_t *back;             /* Pointer to end of queue */
    free_func frd;                  /* Function to free one data */
    size_t size;                    /* Size of the queue */
} queue_t;

queue_t*        create_queue        (free_func frd);
scl_error_t     free_queue          (queue_t * const queue);
scl_error_t     print_queue         (const queue_t * const queue, const_action_func print);

uint8_t         is_queue_empty      (const queue_t * const queue);
size_t          get_queue_size      (const queue_t * const queue);

scl_error_t     change_queue_data   (void * const old_data, const void * const new_data, size_t data_size);
void*           queue_front         (const queue_t * const queue);
void*           queue_back          (const queue_t * const queue);
scl_error_t     queue_push          (queue_t * const queue, const void * const data, size_t data_size);
scl_error_t     queue_pop           (queue_t * const queue);

#endif /* QUEUE_UTILS_H_ */