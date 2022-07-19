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
 * a new priority queue object . Function may fail if `compare_priority`
 * function is not specified in input data, also if not enough memory is left
 * on heap for priority queue the function will fail.
 * 
 * @param init_capacity initial size allocated for a priority queue
 * @param compare_data a pointer to a function to compare two sets of data
 * @param compare_priority a pointer to a function to compare two sets of priorities
 * @param free_data a pointer to a function to free memory of one data set
 * @param free_priority a pointer to a function to free memory of one priority set
 * @return priority_queue_t* a new allocated priority queue object or `NULL` if function fails
 */
priority_queue_t* create_priority_queue(
    size_t init_capacity,
    int (*compare_data)(const void*, const void*),
    int (*compare_priority)(const void*, const void*),
    void (*free_data)(void*),
    void (*free_priority)(void*)
) {
    /* Check if input data is valid */
    if (NULL == compare_priority) {
        errno = EINVAL;
        perror("Compare function undefined for priority queue");
        return NULL;
    }

    /* Set default capacity if necessary */
    if (0 == init_capacity) {
        init_capacity = DEFAULT_CAPACITY;
    }

    /* Allocate a new priority queue object on heap memory */
    priority_queue_t* new_pri_queue = malloc(sizeof(*new_pri_queue));

    /* Check if priority queue was allocated successfully */
    if (NULL != new_pri_queue) {

        /* Set priority queue default functions */
        new_pri_queue->compare_data = compare_data;
        new_pri_queue->compare_priority = compare_priority;
        new_pri_queue->free_data = free_data;
        new_pri_queue->free_priority = free_priority;

        /* Set default size and capacity for priority queue */
        new_pri_queue->capacity = init_capacity;
        new_pri_queue->size = 0;

        /* Allocate memory for heap nodes */
        new_pri_queue->nodes = malloc(sizeof(*new_pri_queue->nodes) * init_capacity);

        /* Check if heap nodes were allocated successfully */
        if (NULL == new_pri_queue->nodes) {
            free(new_pri_queue);
            return NULL;
        }

        /* Initialize every heap node as NULL */
        for (size_t iter = 0; iter < init_capacity; ++iter) {
            new_pri_queue->nodes[iter] = NULL;    
        }  
    }

    /* Return a new allocated priority queue or NULL */
    return new_pri_queue;
}

/**
 * @brief Function to free memory allocated for one priority queue
 * node object. Function may fail if priority queue is not allocated
 * or node does not exist.
 * 
 * @param pqueue an allocated priority queue object
 * @param free_node an allocated priority queue node object
 */
static void free_priority_queue_node(priority_queue_t* pqueue, pri_node_t** free_node) {
    /* Check if node needs to be freed */
    if ((NULL != pqueue) && (NULL != free_node) && (NULL != *free_node)) {

        /* Free memory allocated for data content */
        if ((NULL != pqueue->free_data) && (NULL != (*free_node)->data)) {
            pqueue->free_data((*free_node)->data);
        }

        /* Free data pointer */
        if (NULL != (*free_node)->data) {
            free((*free_node)->data);
        }

        /* Set data pointer to default value */
        (*free_node)->data = NULL;

        /* Free memory allocated for priority content */
        if ((NULL != pqueue->free_priority) && (NULL != (*free_node)->pri)) {
            pqueue->free_priority((*free_node)->pri);
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
 */
void free_priority_queue(priority_queue_t* pqueue) {
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
        pqueue = NULL;
    }
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
 * @param first_node priority queue node object
 * @param second_node priority queue node object
 */
static void swap_pri_queue_nodes(pri_node_t** first_node, pri_node_t** second_node) {
    /* Check if nodes can be swapped */
    if ((NULL == first_node) || (NULL == second_node) || (NULL == (*first_node)) || (NULL == (*second_node))) {
        return;
    }

    /* Swap node pointers */
    pri_node_t* temp_node = *first_node;
    *first_node = *second_node;
    *second_node = temp_node;
}

/**
 * @brief Function to sift one node up to repair the
 * proprieties of a heap structure. Function may fail if
 * priority queue is not allocated, if nodes are not allocated
 * or if function to compare priority is `NULL`.
 * 
 * @param pqueue an allocated priority queue object
 * @param start_index starting index from priority queue
 * @return int 1 if function fails and 0 otherwise
 */
static int sift_node_up(priority_queue_t* pqueue, size_t start_index) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == pqueue->compare_priority)) {
        return 1;
    }

    /* Sift heap node up until it reaches its position according to its priority */
    while ((start_index > 0) && (pqueue->compare_priority(pqueue->nodes[start_index]->pri, pqueue->nodes[get_node_parent_pos(start_index)]->pri) >= 1)) {
        /* Swap nodes */
        swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[get_node_parent_pos(start_index)]);
        
        /* Check new position of the current heap node */
        start_index = get_node_parent_pos(start_index);
    }

    /* Sifting up went successfully */
    return 0;
}

