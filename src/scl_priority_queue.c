/**
 * @file scl_priority_queue.c
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

#include "./include/scl_priority_queue.h"

/**
 * @brief Default capacity for a priority queue
 * and default ratio of reallocation for a priority queue
 * 
 */
#define DEFAULT_CAPACITY 10
#define DEFAULT_REALLOC_RATIO 2

/**
 * @brief Create a priority queue object. Function will create
 * a new priority queue object . Function may fail if `cmp_pr`
 * function is not specified in input data, also if not enough memory is left
 * on heap for priority queue the function will fail.
 * 
 * @param init_capacity initial size allocated for a priority queue
 * @param cmp_pr a pointer to a function to compare two sets of priorities
 * @param cmp_dt a pointer to a function to compare two sets of data
 * @param frd_pr a pointer to a function to free memory of one priority set
 * @param frd_dt a pointer to a function to free memory of one data set
 * @param pri_size length in bytes of the pri data type
 * @param data_size length in bytes of the data data type
 * @return priority_queue_t* a new allocated priority queue object or `NULL` if function fails
 */
priority_queue_t* create_priority_queue(size_t init_capacity, compare_func cmp_pr, compare_func cmp_dt, free_func frd_pr, free_func frd_dt, size_t pri_size, size_t data_size) {
    /* Check if input data is valid */
    if (NULL == cmp_pr) {
        errno = EINVAL;
        perror("Compare function undefined for priority queue");
        return NULL;
    }

    /* Check if data and pri sizes are valid */
    if (0 == pri_size) {
        errno = EINVAL;
        perror("Priority type size are zero");
        return NULL;
    }

    /* Set default capacity if necessary */
    if (0 == init_capacity) {
        init_capacity = DEFAULT_CAPACITY;
    }

    /* Allocate a new priority queue object on heap memory */
    priority_queue_t *new_pri_queue = malloc(sizeof(*new_pri_queue));

    /* Check if priority queue was allocated successfully */
    if (NULL != new_pri_queue) {

        /* Set priority queue default functions */
        new_pri_queue->cmp_pr = cmp_pr;
        new_pri_queue->cmp_dt = cmp_dt;
        new_pri_queue->frd_pr = frd_pr;
        new_pri_queue->frd_dt = frd_dt;

        /* Set default size and capacity for priority queue */
        new_pri_queue->pri_size = pri_size;
        new_pri_queue->data_size = data_size;
        new_pri_queue->capacity = init_capacity;
        new_pri_queue->size = 0;

        /* Allocate memory for heap nodes */
        new_pri_queue->nodes = malloc(sizeof(*new_pri_queue->nodes) * init_capacity);

        /* Check if heap nodes were allocated successfully */
        if (NULL == new_pri_queue->nodes) {
            free(new_pri_queue);
            return NULL;
        }

        /* Initialize every heap node as `NULL` */
        for (size_t iter = 0; iter < init_capacity; ++iter) {
            new_pri_queue->nodes[iter] = NULL;    
        }  
    }

    /* Return a new allocated priority queue or `NULL` */
    return new_pri_queue;
}

/**
 * @brief Function to free memory allocated for one priority queue
 * node object. Function may fail if priority queue is not allocated
 * or node does not exist.
 * 
 * @param pqueue an allocated priority queue object
 * @param free_node pointer to pointer of a node to delete from heap
 */
static void free_priority_queue_node(const priority_queue_t * const __restrict__ pqueue, pri_node_t ** const __restrict__ free_node) {
    /* Check if node needs to be freed */
    if ((NULL != pqueue) && (NULL != free_node) && (NULL != *free_node)) {

        /* Free memory allocated for data content */
        if ((NULL != pqueue->frd_dt) && (NULL != (*free_node)->data)) {
            pqueue->frd_dt((*free_node)->data);
        }

        /* Free data pointer */
        if (NULL != (*free_node)->data) {
            free((*free_node)->data);
        }

        /* Set data pointer to default value */
        (*free_node)->data = NULL;

        /* Free memory allocated for priority content */
        if ((NULL != pqueue->frd_pr) && (NULL != (*free_node)->pri)) {
            pqueue->frd_pr((*free_node)->pri);
        }

        /* Free priority pointer */
        if (NULL != (*free_node)->pri) {
            free((*free_node)->pri);
        }

        /* Set priority pointer to default value */
        (*free_node)->pri = NULL;

        /* Free node pointer */
        free((*free_node));

        /* Set node pointer to default value */
        (*free_node) = NULL;
    }
}

