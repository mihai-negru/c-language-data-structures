/**
 * @file stackUtils.h
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
typedef struct stackNode {
    void *data;                 // Pointer to data
    struct stackNode *next;     // Pointer to next data node
} TStackNode;

/**
 * @brief stack object definition
 * 
 */
typedef struct {
    TStackNode *top;            // Pointer to top data node
    void (*freeData)(void *);   // Function to free one data
    size_t size;                // Size of the stack
} TStack;

TStack* create_stack(
    void (*freeData)(void *)
);

void free_stack(
    TStack *stack
);

void print_stack(
    TStack *stack,
    void (*printData)(void *)
);

int is_stack_empty(
    TStack *stack
);

int get_stack_size(
    TStack *stack
);

void* stack_top(
    TStack *stack
);

int stack_push(
    TStack *stack,
    void *data,
    size_t dataSize
);

int stack_pop(
    TStack *stack
);

#endif // STACK_UTILS_H_