/**
 * @brief Function to sift one node down to repair the
 * proprieties of a heap structure. Function may fail if
 * priority queue is not allocated, if nodes are not allocated
 * or if function to compare priority is `NULL`.
 * 
 * @param pqueue an allocated priority queue object
 * @param start_index starting index from priority queue
 * @return int 1 if function fails and 0 otherwise
 */
static int sift_node_down(priority_queue_t* pqueue, size_t start_index) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == pqueue->compare_priority)) {
        return 1;
    }

    /* Initialize swap index as current heap node index */
    size_t swap_index = start_index;

    /* Initialize index for checking as the left child index */
    size_t check_index = get_node_left_child_pos(start_index);

    /* Check if left child has a bigger/smaller priority than current heap node */
    if ((check_index < pqueue->size) && (pqueue->compare_priority(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)) {
        swap_index = check_index;
    }

    /* Set index for checking to right child index */
    check_index = get_node_right_child_pos(start_index);

    /* Check if right child has a bigge/smaller priority than current heap node or brother node */
    if ((check_index < pqueue->size) && (pqueue->compare_priority(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)) {
        swap_index = check_index;
    }

    /* If swap index is the same than start index than node was sifted down enough */
    if (start_index != swap_index) {
        /* Swap current heap node with swap heap node */
        swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[swap_index]);

        /* Sift down the new position of the current heap node */
        return sift_node_down(pqueue, swap_index); 
    }

    /* Sifting down went successfully */
    return 0;
}

/**
 * @brief Create a priority queue node object. Function may fail if priority pointer
 * is `NULL` or if its size is 0. One priority queue node may have NO data (NULL), but
 * it must have a valid priority.
 * 
 * @param data pointer to a set of generic data type to insert in priority queue
 * @param priority pointer to a set of generic data type to represent priority in queue
 * @param data_size size of one element of data type
 * @param pri_size size of one element of priority type
 * @return pri_node_t* an allocated priority queue node object or `NULL` in case function fails
 */
static pri_node_t* create_priority_queue_node(const void* data, const void* priority, size_t data_size, size_t pri_size) {
    /* Check if input data is valid */
    if ((NULL == priority) || (0 == pri_size)) {
        return NULL;
    }

    /* Allocate a new priority queue node on heap memory */
    pri_node_t* new_pri_queue_node = malloc(sizeof(*new_pri_queue_node));

    /* Check if new priority queue node was allocated successfully */
    if (NULL != new_pri_queue_node) {

        /* Check if new node will have a valid data pointer */
        if ((NULL != data) && (0 != data_size)) {

            /* Allcoate memory for data content */
            new_pri_queue_node->data = malloc(data_size);

            /* Check if data content was allocated */
            if (NULL == new_pri_queue_node->data) {
                free(new_pri_queue_node);
            
                errno = ENOMEM;
                perror("Not enough memory for data allocation");
            
                return NULL;
            }

            /* Copy all bytes from data to data node pointer */
            memcpy(new_pri_queue_node->data, data, data_size);
        } else {

            /* New node does not have data so set it to default value */
            new_pri_queue_node->data = NULL;
        }

        /* Allocate memory for priority content */
        new_pri_queue_node->pri = malloc(pri_size);

        /* Check if priority content was allocated */
        if (NULL == new_pri_queue_node->pri) {
            if (NULL != new_pri_queue_node->data) {
                free(new_pri_queue_node->data);
            }

            free(new_pri_queue_node);
            
            errno = ENOMEM;
            perror("Not enough memory for priority allocation");
            
            return NULL;
        }

        /* Copy all bytes from priority to priority node pointer */
        memcpy(new_pri_queue_node->pri, priority, pri_size);
    } else {
        errno = ENOMEM;
        perror("Not enough memory for priority queue node allocation");
    }

    /* Return a new allocated priority queue node or NULL */
    return new_pri_queue_node;
}

