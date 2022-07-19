/**
 * @file scl_list.h
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
typedef struct list_node {
    void* data;                                         /* Pointer to data */
    struct list_node* next;                             /* Pointer to next node */
} list_node_t;

/**
 * @brief Linked List object definition
 * 
 */
typedef struct {
    list_node_t *head;                                  /* head of linked list */
    list_node_t *tail;                                  /* tail of linked list */
    int (*compare_data)(const void*, const void*);      /* function to compare items */
    void (*print_data)(const void*);                    /* function to print item */
    void (*free_data)(void*);                           /* function to free item */
    size_t size;                                        /* size of linked list */
} list_t;

list_t*         create_list         (int (*compare_data)(const void*, const void*), void (*print_data)(const void*), void (*free_data)(void *));
void            print_list          (list_t* list);
void            free_list           (list_t* list);

int             is_list_empty       (list_t* list);
int             get_list_size       (list_t* list);
list_node_t*    get_list_head       (list_t* list);
list_node_t*    get_list_tail       (list_t* list);

void            list_swap_data      (list_t* list, list_node_t* first_node, list_node_t* second_node);
int             list_change_data    (list_t* list, list_node_t* base_node, const void* new_data, size_t data_size);
int             list_insert         (list_t* list, const void* data, size_t data_size);
int             list_insert_order   (list_t* list, const void* data, size_t data_size);
int             list_insert_front   (list_t* list, const void* data, size_t data_size);
int             list_insert_index   (list_t* list, const void* data, size_t data_size, size_t data_index);

list_node_t*    list_find_index     (list_t* list, size_t data_index);
list_node_t*    list_find_data      (list_t* list, const void* data);

int             list_delete_data    (list_t* list, void *data);
int             list_delete_index   (list_t* list, size_t data_index);
int             list_erase          (list_t* list, size_t left_index, size_t right_index);

list_t*         list_filter         (list_t* list, int (*filter)(const void*), size_t data_size);
void            list_map            (list_t* list, void* (*mapFunction)(void*), size_t data_size);

#endif /* LIST_UTILS_H_ */