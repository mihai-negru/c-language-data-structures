/**
 * @file doubleListUtils.h
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

#ifndef DOUBLE_LIST_UTILS_H_
#define DOUBLE_LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "functionTypes.h"

/**
 * @brief Double Linked List Node object definition
 * 
 */
typedef struct dlistNode {
    void *data;                 // Pointer to data
    struct dlistNode *prev;     // Pointer to previous node
    struct dlistNode *next;     // Pointer to next node
} TdlistNode;

/**
 * @brief Double Linked List object definition
 * 
 */
typedef struct {
    TdlistNode *head;                       // head of linked list
    TdlistNode *tail;                       // tail of linked list
    int (*compareData)(void *, void *);     // function to compare items
    void (*printData)(void *);              // function to print item
    void (*freeData)(void *);               // function to free item
    size_t size;                            // size of linked list
} dlinkedList;

dlinkedList* dcreateLinkedList(
    int (*compareData)(void *, void *),
    void (*printData)(void *),
    void (*freeData)(void *)
);

TdlistNode* dcreatedlistNode(
    void *data,
    size_t dataSize
);

void dlist_print_front(
    dlinkedList *list
);

void dlist_print_back(
    dlinkedList *list
);

void dlist_free_all(
    dlinkedList *list
);

int is_dlist_empty(
    dlinkedList *list
);

int get_dlist_size(
    dlinkedList *list
);

TdlistNode* get_dlist_head(
    dlinkedList *list
);

TdlistNode* get_dlist_tail(
    dlinkedList *list
);

void dlist_swap_data(
    dlinkedList *list,
    TdlistNode *firstNode,
    TdlistNode *secondNode
);

int dlist_change_data(
    dlinkedList *list,
    TdlistNode *baseNode,
    void *newData,
    size_t dataSize
);

int dlist_insert(
    dlinkedList *list,
    void *data,
    size_t dataSize
);

int dlist_insert_order(
    dlinkedList *list,
    void *data,
    size_t dataSize
);

int dlist_insert_front(
    dlinkedList *list,
    void *data,
    size_t dataSize
);

int dlist_insert_index(
    dlinkedList *list,
    void *data,
    size_t dataSize,
    size_t dataIndex
);

TdlistNode* dlist_find_index(
    dlinkedList *list,
    size_t dataIndex
);

TdlistNode* dlist_find_data(
    dlinkedList *list,
    void *data
);

int dlist_delete_data(
    dlinkedList *list,
    void *data
);

int dlist_delete_index(
    dlinkedList *list,
    size_t dataIndex
);

int dlist_erase(
    dlinkedList *list,
    size_t leftIndex,
    size_t rightIndex
);

dlinkedList* dlist_filter(
    dlinkedList *list,
    int (*filterFunction)(void *),
    size_t dataSize
);

void dlist_map(
    dlinkedList *list,
    void* (*mapFunction)(void *),
    size_t dataSize
);

#endif // DOUBLE_LIST_UTILS_H_