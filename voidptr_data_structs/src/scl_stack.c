/**
 * @file scl_stack.c
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

#include "./include/scl_stack.h"

/**
 * @brief Create a stack object. Allocation may fail
 * if there is not enough memory on heap, in this case
 * an exception will be thrown
 * 
 * @param frd pointer to a function to free content of one data
 * @param data_size length in bytes of the data data type
 * @return sstack_t* a new allocated stack object or `NULL` (if function failed)
 */
sstack_t* create_stack(free_func frd, size_t data_size) {
    if (0 == data_size) {
        return NULL;
    }

    /* Allocate a new stack on the heap */
    sstack_t *new_stack = malloc(sizeof(*new_stack));

    /* Check if stack allocation went right */
    if (NULL != new_stack) {

        /* Set function pointers */
        new_stack->frd = frd;

        /* Set top and size of an empty stack */
        new_stack->top = NULL;
        new_stack->data_size = data_size;
        new_stack->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for stack allocation");
    }

    /* Return a new allocated stack or `NULL` */
    return new_stack;
}

/**
 * @brief Create a stack node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough
 * memory is left on heap, in this case function will return `NULL`
 * and an exception will be thrown
 * 
 * @param stack an allocated stack object
 * @param data pointer to an address of a generic data
 * @return stack_node_t* new allocated stack node object or `NULL`
 */
static stack_node_t* create_stack_node(const sstack_t * const __restrict__ stack, const void * __restrict__ data) {
    /* Check if data address is valid */
    if (NULL == data) {
        return NULL;
    }

    /* Allocate a new node on the heap */
    stack_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if node allocation went successfully */
    if (NULL != new_node) {

        /* Set default next pointer */
        new_node->next = NULL;

        /* Allocate heap memory for data */
        new_node->data = malloc(stack->data_size);

        /* Check if memory allocation went right */
        if (NULL != new_node->data) {

            /* Copy all bytes from data address to new node's data */
            memcpy(new_node->data, data, stack->data_size);
        } else {
            free(new_node);
            new_node = NULL;

            errno = ENOMEM;
            perror("Not enough memory for data node stack allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for stack node allocation");
    }

    /* Return a new stack node object or `NULL` */
    return new_node;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * stack object. The function will iterate through all nodes and will
 * free the data content according to frd function provided by user at
 * creation of stack, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param stack an allocated stack object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_stack(sstack_t * const __restrict__ stack) {
    /* Check if stack needs to be freed */
    if (NULL != stack) {

        /* Iterate through every node from stack */
        while (NULL != stack->top) {
            stack_node_t *iterator = stack->top;

            /* Update new top of the stack */
            stack->top = stack->top->next;

            /* Free content of data if necessary */
            if ((NULL != stack->frd) && (NULL != iterator->data)) {
                stack->frd(iterator->data);
            }

            /* Free node pointer to data */
            if (NULL != iterator->data) {
                free(iterator->data);
            }

            /* Set node pointer to data as `NULL` */
            iterator->data = NULL;

            /* Free node pointer */
            if (NULL != iterator) {
                free(iterator);
            }

            /* Set node pointer as `NULL` */
            iterator = NULL;
        }

        /* Free stack object */
        free(stack);

        return SCL_OK;
    }

    return SCL_NULL_STACK;
}

/**
 * @brief Function prints all elements within the
 * given stack according to printData function. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param stack a stack object
 * @param print a pointer to a function to print content of data pointer
 * @return scl_error_t enum object for handling errors
 */
scl_error_t print_stack(const sstack_t * const __restrict__ stack, action_func print) {
    /* Check if input data is valid */
    if (NULL == stack) {
        return SCL_NULL_STACK;
    }

    if (NULL == print) {
        return SCL_NULL_ACTION_FUNC;
    } 

    /* Stack is empty, print [] */
    if (NULL == stack->top) {
        printf("[ ]");
    } else {
        const stack_node_t *iterator = stack->top;

        /*
         * Print every node according
         * to print function
         */
        while (NULL != iterator) {
            print(iterator->data);
            iterator = iterator->next;
        }
    }
    
    return SCL_OK;
}

/**
 * @brief Function to check if a stack object
 * is empty or not. The function tests if top of stack
 * is `NULL` in that case function will return true, otherwise
 * it will return false. A `NULL` stack is also considered as an
 * empty stack
 * 
 * @param stack a stack object
 * @return uint8_t 1(True) if stack is not allocated or empty and 0(False) otherwise
 */
uint8_t is_stack_empty(const sstack_t * const __restrict__ stack) {
    if ((NULL == stack) || (NULL == stack->top)) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief Get the stack size object. If stack is not
 * allocated then function will return SIZE_MAX value.
 * 
 * @param stack a stack object
 * @return size_t SIZE_MAX if stack is not allocated or
 * stack size
 */
size_t get_stack_size(const sstack_t * const __restrict__ stack) {
    if (NULL == stack) {
        return SIZE_MAX;
    }

    return stack->size;
}

/**
 * @brief Function to return a pointer to data of the
 * top node. If stack is not allocated or stack is empty
 * then `NULL` will be returned, otherwise a pointer to the actual
 * data memory location will be returned
 * 
 * @param stack a stack object
 * @return const void* a pointer to top element data
 */
const void* stack_top(const sstack_t * const __restrict__ stack) {
    if ((NULL == stack) || (NULL == stack->top)) {
        return NULL;
    }

    return stack->top->data;
}

/**
 * @brief Function to push one generic data to a stack.
 * Function may fail if stack or data is not valid (have
 * address `NULL`) or not enough heap memory is left.
 * 
 * @param stack a stack object
 * @param data pointer to an address of a generic data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t stack_push(sstack_t * const __restrict__ stack, const void * __restrict__ data) {
    /* Check if stack and data addresses are valid */
    if (NULL == stack) {
        return SCL_NULL_STACK;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Create a new stack node */
    stack_node_t *new_node = create_stack_node(stack, data);

    /* Check if new node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Insert node into stack */
    if (NULL == stack->top) {
        
        /* Stack is empty */
        stack->top = new_node;
    } else {

        /* Update top links */
        new_node->next = stack->top;
        stack->top = new_node;
    }

    /* Increase stack size */
    ++(stack->size);

    /* Pushing went successfully */
    return SCL_OK;
}

/**
 * @brief Function to pop one genric data from a stack.
 * Function may fail if stack or data is not valid (have
 * address `NULL`). Function will remove the top element from the stack
 * and will clear the content of the data accroding to frd
 * function provided by the user at creation of the stack.
 * 
 * @param stack a stack object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t stack_pop(sstack_t * const __restrict__ stack) {
    /* Check if stack is allocated and it is not empty */
    if (NULL == stack) {
        return SCL_NULL_STACK;
    }

    if ((NULL == stack->top) || (0 == stack->size)) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    /* Pointer to current wipe node */
    stack_node_t *delete_node = stack->top;

    /* Update the top pointer of the stack */
    stack->top = stack->top->next;

    /* Decrease stack size */
    --(stack->size);

    /* Free content of the data if necessary */
    if ((NULL != stack->frd) && (NULL != delete_node->data)) {
        stack->frd(delete_node->data);
    }

    /* Free pointer to data memory location */
    if (NULL != delete_node->data) {
        free(delete_node->data);
    }

    /* Set data pointer to default value */
    delete_node->data = NULL;

    /* Free node memory */
    if (NULL != delete_node) {
        free(delete_node);
    }

    /* Set node poiner to default value */
    delete_node = NULL;

    /* Popping went successfully */
    return SCL_OK;
}
