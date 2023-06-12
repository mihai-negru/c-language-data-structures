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
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief stack Node object definition
 * 
 */
typedef struct stack_node_s {
    void *data;                     /* Pointer to data */
    struct stack_node_s *next;      /* Pointer to next data node */
} stack_node_t;

/**
 * @brief stack object definition
 * 
 */
typedef struct stack_s {
    stack_node_t *top;              /* Pointer to top data node */
    free_func frd;                  /* Function to free one data */
    size_t data_size;               /* Length in bytes of the data data type */
    size_t size;                    /* Size of the stack */
} sstack_t;

sstack_t*       create_stack        (free_func frd, size_t data_size);
scl_error_t     free_stack          (sstack_t * const __restrict__ stack);
scl_error_t     print_stack         (const sstack_t * const __restrict__ stack, action_func print);

uint8_t         is_stack_empty      (const sstack_t * const __restrict__ stack);
size_t          get_stack_size      (const sstack_t * const __restrict__ stack);

const void*     stack_top           (const sstack_t * const __restrict__ stack);
scl_error_t     stack_push          (sstack_t * const __restrict__ stack, const void * __restrict__ data);
scl_error_t     stack_pop           (sstack_t * const __restrict__ stack);

#endif /* STACK_UTILS_H_ */
