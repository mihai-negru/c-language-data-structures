/**
 * @file scl_list.c
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

#include "./include/scl_list.h"

/**
 * @brief Create a Linked List object. Allocation may fail if user
 * does not provide a compare and a print function, also in case if
 * heap memory is full function will return a `NULL` pointer
 * 
 * @param cmp pointer to a function to compare two sets of data
 * @param frd pointer to a function to free the content of data
 * basic types like int, float, double, etc... do not need a free function
 * so you can pass a NULL pointer
 * @param data_size length in bytes of the data data type
 * @return list_t* return a new dynamically allocated list or `NULL` if
 * allocation went wrong
 */
list_t* create_list(compare_func cmp, free_func frd, size_t data_size) {
    /*
     * It is required for every linked list to have a compare function
     * The free function is optional
     */
    if (NULL == cmp) {
        errno = EINVAL;
        perror("Compare function undefined for linked list");
        return NULL;
    }

    if (0 == data_size) {
        errno = EINVAL;
        perror("Data size at creation is zero");
        return NULL;
    }

    /* Allocate a new list on heap */
    list_t *new_list = malloc(sizeof(*new_list));

    /* Check if new list was allocated */
    if (NULL != new_list) {

        /* Set pointer functions in linked list class */
        new_list->cmp = cmp;
        new_list->frd = frd;

        /* Initialize head, tail and size of new list */
        new_list->head = new_list->tail = NULL;
        new_list->data_size = data_size;
        new_list->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for list allocation");
    }

    /* Return new allocated list or `NULL` */
    return new_list;
}

/**
 * @brief Create a Linked Node object. Creation of a
 * node will fail if the pointer to data is `NULL` or
 * heap memory is also full, in this case function will return a `NULL`
 * pointer
 * 
 * @param list an allocated linked list object
 * @param data pointer to address of a generic data
 * @return list_node_t* return a new allocated node object
 */