/**
 * @brief Function to free the entire priority queue from heap
 * memory. Function will process just allocated priority queues
 * 
 * @param pqueue an allocated priority queue object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_priority_queue(priority_queue_t * const __restrict__ pqueue) {
    /* Check if priority queue is valid */
    if (NULL != pqueue) {

        /* Check if heap nodes need to be freed */
        if (NULL != pqueue->nodes) {

            /* Free every heap node */
            for (size_t iter = 0; iter < pqueue->size; ++iter) {
                free_priority_queue_node(pqueue, &pqueue->nodes[iter]);
            }

            /* Free heap nodes pointer and set to default value */
            free(pqueue->nodes);
            pqueue->nodes = NULL;  
        }

        /* Free priority queue pointer and set to default value */
        free(pqueue);

        return SCL_OK;
    }

    return SCL_NULL_PRIORITY_QUEUE;
}

/**
 * @brief MACRO to get index of the left child of selected node
 * 
 */
#define get_node_left_child_pos(node_index) (2 * (node_index) + 1)

/**
 * @brief MACRO to get index of the right child of selected node
 * 
 */
#define get_node_right_child_pos(node_index) (2 * (node_index) + 2)

/**
 * @brief MACRO to get index of the parent of selected node
 * 
 */
#define get_node_parent_pos(node_index) (((node_index) - 1) / 2)

/**
 * @brief Function to swap data between two sets of data
 * 
 * @param first_node pointer to pointer of priority queue node object
 * @param second_node pointer to pointer of priority queue node object
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t swap_pri_queue_nodes(pri_node_t ** const __restrict__ first_node, pri_node_t ** const __restrict__ second_node) {
    /* Check if nodes can be swapped */
    if ((NULL == first_node) || (NULL == second_node) || (NULL == (*first_node)) || (NULL == (*second_node))) {
        return SCL_CANNOT_SWAP_DATA;
    }

    /* Swap node pointers */
    pri_node_t* const temp_node = *first_node;
    *first_node = *second_node;
    *second_node = temp_node;

    return SCL_OK;
}

/**
 * @brief Function to sift one node up to repair the
 * proprieties of a heap structure. Function may fail if
 * priority queue is not allocated, if nodes are not allocated
 * or if function to compare priority is `NULL`.
 * 
 * @param pqueue an allocated priority queue object
 * @param start_index starting index from priority queue
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t sift_node_up(const priority_queue_t * const __restrict__ pqueue, size_t start_index) {
    /* Check if input data is valid */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if (NULL == pqueue->cmp_pr) {
        return SCL_NULL_COMPARE_PRIORITY_FUNC;
    }

    scl_error_t err = SCL_OK;

    /* Sift heap node up until it reaches its position according to its priority */
    while ((start_index > 0) && (pqueue->cmp_pr(pqueue->nodes[start_index]->pri, pqueue->nodes[get_node_parent_pos(start_index)]->pri) >= 1)) {
        /* Swap nodes */
        err = swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[get_node_parent_pos(start_index)]);
        
        if (SCL_OK != err) {
            return err;
        }

        /* Check new position of the current heap node */
        start_index = get_node_parent_pos(start_index);
    }

    /* Sifting up went successfully */
    return SCL_OK;
}

/**
 * @brief Function to sift one node down to repair the
 * proprieties of a heap structure. Function may fail if
 * priority queue is not allocated, if nodes are not allocated
 * or if function to compare priority is `NULL`.
 * 
 * @param pqueue an allocated priority queue object
 * @param start_index starting index from priority queue
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t sift_node_down(const priority_queue_t * const __restrict__ pqueue, size_t start_index) {
    /* Check if input data is valid */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if (NULL == pqueue->cmp_pr) {
        return SCL_NULL_COMPARE_PRIORITY_FUNC;
    }

    /* Initialize swap index as current heap node index */
    size_t swap_index = start_index;

    /* Initialize index for checking as the left child index */
    size_t check_index = get_node_left_child_pos(start_index);

    /* Check if left child has a bigger/smaller priority than current heap node */
    if ((check_index < pqueue->size) && (pqueue->cmp_pr(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)) {
        swap_index = check_index;
    }

    /* Set index for checking to right child index */
    check_index = get_node_right_child_pos(start_index);

    /* Check if right child has a bigge/smaller priority than current heap node or brother node */
    if ((check_index < pqueue->size) && (pqueue->cmp_pr(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)) {
        swap_index = check_index;
    }

    scl_error_t err = SCL_OK;

    /* If swap index is the same than start index than node was sifted down enough */
    if (start_index != swap_index) {
        /* Swap current heap node with swap heap node */
        err = swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[swap_index]);

        if (SCL_OK != err) {
            return err;
        }

        /* Sift down the new position of the current heap node */
        return sift_node_down(pqueue, swap_index); 
    }

    /* Sifting down went successfully */
    return SCL_OK;
}

