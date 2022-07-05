/**
 * @file queueUtils.h
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
typedef struct queueNode {
    void *data;                 // Pointer to data
    struct queueNode *next;     // Pointer to next data node
} TQueueNode;

/**
 * @brief queue object definition
 * 
 */
typedef struct {
    TQueueNode *front;          // Pointer to beginning of queue
    TQueueNode *back;           // Pointer to end of queue
    void (*freeData)(void *);   // Function to free one data
    size_t size;                // Size of the queue
} TQueue;

TQueue* create_queue(
    void (*freeData)(void *)
);

void free_queue(
    TQueue *queue
);

void print_queue(
    TQueue *queue,
    void (*printData)(void *)
);

int is_queue_empty(
    TQueue *queue
);

int get_queue_size(
    TQueue *queue
);

void* queue_front(
    TQueue *queue
);

void* queue_back(
    TQueue *queue
);

int queue_push(
    TQueue *queue,
    void *data,
    size_t dataSize
);

int queue_pop(
    TQueue *queue
);

#endif // QUEUE_UTILS_H_