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
 * @param free_data pointer to a function to free content of one data
 * @return stack_t* a new allocated stack object or NULL (if function failed)
 */
stack_t* create_stack(void (*free_data)(void*)) {
    /* Allocate a new stack on the heap */
    stack_t* new_stack = malloc(sizeof(*new_stack));

    /* Check if stack allocation went right */
    if (NULL != new_stack) {

        /* Set function pointers */
        new_stack->free_data = free_data;

        /* Set top and size of an empty stack */
        new_stack->top = NULL;
        new_stack->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for stack allocation");
    }

    /* Return a new allocated stack or NULL */
    return new_stack;
}

/**
 * @brief Create a stack node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough
 * memory is left on heap, in this case function will return NULL
 * and an exception will be thrown
 * 
 * @param data pointer to an address of a generic data
 * @param data_size size of one generic data
 * @return stack_node_t* new allocated stack node object or NULL
 */
static stack_node_t* create_stack_node(const void* data, size_t data_size) {
    /* Check if data address is valid */
    if (NULL == data) {
        return NULL;
    }

    /* Allocate a new node on the heap */
    stack_node_t* new_node = malloc(sizeof(*new_node));

    /* Check if node allocation went successfully */
    if (NULL != new_node) {

        /* Set default next pointer */
        new_node->next = NULL;

        /* Allocate heap memory for data */
        new_node->data = malloc(data_size);

        /* Check if memory allocation went right */
        if (NULL != new_node->data) {

            /* Copy all bytes from data address to new node's data */
            memcpy(new_node->data, data, data_size);
        } else {
            errno = ENOMEM;
            perror("Not enough memory for node stack allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    /* Return a new stack node object or NULL */
    return new_node;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * stack object. The function will iterate through all nodes and will
 * free the data content according to freeData function provided by user at
 * creation of stack, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param stack an allocated stack object
 */
void free_stack(stack_t* stack) {
    /* Check if stack needs to be freed */
    if (NULL != stack) {

        /* Iterate through every node from stack */
        while (NULL != stack->top) {
            stack_node_t* iterator = stack->top;

            /* Update new top of the stack */
            stack->top = stack->top->next;

            /* Free content of data if necessary */
            if ((NULL != stack->free_data) && (NULL != iterator->data)) {
                stack->free_data(iterator->data);
            }

            /* Free node pointer to data */
            if (NULL != iterator->data) {
                free(iterator->data);
            }

            /* Set node pointer to data as NULL */
            iterator->data = NULL;

            /* Free node pointer */
            if (NULL != iterator) {
                free(iterator);
            }

            /* Set node pointer as NULL */
            iterator = NULL;
        }

        /* Free stack object */
        free(stack);

        /* Set stack pointer as NULL */
        stack = NULL;
    }
}

/**
 * @brief Function prints all elements within the
 * given stack according to printData function. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param stack a stack object
 * @param print_data a pointer to a function to print content of data pointer
 */
void print_stack(stack_t* stack, void (*print_data)(const void*)) {
    /* Check is stack is allocated */
    if (NULL != stack) {

        /* Stack is empty, print [] */
        if (NULL == stack->top) {
            printf("[ ]");
        }

        stack_node_t* iterator = stack->top;

        /*
         * Print every node according
         * to printData function
         */
        while (NULL != iterator) {
            print_data(iterator->data);
            iterator = iterator->next;
        }
    }
}

/**
 * @brief Function to check if a stack object
 * is empty or not. The function tests if top of stack
 * is NULL in that case function will return true, otherwise
 * it will return false. A NULL stack is also considered as an
 * empty stack
 * 
 * @param stack a stack obecjt
 * @return int 1(True) if stack is not allocated or empty and 0(False) otherwise
 */
int is_stack_empty(stack_t* stack) {
    if ((NULL == stack) || (NULL == stack->top)) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief Get the stack size object. If stack is not
 * allocated then function will return -1 value.
 * 
 * @param stack a stack object
 * @return int -1 if stack is not allocated or
 * stack size
 */
int get_stack_size(stack_t* stack) {
    if (NULL == stack) {
        return -1;
    }

    return stack->size;
}

/**
 * @brief Function to return the real data of the
 * top node. If stack is not allocated or stack is empty
 * then NULL will be returned, otherwise a pointer to the actual
 * data memory location will be returned
 * 
 * @param stack a stack object
 * @return void* a pointer to top element data
 */
void* stack_top(stack_t* stack) {
    if ((NULL == stack) || (NULL == stack->top)) {
        return NULL;
    }

    return stack->top->data;
}

/**
 * @brief Function to push one generic data to a stack.
 * Function may fail if stack or data is not valid (have
 * address NULL) or not enough heap memory is left. You can
 * push different data types into stack, but you will have to define
 * diferent functions to print stack and to maintain it
 * 
 * @param stack a stack object
 * @param data pointer to an address of a generic data type
 * @param data_size size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * pushing on the stack went successfully
 */
int stack_push(stack_t* stack, const void* data, size_t data_size) {
    /* Check if stack and data addresses are valid */
    if ((NULL == stack) || (NULL == data)) {
        return 1;
    }

    /* Create a new stack node */
    stack_node_t* new_node = create_stack_node(data, data_size);

    /* Check if new node was allocated */
    if (NULL == new_node) {
        return 1;
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
    return 0;
}

/**
 * @brief Function to pop one genric data from a stack.
 * Function may fail if stack or data is not valid (have
 * address NULL). Function will remove the top element from the stack
 * and will clear the content of the data accroding to freeData
 * function provided by the user at creation of the stack.
 * 
 * @param stack a stack object
 * @return int 1(Fail) if function failed or 0(Success) if
 * popping on the stack went successfully
 */
int stack_pop(stack_t* stack) {
    /* Check if stack is allocated and it is not empty */
    if ((NULL == stack) || (NULL == stack->top) || (0 == stack->size))
        return 1;

    /* Pointer to current wipe node */
    stack_node_t* delete_node = stack->top;

    /* Update the top pointer of the stack */
    stack->top = stack->top->next;

    /* Decrease stack size */
    --(stack->size);

    /* Free content of the data if necessary */
    if ((NULL != stack->free_data) && (NULL != delete_node->data)) {
        stack->free_data(delete_node->data);
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
    return 0;
}