/**
 * @brief Create a priority queue node object. Function may fail if priority pointer
 * is `NULL` or if its size is 0. One priority queue node may have NO data (`NULL`), but
 * it must have a valid priority.
 * 
 * @param pqueue an allocated priority queue object
 * @param data pointer to a set of generic data type to insert in priority queue
 * @param priority pointer to a set of generic data type to represent priority in queue
 * @return pri_node_t* an allocated priority queue node object or `NULL` in case function fails
 */
static pri_node_t* create_priority_queue_node(const priority_queue_t * const __restrict__ pqueue, const void *data, const void *priority) {
    /* Check if input data is valid */
    if (NULL == priority) {
        return NULL;
    }

    /* Allocate a new priority queue node on heap memory */
    pri_node_t *new_pri_queue_node = malloc(sizeof(*new_pri_queue_node));

    /* Check if new priority queue node was allocated successfully */
    if (NULL != new_pri_queue_node) {

        /* Check if new node will have a valid data pointer */
        if (NULL != data) {

            /* Allcoate memory for data content */
            new_pri_queue_node->data = malloc(pqueue->data_size);

            /* Check if data content was allocated */
            if (NULL == new_pri_queue_node->data) {
                free(new_pri_queue_node);
            
                errno = ENOMEM;
                perror("Not enough memory for data value allocation");
            
                return NULL;
            }

            /* Copy all bytes from data to data node pointer */
            memcpy(new_pri_queue_node->data, data, pqueue->data_size);
        } else {

            /* New node does not have data so set it to default value */
            new_pri_queue_node->data = NULL;
        }

        /* Allocate memory for priority content */
        new_pri_queue_node->pri = malloc(pqueue->pri_size);

        /* Check if priority content was allocated */
        if (NULL == new_pri_queue_node->pri) {
            if (NULL != new_pri_queue_node->data) {
                free(new_pri_queue_node->data);
            }

            free(new_pri_queue_node);
            
            errno = ENOMEM;
            perror("Not enough memory for priority value allocation");
            
            return NULL;
        }

        /* Copy all bytes from priority to priority node pointer */
        memcpy(new_pri_queue_node->pri, priority, pqueue->pri_size);
    } else {
        errno = ENOMEM;
        perror("Not enough memory for priority queue node allocation");
    }

    /* Return a new allocated priority queue node or `NULL` */
    return new_pri_queue_node;
}

/**
 * @brief Function to create a priority queue starting from an array.
 * Function will take O(N) time to create the priority queue, which is
 * faster than O(NlogN) time. However function may fail and return `NULL`
 * if priority array is not allocated or an priority element is not allocated,
 * also a valid priority compare function.
 * 
 * @param empty_pqueue an allocated EMPTY priority queue object
 * @param data an array of elements to insert into a priority queue
 * @param priority an array of priorities to insert into a priority queue
 * @return scl_error_t enum object for handling errors
 */
scl_error_t heapify(priority_queue_t * const __restrict__ empty_pqueue, const void *data, const void *priority) {
    if (0 == is_priq_empty(empty_pqueue)) {
        return SCL_NOT_EMPTY_PRIORITY_QUEUE;
    }

    /* Fill up heap nodes pointers with real content without following heap rules */
    for (size_t iter = 0; iter < empty_pqueue->capacity; ++iter) {

        /* Initialize a new priority queue node as default values */
        pri_node_t *new_pqueue_node = NULL;

        /* Create a new priority queue node depending by data pointer */
        if ((NULL != data) && (0 != empty_pqueue->data_size)) {
            new_pqueue_node = create_priority_queue_node(empty_pqueue, (const uint8_t *)data + iter * empty_pqueue->data_size, (const uint8_t *)priority + iter * empty_pqueue->pri_size);
        } else {
            new_pqueue_node = create_priority_queue_node(empty_pqueue, NULL, (const uint8_t *)priority + iter * empty_pqueue->pri_size);
        }

        /* Check if new priority queue node was created successfully */
        if (NULL == new_pqueue_node) {
            return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
        }
        
        /* Link new created priority queue node to heap nodes */
        empty_pqueue->nodes[iter] = new_pqueue_node;

        /* Increase priority queue's size */
        ++(empty_pqueue->size);
    }

    /* Sift down every node that is not a leaf in binary-heap */
    for (int64_t iter = (int64_t)((empty_pqueue->size / 2) - 1); iter >= 0; --iter) {
        sift_node_down(empty_pqueue, (size_t)iter);
    }

    /* Return a new allocated priority queue or NULL */
    return SCL_OK;
}