/**
 * @brief Function to create a priority queue starting from an array.
 * Function will take O(N) time to create the priority queue, which is
 * faster than O(NlogN) time. However function may fail and return NULL
 * if priority array is not allocated or an priority element is not allocated,
 * also a valid priority compare function.
 * 
 * @param data an array of elements to insert into a priority queue
 * @param priority an array of priorities to insert into a priority queue
 * @param data_size size of one data element from priority queue
 * @param pri_size size of one priority element from priority queue
 * @param number_of_data number of elements into data and priority array
 * @param compare_data pointer to a function two compare two sets of data
 * @param compare_priority pointer to a function two compare two sets of priority
 * @param free_data a pointer to a function to free memory of one data set
 * @param free_priority a pointer to a function to free memory of one priority set
 * @return priority_queue_t* an allocated priority queue object or `NULL` if function fails
 */
priority_queue_t* heapify(
    const void* data,
    const void* priority,
    size_t data_size,
    size_t pri_size,
    size_t number_of_data,
    int (*compare_data)(const void*, const void*),
    int (*compare_priority)(const void*, const void*),
    void (*free_data)(void*),
    void (*free_priority)(void*)
) {
    /* Check if input data is valid */
    if ((NULL == priority) || (0 == pri_size) || (0 == number_of_data) || (NULL == compare_priority)) {
        return NULL;
    }

    /* Create a new priority queue */
    priority_queue_t* new_pqueue = create_priority_queue(number_of_data, compare_data, compare_priority, free_data, free_priority);

    /* Check if priority queue was created successfully */
    if (NULL != new_pqueue) {

        /* Fill up heap nodes pointers with real content without following heap rules */
        for (size_t iter = 0; iter < new_pqueue->capacity; ++iter) {

            /* Initialize a new priority queue node as default values */
            pri_node_t* new_pqueue_node = NULL;

            /* Create a new priority queue node depending by data pointer */
            if ((NULL != data) && (0 != data_size)) {
                new_pqueue_node = create_priority_queue_node(data + iter * data_size, priority + iter * pri_size, data_size, pri_size);
            } else {
                new_pqueue_node = create_priority_queue_node(NULL, priority + iter * pri_size, 0, pri_size);
            }

            /* Check if new priority queue node was created successfully */
            if (NULL == new_pqueue_node) {
                return NULL;
            }
            
            /* Link new created priority queue node to heap nodes */
            new_pqueue->nodes[iter] = new_pqueue_node;

            /* Increase priority queue's size */
            ++(new_pqueue->size);
        }

        /* Sift down every node that is not a leaf in binary-heap */
        for (int iter = (int)((new_pqueue->size / 2) - 1); iter >= 0; --iter) {
            sift_node_down(new_pqueue, (size_t)iter);
        }
    }

    /* Return a new allocated priority queue or NULL */
    return new_pqueue;
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
 * @param pri_size size of the new priority element
 * @return int 1 if function fails, 0 otherwise
 */
int change_node_priority(priority_queue_t* pqueue, size_t node_index, const void* new_pri, size_t pri_size) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == pqueue->nodes[node_index]) || (NULL == new_pri) || (0 == pri_size)) {
        return 1;
    }

    /* Check if selected node index is a valid index */
    if ((__SIZE_MAX__ == node_index) || (node_index >= pqueue->size)) {
        return 1;
    }

    /* Check if selected priority queue is a valid queue */
    if ((NULL == pqueue->nodes[node_index]->pri) || (NULL == pqueue->compare_priority)) {
        return 1;
    }

    /*
     * Sift selected node down if new priority has a smaller rank 
     * than old priority according to compare function
     */
    if (pqueue->compare_priority(pqueue->nodes[node_index]->pri, new_pri) >= 1) {

        /* Copy new priority into old priority */
        memmove(pqueue->nodes[node_index]->pri, new_pri, pri_size);

        /* Sift node down */
        return sift_node_down(pqueue, node_index);
    }

    /*
     * Sift selected node up if new priority has a bigger rank 
     * than old priority according to compare function
     */
    if (pqueue->compare_priority(pqueue->nodes[node_index]->pri, new_pri) <= -1) {

        /* Copy new priority into old priority */
        memmove(pqueue->nodes[node_index]->pri, new_pri, pri_size);

        /* Sift node up */
        return sift_node_up(pqueue, node_index);
    }

    /* Old priority is equal to new priority */
    return 0;
}

