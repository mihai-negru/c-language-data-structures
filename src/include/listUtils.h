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
 * along with C-language-Data-Structures.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Linked List Node object definition
 * 
 */
typedef struct listNode {
    void *data;                 // Pointer to data
    struct listNode *next;      // Pointer to next node
} TlistNode;

/**
 * @brief Linked List object definition
 * 
 */
typedef struct {
    TlistNode *head;                        // head of linked list
    TlistNode *tail;                        // tail of linked list
    int (*compareData)(const void *, const void *);     // function to compare items
    void (*printData)(const void *);              // function to print item
    void (*freeData)(void *);               // function to free item
    size_t size;                            // size of linked list
} linkedList;

linkedList* createLinkedList(
    int (*compareData)(const void *, const void *),
    void (*printData)(const void *),
    void (*freeData)(void *)
);

TlistNode* createlistNode(
    const void *data,
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
    const void *newData,
    size_t dataSize
);

int list_insert(
    linkedList *list,
    const void *data,
    size_t dataSize
);

int list_insert_order(
    linkedList *list,
    const void *data,
    size_t dataSize
);

int list_insert_front(
    linkedList *list,
    const void *data,
    size_t dataSize
);

int list_insert_index(
    linkedList *list,
    const void *data,
    size_t dataSize,
    size_t dataIndex
);

TlistNode* list_find_index(
    linkedList *list,
    size_t dataIndex
);

TlistNode* list_find_data(
    linkedList *list,
    const void *data
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
    int (*filterFunction)(const void *),
    size_t dataSize
);

void list_map(
    linkedList *list,
    void* (*mapFunction)(void *),
    size_t dataSize
);

#endif // LIST_UTILS_H_