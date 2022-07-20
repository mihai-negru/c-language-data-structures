/**
 * @file scl_dlist.h
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

#ifndef DOUBLE_LIST_UTILS_H_
#define DOUBLE_LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Double Linked List Node object definition
 * 
 */
typedef struct dlist_node {
    void* data;                                         /* Pointer to data */
    struct dlist_node* prev;                            /* Pointer to previous node */
    struct dlist_node* next;                            /* Pointer to next node */
} dlist_node_t;

/**
 * @brief Double Linked List object definition
 * 
 */
typedef struct {
    dlist_node_t* head;                                 /* head of linked list */
    dlist_node_t* tail;                                 /* tail of linked list */
    int (*compare_data)(const void*, const void*);      /* function to compare items */
    void (*print_data)(const void*);                    /* function to print item */
    void (*free_data)(void*);                           /* function to free item */
    size_t size;                                        /* size of linked list */
} dlist_t;

dlist_t*          create_dlist            (int (*compare_data)(const void*, const void*), void (*print_data)(const void*), void (*free_data)(void*));
void              print_front_dlist       (dlist_t* list);
void              print_back_dlist        (dlist_t* list);
void              free_dlist              (dlist_t* list);

int               is_dlist_empty          (dlist_t* list);
int               get_dlist_size          (dlist_t* list);
dlist_node_t*     get_dlist_head          (dlist_t* list);
dlist_node_t*     get_dlist_tail          (dlist_t* list);

void              dlist_swap_data         (dlist_t* list, dlist_node_t *first_node, dlist_node_t *second_node);
int               dlist_change_data       (dlist_t* list, const dlist_node_t* base_node, const void* new_data, size_t data_size);
int               dlist_insert            (dlist_t* list, const void* data, size_t data_size);
int               dlist_insert_order      (dlist_t* list, const void* data, size_t data_size);
int               dlist_insert_front      (dlist_t* list, const void* data, size_t data_size);
int               dlist_insert_index      (dlist_t* list, const void* data, size_t data_size,size_t data_index);

dlist_node_t*     dlist_find_index        (dlist_t* list, size_t data_index);
dlist_node_t*     dlist_find_data         (dlist_t* list, const void* data);

int               dlist_delete_data       (dlist_t* list, void* data);
int               dlist_delete_index      (dlist_t* list, size_t data_index);
int               dlist_erase             (dlist_t* list, size_t left_index, size_t right_index);

dlist_t*          dlist_filter            (dlist_t* list, int (*filter)(const void*),size_t data_size);
void              dlist_map               (dlist_t* list, const void* (*map)(void*), size_t data_size);

#endif /* DOUBLE_LIST_UTILS_H_ */