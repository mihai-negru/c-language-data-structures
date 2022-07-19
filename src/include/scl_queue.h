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
#include <errno.h>

/**
 * @brief queue Node object definition
 * 
 */
typedef struct queue_node {
    void* data;                     /* Pointer to data */
    struct queue_node* next;        /* Pointer to next data node */
} queue_node_t;

/**
 * @brief queue object definition
 * 
 */
typedef struct {
    queue_node_t* front;            /* Pointer to beginning of queue */
    queue_node_t* back;             /* Pointer to end of queue */
    void (*free_data)(void*);       /* Function to free one data */
    size_t size;                    /* Size of the queue */
} queue_t;

queue_t*    create_queue        (void (*free_data)(void*));
void        free_queue          (queue_t* queue);
void        print_queue         (queue_t* queue, void (*print_data)(const void*));

int         is_queue_empty      (queue_t* queue);
int         get_queue_size      (queue_t* queue);

void*       queue_front         (queue_t* queue);
void*       queue_back          (queue_t* queue);
int         queue_push          (queue_t* queue, const void* data, size_t data_size);
int         queue_pop           (queue_t* queue);

#endif /* QUEUE_UTILS_H_ */