/**
 * @brief Function to change the priority of one node in the
 * priority queue object. After the priority of the selected
 * node is changed, function will update by itself the new
 * location of the selected node so the heap propreties are
 * preserved.
 * 
 * @param pqueue an allocaetd priority queue object
 * @param node_index index of the node to change it's priority
 * @param new_pri pointer to a new set of priority data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t change_node_priority(const priority_queue_t * const __restrict__ pqueue, size_t node_index, const void * __restrict__ new_pri) {
    /* Check if input data is valid */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    /* Check if priority queue nodes are allocated */
    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    /* Check if index of the node is not out of bound */
    if ((SIZE_MAX == node_index) || (node_index >= pqueue->size)) {
        return SCL_INDEX_OVERFLOWS_SIZE;
    }

    /* Check if selected node is in the priority queue */
    if (NULL == pqueue->nodes[node_index]) {
        return SCL_CHANGE_PRIORITY_TO_NULL;
    }

    if (NULL == new_pri) {
        return SCL_INVALID_PRIORITY;
    }

    if (NULL == pqueue->cmp_pr) {
        return SCL_NULL_COMPARE_PRIORITY_FUNC;
    }

    if (NULL == pqueue->nodes[node_index]->pri) {
        return SCL_UNDEFINED_PRIORITY;
    }

    /*
     * Sift selected node down if new priority has a smaller rank 
     * than old priority according to compare function
     */
    if (pqueue->cmp_pr(pqueue->nodes[node_index]->pri, new_pri) >= 1) {

        /* Copy new priority into old priority */
        memcpy(pqueue->nodes[node_index]->pri, new_pri, pqueue->pri_size);

        /* Sift node down */
        return sift_node_down(pqueue, node_index);
    }

    /*
     * Sift selected node up if new priority has a bigger rank 
     * than old priority according to compare function
     */
    if (pqueue->cmp_pr(pqueue->nodes[node_index]->pri, new_pri) <= -1) {

        /* Copy new priority into old priority */
        memmove(pqueue->nodes[node_index]->pri, new_pri, pqueue->pri_size);

        /* Sift node up */
        return sift_node_up(pqueue, node_index);
    }

    /* Old priority is equal to new priority */
    return SCL_OK;
}

/**
 * @brief Function to change the data of one node in the
 * priority queue object. After the data of the selected
 * node is changed, the priority queue proprietis will be
 * preserved because data does not contribue in the arrangement
 * of the priority queue.
 * 
 * @param pqueue an allocaetd priority queue object
 * @param node_index index of the node to change it's data
 * @param new_data pointer to a new set of data type
 * @return scl_error_t enum object for handling errors
 */
scl_error_t change_node_data(const priority_queue_t * const __restrict__ pqueue, size_t node_index, const void * __restrict__ new_data) {
    /* Check if input data is valid */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if ((SIZE_MAX == node_index) || (node_index >= pqueue->size)) {
        return SCL_INDEX_OVERFLOWS_SIZE;
    }

    if (NULL == pqueue->nodes[node_index]) {
        return SCL_CHANGE_PRIORITY_TO_NULL;
    }

    if (NULL == new_data) {
        return SCL_INVALID_DATA;
    }

    if (NULL == pqueue->nodes[node_index]->data) {
        return SCL_UNDEFINED_DATA;
    }

    /* Copy a new value in old data pointer */
    memcpy(pqueue->nodes[node_index]->data, new_data, pqueue->data_size);
    
    return SCL_OK;
}

