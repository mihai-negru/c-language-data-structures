/**
 * @file queueUtils.c
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

#include "./include/queueUtils.h"

TQueue* create_queue(void (*freeData)(void *)) {
    (void)freeData;
    return NULL;
}

static TQueueNode* create_queue_node(void *data, size_t dataSize) {
    (void)data;
    (void)dataSize;
    return NULL;
}

void free_queue(TQueue *queue) {
    (void)queue;
}

void print_queue(TQueue *queue, void (*printData)(void *)) {
    (void)queue;
    (void)printData;
}

int is_queue_empty(TQueue *queue) {
    (void)queue;
    return 0;
}

int get_queue_size(TQueue *queue) {
    (void)queue;
    return 0;
}

void* queue_front(TQueue *queue) {
    (void)queue;
    return NULL;
}

void* queue_back(TQueue *queue) {
    (void)queue;
    return NULL;
}

int queue_push(TQueue *queue, void *data, size_t dataSize) {
    (void)queue;
    (void)data;
    (void)dataSize;

    TQueueNode *newNode = create_queue_node(data, dataSize);
    (void)newNode;
    return 0;
}

int queue_pop(TQueue *queue) {
    (void)queue;
    return 0;
}