/**
 * @brief Function to find the index of one data element. Function
 * will fail if priority queue is empty or if data was not found.
 * 
 * @param pqueue priority queue object
 * @param data pointer to a generic data type
 * @return size_t index of the found data from input or `__SIZE_MAX__` in case
 * data was not found in priority queue
 */
size_t pri_find_data_index(priority_queue_t* pqueue, const void* data) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (0 == pqueue->size) || (NULL == pqueue->nodes) || (NULL == data) || (NULL == pqueue->compare_data)) {
        return __SIZE_MAX__;
    }

    /* Find desired data index according to compare function */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        if (NULL != pqueue->nodes[iter]) {
            if (0 == pqueue->compare_data(pqueue->nodes[iter]->data, data)) {
                return iter;
            }
        }    
    }

    /* No data was found according to compare data function */
    return __SIZE_MAX__;
}

/**
 * @brief Function to find the index of one priority element. Function
 * will fail if priority queue is empty or if priority was not found.
 * 
 * @param pqueue priority queue object
 * @param priority pointer to a generic priority type
 * @return size_t index of the found priority from input or
 * `__SIZE_MAX__` in case priority was not found in priority queue
 */
size_t pri_find_pri_index(priority_queue_t* pqueue, const void* priority) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (0 == pqueue->size) || (NULL == pqueue->nodes) || (NULL == priority) || (NULL == pqueue->compare_priority)) {
        return __SIZE_MAX__;
    }

    /* Find desired priority index according to compare function */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        if (NULL != pqueue->nodes[iter]) {
            if (0 == pqueue->compare_priority(pqueue->nodes[iter]->pri, priority)) {
                return iter;
            }
        }
    }

    /* No priority was found according to compare priority function */
    return __SIZE_MAX__;
}

/**
 * @brief Function to push one element in priority queue object.
 * Function may fail if reallocation of the array fails or if
 * node is not created successfully.
 * 
 * @param pqueue priority queue object
 * @param data pointer to one data element
 * @param priority pointer to one priority element
 * @param data_size size of one data element
 * @param pri_size size of one priority element
 * @return int 1 if function failed or 0 otherwise
 */