/**
 * @brief Function to find the index of one data element. Function
 * will fail if priority queue is empty or if data was not found.
 * 
 * @param pqueue priority queue object
 * @param data pointer to a generic data type
 * @return size_t index of the found data from input or `SIZE_MAX` in case
 * data was not found in priority queue
 */
size_t pri_find_data_index(const priority_queue_t * const __restrict__ pqueue, const void * const __restrict__ data) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (0 == pqueue->size) || (NULL == pqueue->nodes) || (NULL == data) || (NULL == pqueue->cmp_dt)) {
        return SIZE_MAX;
    }

    /* Find desired data index according to compare function */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        if (NULL != pqueue->nodes[iter]) {
            if (0 == pqueue->cmp_dt(pqueue->nodes[iter]->data, data)) {
                return iter;
            }
        }    
    }

    /* No data was found according to compare data function */
    return SIZE_MAX;
}

/**
 * @brief Function to find the index of one priority element. Function
 * will fail if priority queue is empty or if priority was not found.
 * 
 * @param pqueue priority queue object
 * @param priority pointer to a generic priority type
 * @return size_t index of the found priority from input or
 * `SIZE_MAX` in case priority was not found in priority queue
 */
size_t pri_find_pri_index(const priority_queue_t * const __restrict__ pqueue, const void * const __restrict__ priority) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (0 == pqueue->size) || (NULL == pqueue->nodes) || (NULL == priority) || (NULL == pqueue->cmp_pr)) {
        return SIZE_MAX;
    }

    /* Find desired priority index according to compare function */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        if (NULL != pqueue->nodes[iter]) {
            if (0 == pqueue->cmp_pr(pqueue->nodes[iter]->pri, priority)) {
                return iter;
            }
        }
    }

    /* No priority was found according to compare priority function */
    return SIZE_MAX;
}

/**
 * @brief Function to push one element in priority queue object.
 * Function may fail if reallocation of the array fails or if
 * node is not created successfully.
 * 
 * @param pqueue priority queue object
 * @param data pointer to one data element
 * @param priority pointer to one priority element
 * @return scl_error_t enum object for handling errors
 */
scl_error_t pri_queue_push(priority_queue_t * const __restrict__ pqueue, const void *data, const void *priority) {
    /* Check if input data is valid */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if (0 == pqueue->capacity) {
        return SCL_PQUEUE_CAPACITY_ZERO;
    }

    if (NULL == priority) {
        return SCL_INVALID_PRIORITY;
    }

    /*
     * Check if priority queue is full
     * If true allocate more heap nodes
     */
    if (pqueue->size >= pqueue->capacity) {

        /* Increase capacity by default reallocation ratio */
        pqueue->capacity *= DEFAULT_REALLOC_RATIO;
        
        /* Try to realloc heap nodes */
        pri_node_t **try_realloc = realloc(pqueue->nodes, sizeof(*(pqueue->nodes)) * pqueue->capacity);

        /*
         * If reallocation went wrong exit pushing function
         * and throw an error
         */
        if (NULL == try_realloc) {
            pqueue->capacity /= DEFAULT_REALLOC_RATIO;

            errno = ENOMEM;
            perror("Not enough memory to reallocate new nodes");

            return SCL_REALLOC_PQNODES_FAIL;
        }

        /* Set heap nodes pointer to the new memory location */
        pqueue->nodes = try_realloc;
    }

    /* Create a new priority queue node */
    pri_node_t *add_node = create_priority_queue_node(pqueue, data, priority);

    /* If new node was not created exit pushing function */
    if (NULL == add_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Link new created node to heap nodes */
    pqueue->nodes[pqueue->size] = add_node;

    /* Increase priority queue size */
    ++(pqueue->size);

    /* Sift node up to preserve heap rules */
    return sift_node_up(pqueue, (pqueue->size - 1));
}

/**
 * @brief Function to get the data element with the highest
 * rank from current priority queue object.
 * 
 * @param pqueue priority queue object
 * @return const void* pointer to first data element
 * from priority queue, user should not modify this pointer.
 */
const void* pri_queue_top(const priority_queue_t * const __restrict__ pqueue) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == pqueue->nodes[0])) {
        return NULL;
    }

    /* Return the peek data pointer */
    return pqueue->nodes[0]->data;
}

/**
 * @brief Function to get the priority element with the highest
 * rank from current priority queue object.
 * 
 * @param pqueue priority queue object
 * @return const void* pointer to first priority element
 * from priority queue, user should not modify this pointer.
 */
