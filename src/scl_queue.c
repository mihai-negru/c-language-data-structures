/**
 * @file scl_queue.c
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

#include "./include/scl_queue.h"

/**
 * @brief Create a queue object. Allocation may fail
 * if there is not enough memory on heap, in this case
 * an exception will be thrown
 * 
 * @param frd pointer to a function to free content of one data
 * @return queue_t* a new allocated queue object or NULL (if function failed)
 */
queue_t* create_queue(free_func frd) {
    /* Allocate a new queue on the heap */
    queue_t *new_queue = malloc(sizeof(*new_queue));

    /* Check if queue allocation went right */
    if (NULL != new_queue) {

        /* Set function pointers */
        new_queue->frd = frd;

        /* Set front, back and size of an empty queue */
        new_queue->front = new_queue->back = NULL;
        new_queue->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for queue allocation");
    }

    /* Return a new allocated queue or NULL */
    return new_queue;
}

/**
 * @brief Create a queue node object. Allocation of a new node
 * may fail if address of data is not valid or if not enough
 * memory is left on heap, in this case function will return NULL
 * and an exception will be thrown
 * 
 * @param data pointer to an address of a generic data
 * @param data_size size of one generic data
 * @return queue_node_t* new allocated queue node object or NULL
 */
static queue_node_t* create_queue_node(const void * const data, size_t data_size) {
    /* Check if data address is valid */
    if (NULL == data) {
        return NULL;
    }

    /* Allocate a new node on the heap */
    queue_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if node allocation went successfully */
    if (NULL != new_node) {

        /* Set default next pointer */
        new_node->next = NULL;

        /* Allocate heap memory for data */
        new_node->data = malloc(data_size);

        /* Check if memory allocation went right */
        if (new_node->data) {

            /* Copy all bytes from data address to new node's data */
            memcpy((uint8_t *)new_node->data, (const uint8_t * const)data, data_size);
        } else {
            free(new_node);
            new_node = NULL;

            errno = ENOMEM;
            perror("Not enough memory for node queue allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node queue allocation");
    }

    /* Return a new queue node object or NULL */
    return new_node;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * queue object. The function will iterate through all nodes and will
 * free the data content according to freeData function provided by user at
 * creation of queue, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param queue an allocated queue object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_queue(queue_t * const queue) {
    /* Check if queue needs to be freed */
    if (NULL != queue) {

        /* Iterate through every node from queue */
        while (NULL != queue->front) {
            queue_node_t *iterator = queue->front;

            /* Update new front of the queue */
            queue->front = queue->front->next;

            /* Free content of data if necessary */
            if ((NULL != queue->frd) && (NULL != iterator->data)) {
                queue->frd(iterator->data);
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

        /* Free queue object */
        free(queue);

        return SCL_OK;
    }

    return SCL_NULL_QUEUE;
}

/**
 * @brief Function prints all elements within the
 * given queue according to printData function. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param queue a queue object
 * @param print a pointer to a function to print content of data pointer
 * @return scl_error_t enum object for handling errors
 */
scl_error_t print_queue(const queue_t * const queue, action_func print) {
    /* Check if input datat is valid */
    if (NULL == queue) {
        return SCL_NULL_QUEUE;
    }

    if (NULL == print) {
        return SCL_NULL_ACTION_FUNC;
    } 

    /* Queue is empty, print [] */
    if (NULL == queue->front) {
        printf("[ ]");
    } else {
        const queue_node_t *iterator = queue->front;

        /*
         * Print every node according
         * to printData function
         */
        while (NULL != iterator) {
            print(iterator->data);
            iterator = iterator->next;
        }
    }
    
    return SCL_OK;
}

/**
 * @brief Function to check if a queue object
 * is empty or not. The function tests if front of queue
 * is NULL in that case function will return true, otherwise
 * it will return false. A NULL queue is also considered as an
 * empty queue
 * 
 * @param queue queue object
 * @return uint8_t 1(True) if queue is not allocated or empty and 0(False) otherwise
 */
uint8_t is_queue_empty(const queue_t * const queue) {
    if ((NULL == queue) || (NULL == queue->front)) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief Get the queue size object. If queue is not
 * allocated then function will return -1 value.
 * 
 * @param queue a queue object
 * @return size_t SIZE_MAX if queue is not allocated or
 * queue size
 */
size_t get_queue_size(const queue_t * const queue) {
    if (NULL == queue) {
        return SIZE_MAX;
    }

    return queue->size;
}

/**
 * @brief Function to change the data of a queue node.
 * 
 * @param old_data pointer to the old data type
 * @param new_data pointer to the new data type
 * @param data_size size of the data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t change_queue_data(void * const old_data, const void * const new_data, size_t data_size) {
    /* Check if input data is valid */
    if ((NULL == old_data) || (NULL == new_data) || (0 == data_size)) {
        return SCL_CANNOT_CHANGE_DATA;
    }

    /* Copy bytes from new data to old data */
    memcpy((uint8_t * const)old_data, (const uint8_t * const)new_data, data_size);

    /* Return success */
    return SCL_OK;
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
void* queue_front(const queue_t * const queue) {
    if ((NULL == queue) || (NULL == queue->front)) {
        return NULL;
    }

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
void* queue_back(const queue_t * const queue) {
    if ((NULL == queue) || (NULL == queue->back)) {
        return NULL;
    }

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
 * @return scl_error_t enum object for handling errors
 */
scl_error_t queue_push(queue_t * const queue, const void * const data, size_t data_size) {
    /* Check if queue and data addresses are valid */
    if (NULL == queue) {
        return SCL_NULL_QUEUE;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Create a new queue node */
    queue_node_t *new_node = create_queue_node(data, data_size);

    /* Check if new node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Insert node into queue */
    if (NULL == queue->front) {

        /* Queue is empty */
        queue->front = new_node;
        queue->back = new_node;
    } else {

        /* Update back links */
        queue->back->next = new_node;
        queue->back = new_node;
    }

    /* Increase queue size */
    ++(queue->size);

    /* Pushing went successfully */
    return SCL_OK;
}

/**
 * @brief Function to pop one genric data from a queue.
 * Function may fail if queue or data is not valid (have
 * address NULL). Function will remove the front element from the queue
 * and will clear the content of the data accroding to freeData
 * function provided by the user at creation of the stack.
 * 
 * @param queue a queue object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t queue_pop(queue_t * const queue) {
    /* Check if queue is allocated and it is not empty */
    if (NULL == queue) {
        return SCL_NULL_QUEUE;
    }

    if ((NULL == queue->front) || (0 == queue->size)) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }


    /* Pointer to current wipe node */
    queue_node_t *delete_node = queue->front;

    /* Update the front pointer of the queue */
    queue->front = queue->front->next;

    /* Decrease queue size */
    --(queue->size);

    /* Free content of the data if necessary */
    if ((NULL != queue->frd) && (NULL != delete_node->data)) {
        queue->frd(delete_node->data);
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