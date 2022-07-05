/**
 * @file queueUtils.c
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

#include "./include/queueUtils.h"

/**
 * @brief Create a queue object. Allocation may fail
 * if there is not enough memory on heap, in this case
 * an exception will be thrown
 * 
 * @param freeData pointer to a function to free content of one data
 * @return TQueue* a new allocated queue object or NULL (if function failed)
 */
TQueue* create_queue(void (*freeData)(void *)) {
    // Allocate a new queue on the heap
    TQueue *newQueue = (TQueue *)malloc(sizeof(TQueue));

    // Check if queue allocation went right
    if (newQueue) {
        // Set function pointers
        newQueue->freeData = freeData;

        // Set front, back and size of an empty queue
        newQueue->front = newQueue->back = NULL;
        newQueue->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for queue allocation");
    }

    // Return a new allocated queue or NULL
    return newQueue;
}

/**
 * @brief Create a queue node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough
 * memory is left on heap, in this case function will return NULL
 * and an exception will be thrown
 * 
 * @param data pointer to an address of a generic data
 * @param dataSize size of one generic data
 * @return TQueueNode* new allocated queue node object or NULL
 */
static TQueueNode* create_queue_node(void *data, size_t dataSize) {
    // Check if data address is valid
    if (data == NULL)
        return NULL;

    // Allocate a new node on the heap
    TQueueNode *newNode = (TQueueNode *)malloc(sizeof(TQueueNode));

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
            perror("Not enough memory for node queue allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    // Return a new queue node object or NULL
    return newNode;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * queue object. The function will iterate through all nodes and will
 * free the data content according to freeData function provided by user at
 * creation of queue, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param queue an allocated queue object
 */
void free_queue(TQueue *queue) {
    // Check if queue needs to be freed
    if (queue) {
        // Iterate through every node from queue
        while (queue->front != NULL) {
            TQueueNode *iterator = queue->front;

            // Update new front of the queue
            queue->front = queue->front->next;

            // Free content of data if necessary
            if (queue->freeData != NULL && iterator->data != NULL)
                queue->freeData(iterator->data);

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

        // Free queue object
        free(queue);
    }
}

/**
 * @brief Function prints all elements within the
 * given queue according to printData function. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param queue a queue object
 * @param printData a pointer to a function to print content of data pointer
 */
void print_queue(TQueue *queue, void (*printData)(void *)) {
    // Check is queue is allocated
    if (queue) {
        // Queue is empty, print []
        if (queue->front == NULL)
            printf("[ ]");

        TQueueNode *iterator = queue->front;

        // Print every node according
        // to printData function
        while (iterator != NULL) {
            printData(iterator->data);
            iterator = iterator->next;
        }
    }
}

/**
 * @brief Function to check if a queue object
 * is empty or not. The function tests if front of queue
 * is NULL in that case function will return true, otherwise
 * it will return false. A NULL queue is also considered as an
 * empty queue
 * 
 * @param stack a stack obecjt
 * @return int 1(True) if queue is not allocated or empty and 0(False) otherwise
 */
int is_queue_empty(TQueue *queue) {
    if (queue == NULL || queue->front == NULL)
        return 1;
    
    return 0;
}

/**
 * @brief Get the queue size object. If queue is not
 * allocated then function will return -1 value.
 * 
 * @param queue a queue object
 * @return int -1 if queue is not allocated or
 * queue size
 */
int get_queue_size(TQueue *queue) {
    if (queue == NULL)
        return -1;

    return queue->size;
}

/**
 * @brief Function to return the real data of the
 * front node. If queue is not allocated or queue is empty
 * then NULL will be returned, otherwise a pointer to the actual
 * data memory location will be returned
 * 
 * @param queue a queue object
 * @return void* a pointer to front element data
 */
void* queue_front(TQueue *queue) {
    if (queue == NULL || queue->front == NULL)
        return NULL;

    return queue->front->data;
}

/**
 * @brief Function to return the real data of the
 * back node. If queue is not allocated or queue is empty
 * then NULL will be returned, otherwise a pointer to the actual
 * data memory location will be returned
 * 
 * @param queue a queue object
 * @return void* a pointer to front element data
 */
void* queue_back(TQueue *queue) {
    if (queue == NULL || queue->back == NULL)
        return NULL;

    return queue->back->data;
}

/**
 * @brief Function to push one generic data to a queue.
 * Function may fail if queue or data is not valid (have
 * address NULL) or not enough heap memory is left. You can
 * push different data types into queue, but you will have to define
 * diferent functions to print queue and to maintain it
 * 
 * @param queue a queue object
 * @param data pointer to an address of a generic data type
 * @param dataSize size of a generic data type element
 * @return int 1(Fail) if function failed or 0(Success) if
 * pushing on the queue went successfully
 */
int queue_push(TQueue *queue, void *data, size_t dataSize) {
    // Check if queue and data addresses are valid
    if (queue == NULL || data == NULL)
        return 1;

    // Create a new queue node
    TQueueNode *newNode = create_queue_node(data, dataSize);

    // Check if new node was allocated
    if (newNode == NULL)
        return 1;

    // Insert node into queue
    if (queue->front == NULL) {
        // Queue is empty
        queue->front = newNode;
        queue->back = newNode;
    } else {
        // Update back links
        queue->back->next = newNode;
        queue->back = newNode;
    }

    // Increase queue size
    ++(queue->size);

    // Pushing went successfully
    return 0;
}

/**
 * @brief Function to pop one genric data from a queue.
 * Function may fail if queue or data is not valid (have
 * address NULL). Function will remove the front element from the queue
 * and will clear the content of the data accroding to freeData
 * function provided by the user at creation of the stack.
 * 
 * @param queue a queue object
 * @return int 1(Fail) if function failed or 0(Success) if
 * popping on the queue went successfully
 */
int queue_pop(TQueue *queue) {
    // Check if queue is allocated and it is not empty
    if (queue == NULL || queue->front == NULL || queue->size == 0)
        return 1;

    // Pointer to current wipe node
    TQueueNode *delete_node = queue->front;

    // Update the front pointer of the queue
    queue->front = queue->front->next;

    // Decrease queue size
    --(queue->size);

    // Free content of the data if necessary
    if (queue->freeData != NULL && delete_node->data != NULL)
        queue->freeData(delete_node->data);

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