const void* pri_queue_top_pri(const priority_queue_t * const __restrict__ pqueue) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == pqueue->nodes[0])) {
        return NULL;
    }

    /* Return the highest ranking priority */
    return pqueue->nodes[0]->pri;
}

/**
 * @brief Function to remove the highest rank node from
 * current specified priority queue object. Function may
 * fail if priority queue is empty or not allocated.
 * 
 * @param pqueue priority queue object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t pri_queue_pop(priority_queue_t * const __restrict__ pqueue) {
    /* Check if input data is valid and if there are nodes to pop from priority queue */
    if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if (0 == pqueue->capacity) {
        return SCL_PQUEUE_CAPACITY_ZERO;
    }

    if (0 == pqueue->size) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    scl_error_t err = SCL_OK;

    /* Swap first heap node with last heap node */
    err = swap_pri_queue_nodes(&pqueue->nodes[0], &pqueue->nodes[pqueue->size - 1]);

    if (SCL_OK != err) {
        return err;
    }

    /* Free memory allocated for last heap node */
    free_priority_queue_node(pqueue, &pqueue->nodes[pqueue->size - 1]);

    /* Decrease priority queue size */
    --(pqueue->size);

    /* Sift first heap node down to reestablish heap propreties */
    return sift_node_down(pqueue, 0);
}

/**
 * @brief Function that will traverse all nodes in priority queue
 * and will perform any action according to "action" function.
 * Usually action will be a printing function, however you
 * can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve
 * heap prorpeties)
 * 
 * @param pqueue an allocated priority queue object
 * @param action a pointer to a function that will perform an action
 * @return scl_error_t enum object for handling errors
 */
scl_error_t pri_queue_traverse(const priority_queue_t * const __restrict__ pqueue, action_func action) {
    /* Check if input data value is valid */
        if (NULL == pqueue) {
        return SCL_NULL_PRIORITY_QUEUE;
    }

    if (NULL == pqueue->nodes) {
        return SCL_NULL_PQUEUE_NODES;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /*
     * Traverse every single node and make an action to node
     * according with input function
     */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        if (NULL != pqueue->nodes[iter]) {
            action(pqueue->nodes[iter]->data);
        }
    }

    return SCL_OK;
}

/**
 * @brief Function will return the size of the priority queue
 * object. If priority queue is not allocated than `__SIZE_MAX__`
 * will be returned as an warning.
 * 
 * @param pqueue an allocated priority queue object
 * @return size_t current size of the priority queue object
 */
size_t pri_queue_size(const priority_queue_t * const __restrict__ pqueue) {
    /* Check if priority queue is valid */
    if (NULL == pqueue) {
        return SIZE_MAX;
    }

    /* Return priority queue size */
    return pqueue->size;
}

/**
 * @brief Function to check if a priority queue object is
 * empty or not. A not allocated object is also an empty object
 * 
 * @param pqueue an allocated priority queue object
 * @return uint8_t 1 if priority queue object is not allocated or empty, 0 if
 * priority queue is not empty
 */
uint8_t is_priq_empty(const priority_queue_t * const __restrict__ pqueue) {
    /* Check fi priority queue is valid and if it is empty */
    if ((NULL == pqueue) || (0 == pqueue->capacity) || (NULL == pqueue->nodes) || (0 == pqueue->size)) {
        return 1;
    }

    /* Priority queue is not empty */
    return 0;
}

/**
 * @brief Function to sort elements of an array by heap sort
 * method. Function will call heapify function to make a priority queue
 * where array elements will be priorities and data will be set as `NULL`.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t heap_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if input data is valid */
    if (NULL == arr) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* Heapify the input array in O(N) complexity */
    priority_queue_t *heap = create_priority_queue(number_of_elem, cmp, NULL, NULL, NULL, arr_elem_size, 0);
    
    scl_error_t err = heapify(heap, NULL, arr);

    if (SCL_OK != err) {
        return err;
    }

    /* Rearrange input array data from heap structure in O(NlogN) complexity */
    for (size_t iter = 0; iter < number_of_elem; ++iter) {
        memcpy((uint8_t *)arr + iter * arr_elem_size, pri_queue_top_pri(heap), arr_elem_size);
        err = pri_queue_pop(heap);

        if (SCL_OK != err) {
            return err;
        }
    }

    /* Free heap memory in O(N) complexity */
    return free_priority_queue(heap);

    /* Overall complexity O(NlogN) */
}