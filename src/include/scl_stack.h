/**
 * @file scl_stack.h
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

#ifndef STACK_UTILS_H_
#define STACK_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief stack Node object definition
 * 
 */
typedef struct stack_node {
    void* data;                     /* Pointer to data */
    struct stack_node* next;        /* Pointer to next data node */
} stack_node_t;

/**
 * @brief stack object definition
 * 
 */
typedef struct {
    stack_node_t* top;              /* Pointer to top data node */
    void (*free_data)(void*);       /* Function to free one data */
    size_t size;                    /* Size of the stack */
} stack_t;

stack_t*    create_stack        (void (*free_data)(void*));
void        free_stack          (stack_t* stack);
void        print_stack         (stack_t* stack, void (*print_data)(const void*));

int         is_stack_empty      (stack_t* stack);
int         get_stack_size      (stack_t* stack);

void*       stack_top           (stack_t* stack);
int         stack_push          (stack_t* stack, const void* data, size_t data_size);
int         stack_pop           (stack_t* stack);

#endif /* STACK_UTILS_H_ */