static list_node_t* create_list_node(const list_t * const __restrict__ list, const void * __restrict__ data) {
    /* It is required for data to be a valid pointer */
    if (NULL == data) {
        return NULL;
    }

    /* Allocate a new Node on heap */
    list_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if new node was allocated */
    if (NULL != new_node) {
        new_node->next = NULL;

        /* Allocate heap memory for data */
        new_node->data = malloc(list->data_size);

        /* Check if data pointer was allocated*/
        if (NULL != new_node->data) {

            /*
             * Copy all bytes from data pointer
             * to memory allocated on heap
             */
            memcpy(new_node->data, data, list->data_size);
        } else {
            free(new_node);
            new_node = NULL;

            errno = ENOMEM;
            perror("Not enough memory for data node list allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    /* Return a new created node or `NULL` */
    return new_node;
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * linked list object. The function will iterate through all nodes and will
 * free the data content according to frd function provided by user at
 * creation of linked list, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param list an allocated linked list object. If list is not allocated
 * no operation will be needed
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_list(list_t * const __restrict__ list) {
    /* Check if list needs to be deallocated */
    if (NULL != list) {

        /* Iterate through every node */
        while (NULL != list->head) {
            list_node_t *iterator = list->head;

            list->head = list->head->next;

            /* Erase content of an element */
            if ((NULL != list->frd) && (NULL != iterator->data)) {
                list->frd(iterator->data);
            }

            /* Free data pointer */
            if (NULL != iterator->data) {
                free(iterator->data);
            }

            /* Set data pointer to `NULL` */
            iterator->data = NULL;
            
            /* Free node pointer */
            if (NULL != iterator) {
                free(iterator);
            }

            /* Set node pointer to `NULL` */
            iterator = NULL;
        }

        /* Free list */
        free(list);

        return SCL_OK;
    }

    return SCL_NULL_LIST;
}

/**
 * @brief Function to check if a linked list object
 * is empty or not. The function tests if head of list
 * is `NULL` in that case function will return true, otherwise
 * it will return false. A `NULL` list is also considered as an
 * empty list
 * 
 * @param list a linked list object 
 * @return uint8_t true(1) if list is empty and false(0) if list is not
 * empty
 */
uint8_t is_list_empty(const list_t * const __restrict__ list) {
    if ((NULL == list) || (NULL == list->head)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Get the list size object. If list is not
 * allocated then function will return SIZE_MAX value.
 * 
 * @param list a linked list object
 * @return size_t SIZE_MAX if list is not allocated or
 * list size
 */
size_t get_list_size(const list_t * const __restrict__ list) {
    if (NULL == list) {
        return SIZE_MAX;
    }

    return list->size;
}

/**
 * @brief Get the list head object
 * 
 * @param list a linked list object
 * @return const void* `NULL` if list is not allocated
 * or actual head data of the list
 */
const void* get_list_head(const list_t * const __restrict__ list) {
    if ((NULL == list) || (NULL == list->head)) {
        return NULL;
    }

    return list->head->data;
}

/**
 * @brief Get the list tail object
 * 
 * @param list a linked list object
 * @return const void* `NULL` if list is not allocated
 * or actual tail data of the list
 */
const void* get_list_tail(const list_t * const __restrict__ list) {
    if ((NULL == list) || (NULL == list->tail)) {
        return NULL;
    }

    return list->tail->data;
}

/**
 * @brief Function to find node that contains
 * specific data provided by user. It uses cmp function
 * provided by user at the creation of the linked list.
 * 
 * @param list a linked list object
 * @param data pointer to a typed data
 * @return list_node_t* `NULL` if data is not found or a pointer
 * to a double linked list node data containing given data
 */
static list_node_t* list_find_node(const list_t * const __restrict__ list, const void * const __restrict__ data) {
    /*
     * Check if list and data are valid and
     * check if list is not empty
     */
    if ((NULL == list) || (NULL == list->head) || (NULL == data)) {
        return NULL;
    }

    list_node_t *iterator = list->head;

    /* Find node */
    while ((NULL != iterator) && (list->cmp(iterator->data, data) != 0)) {
        iterator = iterator->next;
    }

    return iterator;
}


/**
 * @brief Function two swap data between two list nodes. If nodes
 * are `NUL`L or the same then no operation will be executed. Function
 * will swap data pointers not node pointers. Function may fail if
 * list is not allocated
 * 
 * @param list an allocated double linked list object
 * @param first_node pointer to value of the first data
 * @param second_node pointer to value of the second data
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_swap_data(const list_t * const __restrict__ list, const void * const __restrict__ first_data, const void * const __restrict__ second_data) {
    /* Check if list and input nodes are allocated */
    if ((NULL == first_data) || (NULL == second_data)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    list_node_t * const list_first = list_find_node(list, first_data);
    list_node_t * const list_second = list_find_node(list, second_data);

    if ((NULL == list_first) && (NULL == list_second)) {
        return SCL_DATA_NOT_FOUND;
    }

    /* If nodes are the same then no swap is nedeed */
    if (list_first == list_second) {
        return SCL_SWAP_SAME_DATA;
    }

    uint8_t *list_first_data = list_first->data;
    uint8_t *list_second_data = list_second->data;

    size_t data_size = list->data_size;

    /*
     * Copy adsress value of first data pointer
     * and interchange data pointers
     */
    while (data_size-- > 0) {
        uint8_t temp = *list_first_data;
        *list_first_data++ = *list_second_data;
        *list_second_data++ = temp;
    }

    return SCL_OK;
}

/**
 * @brief Function to change data of a specific linked list node.  
 * 
 * @param list an allocated double linked list object
 * @param base_node pointer to value of the base data
 * @param new_data a pointer to value of the new data to replace
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_change_data(const list_t * const __restrict__ list, const void * const __restrict__ base_data, const void * __restrict__ new_data) {
    /* Check if input is valid */
    if (NULL == new_data) {
        return SCL_CANNOT_CHANGE_DATA;
    }

    list_node_t * const list_base = list_find_node(list, base_data);

    if (NULL == list_base) {
        return SCL_DATA_NOT_FOUND;
    }

    /* Copy all bytes from new data to current data */
    memcpy(list_base->data, new_data, list->data_size);

    return SCL_OK;
}

/**
 * @brief Function to insert an element to the end of the list.
 * 
 * @param list a linked list object
 * @param data a pointer for data to insert in list
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_insert(list_t * const __restrict__ list, const void * __restrict__ data) {
    /* Check if list and data are valid */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Create a new linked list node */
    list_node_t *new_node = create_list_node(list, data);

    /* Check if node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Insert item in list */
    if (NULL == list->head) {

        /*
         * List is empty so update head
         * and tail of the list
         */
        list->head = new_node;
        list->tail = new_node;
    } else {

        /* Insert element at the end of the list */
        list->tail->next = new_node;
        list->tail = new_node;
    }

    /* Increase size of the list */
    ++(list->size);

    /* Insertion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to insert an element in order in the list.
 * Function will find the position of the new elements according
 * to cmp function provided at the creation of the list
 * 
 * @param list a linked list object
 * @param data a pointer for data to insert in list
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_insert_order(list_t * const __restrict__ list, const void * __restrict__ data) {
    /* Check if list and data are valid */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Create a new linked list node */
    list_node_t *new_node = create_list_node(list, data);

    /* Check if node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Insert element into the list */
    if (NULL == list->head) {

        /*
         * List is empty so update
         * head and tail of the list
         */
        list->head = new_node;
        list->tail = new_node;
    } else {
        list_node_t *iterator = list->head;
        list_node_t *prev_iterator = NULL;

        /* Find the position for the new element */
        while ((NULL != iterator) && (list->cmp(new_node->data, iterator->data) > 0)) {
            prev_iterator = iterator;
            iterator = iterator->next;
        }

        if (NULL == prev_iterator) {

            /*
             * New node must be inserted at the begining
             * of the linked list
             */
            new_node->next = list->head;
            list->head = new_node;
        } else {

            /* Insert element at the new position */
            new_node->next = iterator;
            prev_iterator->next = new_node;

            if (NULL == iterator) {
                list->tail = new_node;
            }
        }
    }

    /* Increase size of the list */
    ++(list->size);

    /* Insertion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to insert an element in front of the list.
 * 
 * @param list a linked list object
 * @param data a pointer for data to insert in list
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_insert_front(list_t * const __restrict__ list, const void * __restrict__ data) {
    /* Check if list and data are valid */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Create a new linked list node */
    list_node_t *new_node = create_list_node(list, data);

    /* Check if node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /*
     * Insert new element at the beginning
     * of the linked list
     */
    if (NULL == list->head) {

        /*
         * List is empty so update
         * head and tail of the list
         */
        list->head = new_node;
        list->tail = new_node;
    } else {

        /*
         * Link new node to the list
         * and update new head
         */
        new_node->next = list->head;
        list->head = new_node;
    }

    /* Increase list size */
    ++(list->size);

    /* Insertion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to insert an element at a specified index in the list.
 * If index is bigger than current list size than element will be inserted
 * at the end of the list.
 * 
 * @param list a linked list object
 * @param data a pointer for data to insert in list
 * @param data_index the index to insert an element into linked list
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_insert_index(list_t * const __restrict__ list, const void * __restrict__ data, size_t data_index) {
    /* Check if list and data are valid */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }


    /* Insert element at the end of the list */
    if (data_index >= list->size)
        return list_insert(list, data);

    /* Insert element at the beginning of the list */
    if (data_index == 0)
        return list_insert_front(list, data);

    /* Create a new linked list node */
    list_node_t *new_node = create_list_node(list, data);

    /* Check if new node was allocated */
    if (NULL == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    list_node_t *iterator = list->head;

    /*
     * Find node that point on the
     * specified index node
     */
    while (--data_index) {
        iterator = iterator->next;
    }

    /* Insert node and update links */
    new_node->next = iterator->next;
    iterator->next = new_node;

    if (NULL == new_node->next) {
        list->head = new_node;
    }

    /* Increase list size */
    ++(list->size);

    /* Insertion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to find a node from a list from
 * a given index. Function will fail if index is bigger than
 * current size of list or list is not allocated
 * 
 * @param list a linked list object
 * @param data_index index to pick node from
 * @return const void* linked list node data from list at specified
 * index
 */
const void* list_find_index(const list_t * const __restrict__ list, size_t data_index) {
    /* Check if list and index are valid */
    if ((NULL == list) || (data_index >= list->size)) {
        return NULL;
    }

    /* Return last element */
    if (data_index == (list->size - 1)) {
        if (NULL != list->tail) {
            return list->tail->data;
        }
    
        return NULL;
    }

    const list_node_t *iterator = list->head;

    /* Iterate in list until hit indexed node */
    while (data_index--) {
        iterator = iterator->next;
    }

    /* Return a pointe to found node */
    if (NULL != iterator) {
        return iterator->data;
    }
    
    /* Data was not found */
    return NULL;
}

/**
 * @brief Function to find node that contains
 * specific data provided by user. It uses cmp function
 * provided by user at the creation of the linked list.
 * 
 * @param list a linked list object
 * @param data pointer to a typed data
 * @return const void* `NULL` if data is not found or a pointer
 * to a linked list node data containing given data
 */
const void* list_find_data(const list_t * const __restrict__ list, const void * const __restrict__ data) {
    /*
     * Check if list and data are valid and
     * check if list is not empty
     */
    if ((NULL == list) || (NULL == list->head) || (NULL == data)) {
        return NULL;
    }

    const list_node_t *iterator = list->head;

    /* Find node */
    while ((NULL != iterator) && (list->cmp(iterator->data, data) != 0)) {
        iterator = iterator->next;
    }

    /* Return a pointer to node or `NULL` */
    if (NULL != iterator) {
        return iterator->data;
    }

    return NULL;
}

/**
 * @brief Function to delete a node based on a value. Program will
 * recieve a list and a pointer to data that user wants to be deleted.
 * However data pointer has to be valid and to exist in the current list
 * (If you are not sure that data exists you should not call list_find_data because
 * delete function will find it by itself and in case it does not exist it will return an error)
 * 
 * @param list a linked list object
 * @param data a pointer to a typed data to be removed 
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_delete_data(list_t * const __restrict__ list, const void * const __restrict__ data) {
    /*
     * Check if list is allocated and it is not empty
     * Check if data pointer is valid
     */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == list->head) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    list_node_t *iterator = list->head;
    list_node_t *prev_iterator = NULL;

    /* Find list data associated with data pointer */
    while ((NULL != iterator) && (list->cmp(iterator->data, data) != 0)) {
        prev_iterator = iterator;
        iterator = iterator->next;
    }

    /* List does not contain such element */
    if (NULL == iterator) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    if (NULL == prev_iterator) {

        /* Element is the head of the list */
        list->head = list->head->next;
    } else {

        /* Update link with next node */
        prev_iterator->next = iterator->next;

        /* Update tail if necessary */
        if (NULL == iterator->next) {
            list->tail = prev_iterator;
        }
    }

    /* Free content of data */
    if ((NULL != list->frd) && (NULL != iterator->data)) {
        list->frd(iterator->data);
    }

    /* Free data pointer and set to `NULL` */
    if (NULL != iterator->data) {
        free(iterator->data);
    }

    iterator->data = NULL;

    /* Free node pointer and set to `NULL` */
    if (NULL != iterator) {
        free(iterator);
    }

    iterator = NULL;

    /* Decrease list size */
    --(list->size);

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to delete a node based on an index. Program will
 * recieve a list and a index from which element will be erased. If
 * data_index is bigger than actual size of the list then function will
 * fail its execution and will return an error. It is necessary for list to be
 * allocated and not be be empty.
 * 
 * @param list a linked list object
 * @param data_index node index in the list to be removed starts from 0
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_delete_index(list_t * const __restrict__ list, size_t data_index) {
    /*
     * Check if list is allocated and it is not empty
     * Check if data pointer is valid
     */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == list->head) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    if (data_index >= list->size) {
        return SCL_INDEX_OVERFLOWS_SIZE;
    }

    list_node_t *iterator = list->head;
    list_node_t *prev_iterator = NULL;

    /* Find node from index */
    while (data_index--) {
        prev_iterator = iterator;
        iterator = iterator->next;
    }

    if (NULL == prev_iterator) {

        /* Removing node is current head */
        list->head = list->head->next;
    } else {

        /* Update links within the nodes */
        prev_iterator->next = iterator->next;

        /* Update list tail if necessery */
        if (NULL == iterator->next) {
            list->tail = prev_iterator;
        }
    }

    /* Free content of data */
    if ((NULL != list->frd) && (NULL != iterator->data)) {
        list->frd(iterator->data);
    }

    /* Free data pointer and set to `NULL` */
    if (NULL != iterator->data) {
        free(iterator->data);
    }

    iterator->data = NULL;

    /* Free node pointer and set to `NULL` */
    if (NULL != iterator) {
        free(iterator);
    }

    iterator = NULL;

    /* Deacrise list size */
    --(list->size);

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to erase a set of nodes from range [left_index; right_index]
 * If left_index is greater than right_index that they will be swapped. If right_index
 * is bigger than actual size of the list right_index will be updated to the end of
 * the list. If both left and right index are bigger than actual list size than
 * the last element from linked object will be removed.
 * 
 * @param list a linked list object
 * @param left_index left index to start deletion
 * @param right_index right index to finish deletion
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_erase(list_t * const __restrict__ list, size_t left_index, size_t right_index) {
    /* Check if list is allocated and it is not empty */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == list->head) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    /*
     * Check if boundaries are set right
     * Swap if necessary
     */
    if (left_index > right_index) {
        size_t temp = left_index;
        left_index = right_index;
        right_index = temp;
    }

    /* Recalibrate left index if needed */
    if (left_index >= list->size) {
        left_index = list->size - 1;
    }

    /* Recalibrate right index if needed */
    if (right_index >= list->size) {
        right_index = list->size - 1;
    }

    list_node_t *iterator = list->head;
    list_node_t *prev_iterator = NULL;

    /* Compute number of nodes from range */
    size_t delete_num = right_index - left_index + 1;

    /*
     * Update iterator and prevIterator
     * pointer to beginning of list deletion
     */
    while (left_index--) {
        prev_iterator = iterator;
        iterator = iterator->next;
    }

    /* Decrease list size */
    list->size -= delete_num;

    /* Delete every number from given range */
    while (delete_num--) {
        if (NULL == prev_iterator) {

            /* Check if removed node is head */
            list->head = list->head->next;
        } else {

            /* Update nodes links */
            prev_iterator->next = iterator->next;

            /* Update list tail if neccessary */
            if (NULL == iterator->next) {
                list->tail = prev_iterator;
            }
        }

        /* Free content of data */
        if ((NULL != list->frd) && (NULL != iterator->data)) {
            list->frd(iterator->data);
        }

        /* Free data pointer and set to `NULL` */
        if (NULL != iterator->data) {
            free(iterator->data);
        }

        iterator->data = NULL;

        /* Free node pointer and set to `NULL` */
        if (NULL != iterator) {
            free(iterator);
        }

        iterator = NULL;

        /* Remove next node */
        if (NULL == prev_iterator) {
            iterator = list->head;
        } else {
            iterator = prev_iterator->next;
        }
    }

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to filter a given linked list object. User
 * has to provide a function that return true(1) or false(0). If
 * filter function return 1 for an item then it will be added in a
 * new linked list, otherwise item will not be inserted. If no
 * element was inserted in the new linked list than the list will
 * be automatically erased from memory
 * 
 * @param list a linked list object
 * @param filter a pointer to a filter function
 * @return list_t* a filtered linked list object with smaller
 * or equal size of the original linked list object
 */
list_t* list_filter(const list_t * const __restrict__ list, filter_func filter) {
    /*
     * Check if input is valid
     * Filter function has to be different from NULL pointer
     */
    if ((NULL == list) || (NULL == list->head) || (NULL == filter)) {
        return NULL;
    }

    /* Create a new linked list object */
    list_t *filter_list = create_list(list->cmp, list->frd, list->data_size);

    /* Check if list was created */
    if (NULL != filter_list) {
        const list_node_t *iterator = list->head;

        /* Iterate through all list nodes */
        while (NULL != iterator) {

            /* Check if item is filtered or not */
            if ((NULL != iterator->data) && (1 == filter(iterator->data))) {
                list_insert(filter_list, iterator->data);
            }

            iterator = iterator->next;
        }

        /*
         * If no element was added to list than free
         * space of the new linked list and return NULL
         */
        if (NULL == filter_list->head) {
            free_list(filter_list);
            filter_list = NULL;
        }
    }

    /* Return filtered list or `NULL` */
    return filter_list;
}

/**
 * @brief Function to traverse all list and
 * do action on all data nodes.
 * 
 * @param list a linked list object
 * @param action a pointer to an action function(can be also a mapping func)
 * @return scl_error_t enum object for handling errors
 */
scl_error_t list_traverse(const list_t * const __restrict__ list, action_func action) {
    /*
     * Check if list is allocated and is not empty
     * Check if user provided a valid map function
     */
    if (NULL == list) {
        return SCL_NULL_DLIST;
    }

    if (NULL == list->head) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == list->head) {
        printf("[ ]\n");
    } else {
        const list_node_t *iterator = list->head;
    
        while (NULL != iterator) {

            /* Call the action function */
            if (NULL != iterator->data) {
                action(iterator->data);
            }

            iterator = iterator->next;
        }
    }

    return SCL_OK;
}
