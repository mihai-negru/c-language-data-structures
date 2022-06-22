/**
 * @file listUtils.h
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

#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "functionTypes.h"

typedef struct listNode {
    void *data;
    struct listNode *next;
} TlistNode;

typedef struct {
    TlistNode *head;
    TlistNode *tail;
    int (*compareData)(void *, void *);
    void (*printData)(void *);
    void (*freeData)(void *);
    size_t size;
} linkedList;

linkedList* createLinkedList(
    int (*compareData)(void *, void *),
    void (*printData)(void *),
    void (*freeData)(void *)
);

TlistNode* createlistNode(
    void *data,
    size_t dataSize
);

void list_print(
    linkedList *list
);

void list_free_all(
    linkedList *list
);

int is_list_empty(
    linkedList *list
);

int get_list_size(
    linkedList *list
);

TlistNode* get_list_head(
    linkedList *list
);

TlistNode* get_list_tail(
    linkedList *list
);

void list_swap_data(
    linkedList *list,
    TlistNode *firstNode,
    TlistNode *secondNode
);

int list_change_data(
    linkedList *list,
    TlistNode *baseNode,
    void *newData,
    size_t dataSize
);

int list_insert(
    linkedList *list,
    void *data,
    size_t dataSize
);

int list_insert_order(
    linkedList *list,
    void *data,
    size_t dataSize
);

int list_insert_front(
    linkedList *list,
    void *data,
    size_t dataSize
);

int list_insert_index(
    linkedList *list,
    void *data,
    size_t dataSize,
    size_t dataIndex
);

TlistNode* list_find_index(
    linkedList *list,
    size_t dataIndex
);

TlistNode* list_find_data(
    linkedList *list,
    void *data
);

int list_delete_data(
    linkedList *list,
    void *data
);

int list_delete_index(
    linkedList *list,
    size_t dataIndex
);

int list_erase(
    linkedList *list,
    size_t leftIndex,
    size_t rightIndex
);

linkedList* list_filter(
    linkedList *list,
    int (*filterFunction)(void *),
    size_t dataSize
);

void list_map(
    linkedList *list,
    void* (*mapFunction)(void *),
    size_t dataSize
);

void list_sort(
    linkedList *list
);

#endif // LIST_UTILS_H_