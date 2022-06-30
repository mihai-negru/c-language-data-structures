/**
 * @file stackUtils.c
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

#include "./include/stackUtils.h"

/**
 * @brief Create a stack object. Allocation may fail
 * if there is not enought memory on heap, in this case
 * an exception will be thrown
 * 
 * @param freeData pointer to a function to free content of one data
 * @return TStack* a new allocated stack object or NULL (if function failed)
 */
TStack* create_stack(void (*freeData)(void *)) {
    // Allocate a new stack on the heap
    TStack *newStack = (TStack *)malloc(sizeof(TStack));

    // Check if stack allocation went right
    if (newStack) {
        // Set function pointers
        newStack->freeData = freeData;

        // Set top and size of an empty stack
        newStack->top = NULL;
        newStack->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for stack allocation");
    }

    // Return a new allocated stack or NULL
    return newStack;
}

/**
 * @brief Create a stack node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough
 * memory is left on heap, in this case function will return NULL
 * and an exception will be thrown
 * 
 * @param data pointer to an address of a generic data
 * @param dataSize size of one generic data
 * @return TStackNode* new allocated stack node object or NULL
 */
TStackNode* create_stack_node(void *data, size_t dataSize) {
    // Check if data address is valid
    if (data == NULL)
        return NULL;

    // Allocate a new node on the heap
    TStackNode *newNode = (TStackNode *)malloc(sizeof(TStackNode));

    // Check if node allocation went successfully
    if (newNode) {
        // Set default next pointer
        newNode->next = NULL;

        // Allocate heap memory for data
        newNode->data = malloc(dataSize);

        // Check if memory allocation went right
        if (newNode->data)
            // Copy all bytes from data address to new node's data
            memcpy(newNode->data, data, dataSize);
        else {
            errno = ENOMEM;
            perror("Not enough memory for node stack allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    // Return a new stack node object or NULL
    return newNode;
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
void free_stack(TStack *stack) {
    // Check if stack needs to be freed
    if (stack) {
        // Iterate through every node from stack
        while (stack->top != NULL) {
            TStackNode *iterator = stack->top;

            // Update new top of the stack
            stack->top = stack->top->next;

            // Free content of data if necessary
            if (stack->freeData != NULL && iterator->data != NULL)
                stack->freeData(iterator->data);

            // Free node pointer to data
            if (iterator->data != NULL)
                free(iterator->data);

            // Set node pointer to data as NULL
            iterator->data = NULL;

            // Free node pointer
            if (iterator != NULL)
                free(iterator);

            // Set node pointer as NULL
            iterator = NULL;
        }

        // Free stack object
        free(stack);
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
 * @param printData a pointer to a function to print content of data pointer
 */
void print_stack(TStack *stack, void (*printData)(void *)) {
    // Check is stack is allocated
    if (stack) {
        // Stack is empty, print []
        if (stack->top == NULL)
            printf("[ ]");

        TStackNode *iterator = stack->top;

        // Print every node accroding
        // to printData function
        while (iterator != NULL) {
            printData(iterator->data);
            iterator = iterator->next;
        }
    }
}

/**
 * @brief Function to check if a stack object
 * is empty or not. The function tests if top of list
 * is NULL in that case function will return true, otherwise
 * it will return false. A NULL list is also considered as an
 * empty list
 * 
 * @param stack a stack obecjt
 * @return int 1(True) if stack is not allocated or empty and 0(False) otherwise
 */
int is_stack_empty(TStack *stack) {
    if (stack == NULL || stack->top == NULL)
        return 1;
    
    return 0;
}

/**
 * @brief Get the stack size object. If stack is not
 * allocated then function will return -1 value.
 * 
 * @param stack a stack object
 * @return int -1 if stack is not allocated or
 * list size
 */
int get_stack_size(TStack *stack) {
    if (stack == NULL)
        return -1;

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
void* stack_top(TStack *stack) {
    if (stack == NULL || stack->top == NULL)
        return NULL;

    return stack->top->data;
}

/**
 * @brief Function to push one generic data to a stack.
 * Function may fail if stack or data is not valid (have
 * address NULL) or not enough heap memory is left. You can
 * push different data types into stack, but you will have to difine
 * diferent functions to print stack or to maintain it
 * 
 * @param stack a stack object
 * @param data pointer to an address of a generic data type
 * @param dataSize size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * pushing on the stack went successfully
 */
int stack_push(TStack *stack, void *data, size_t dataSize) {
    // Check if stack and data addresses are valid
    if (stack == NULL || data == NULL)
        return 1;

    // Create a new stack node
    TStackNode *newNode = create_stack_node(data, dataSize);

    // Check if new node was allocated
    if (newNode == NULL)
        return 1;

    // Insert node into stack
    if (stack->top == NULL) {
        // Stack is empty
        stack->top = newNode;
    } else {
        // Update top links
        newNode->next = stack->top;
        stack->top = newNode;
    }

    // Increase stack size
    ++(stack->size);

    // Pushing went successfully
    return 0;
}

/**
 * @brief Function to pop one genric data from a stack.
 * Function may fail if stack or data is not valid (have
 * address NULL). Function will remove element from the stack
 * and will clear the content of the data accroding to freeData
 * function provided by the user at creation of the stack.
 * 
 * @param stack a stack object
 * @return int 1(Fail) if function failed or 0(Success) if
 * popping on the stack went successfully
 */
int stack_pop(TStack *stack) {
    // Check if stack is allocated and it is not empty
    if (stack == NULL || stack->top == NULL || stack->size == 0)
        return 1;

    // Pointer to current wipe node
    TStackNode *delete_node = stack->top;

    // Update the top pointer of the stack
    stack->top = stack->top->next;

    // Decrease stack size
    --(stack->size);

    // Free content of the data if necessary
    if (stack->freeData != NULL && delete_node->data != NULL)
        stack->freeData(delete_node->data);

    // Free pointer to data memory location
    if (delete_node->data != NULL)
        free(delete_node->data);

    // Set data pointer to default value
    delete_node->data = NULL;

    // Free node memory
    if (delete_node != NULL)
        free(delete_node);

    // Set node poiner to default value
    delete_node = NULL;

    // Popping went successfully
    return 0;
}