int pri_queue_push(priority_queue_t* pqueue, const void* data, const void* priority, size_t data_size, size_t pri_size) {
    /* Check if input data is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (0 == pqueue->capacity) || (NULL == priority) || (0 == pri_size)) {
        return 1;
    }

    /*
     * Check if priority queue is full
     * If true allocate more heap nodes
     */
    if (pqueue->size >= pqueue->capacity) {

        /* Increase capacity by default reallocation ratio */
        pqueue->capacity *= DEFAULT_REALLOC_RATIO;
        
        /* Try to realloc heap nodes */
        pri_node_t** try_realloc = realloc(pqueue->nodes, sizeof(*(pqueue->nodes)) * pqueue->capacity);

        /*
         * If reallocation went wrong exit pushing function
         * and throw an error
         */
        if (NULL == try_realloc) {
            pqueue->capacity /= DEFAULT_REALLOC_RATIO;

            errno = ENOMEM;
            perror("Not enough memory to reallocate new nodes");

            return 1;
        }

        /* Set heap nodes pointer to the new memory location */
        pqueue->nodes = try_realloc;
    }

    /* Create a new priority queue node */
    pri_node_t* add_node = create_priority_queue_node(data, priority, data_size, pri_size);

    /* If new node was not created exit pushing function */
    if (NULL == add_node) {
        return 1;
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
const void* pri_queue_top(priority_queue_t* pqueue) {
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
const void* pri_queue_top_pri(priority_queue_t* pqueue) {
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
 * fail if queue is empty or not allocated.
 * 
 * @param pqueue priority queue object
 * @return int 1 if function fails or 0 otherwise
 */
int pri_queue_pop(priority_queue_t* pqueue) {
    /* Check if input data is valid and if there are nodes to pop from priority queue */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (0 == pqueue->capacity) || (0 == pqueue->size)) {
        return 1;
    }

    /* Swap first heap node with last heap node */
    swap_pri_queue_nodes(&pqueue->nodes[0], &pqueue->nodes[pqueue->size - 1]);

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
 */
void pri_queue_traverse(priority_queue_t* pqueue, void (*action)(const pri_node_t*)) {
    /* Check if input data value is valid */
    if ((NULL == pqueue) || (NULL == pqueue->nodes) || (NULL == action)) {
        return;
    }

    /*
     * Traverse every single node and make an action to node
     * according with input function
     */
    for (size_t iter = 0; iter < pqueue->size; ++iter) {
        action(pqueue->nodes[iter]);
    }
}

/**
 * @brief Function will return the size of the priority queue
 * object. If priority queue is not allocated than `__SIZE_MAX__`
 * will be returned as an warning.
 * 
 * @param pqueue an allocated priority queue object
 * @return size_t current size of the priority queue object
 */
size_t pri_queue_size(priority_queue_t* pqueue) {
    /* Check if priority queue is valid */
    if (NULL == pqueue) {
        return __SIZE_MAX__;
    }

    /* Return priority queue size */
    return pqueue->size;
}

/**
 * @brief Function to check if a priority queue object is
 * empty or not. A not allocated object is also an empty object
 * 
 * @param pqueue an allocated priority queue object
 * @return int 1 if priority queue object is not allocated or empty, 0 if
 * priority queue is not empty
 */
int is_priq_empty(priority_queue_t* pqueue) {
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
 * where array elements will be priorities and data will be set as NULL.
 * 
 * @param arr an array of any typeto sort its elements
 * @param number_of_arr number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param compare_arr pointer to a function to compare two sets of data from array
 */
void heap_sort(void* arr, size_t number_of_arr, size_t arr_elem_size, int (*compare_arr)(const void*, const void*)) {
    /* Check if input data is valid */
    if ((NULL == arr) || (0 == number_of_arr) || (0 == arr_elem_size) || (NULL == compare_arr)) {
        return;
    }

    /* Heapify the input array in O(N) complexity */
    priority_queue_t* heap = heapify(NULL, arr, 0, arr_elem_size, number_of_arr, NULL, compare_arr, NULL, NULL);

    /* Rearrange input array data from heap structure in O(NlogN) complexity */
    for (size_t iter = 0; iter < number_of_arr; ++iter) {
        memcpy(arr + iter * arr_elem_size, pri_queue_top_pri(heap), arr_elem_size);
        pri_queue_pop(heap);
    }

    /* Free heap memory in O(N) complexity */
    free_priority_queue(heap);

    /* Overall complexity O(NlogN) */
}