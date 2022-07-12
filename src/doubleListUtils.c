/**
 * @file doubleListUtils.c
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

#include "./include/doubleListUtils.h"

/**
 * @brief Create a Double Linked List object. Allocation may fail if user
 * does not provide a compare and a print function, also in case if
 * heap memory is full function will return a NULL pointer
 * 
 * @param compareData pointer to a function to compare two sets of data
 * @param printData pointer to a function to print specific data
 * @param freeData pointer to a function to free the content of data
 * basic types like int, float, double, etc... do not need a free function
 * so you can pass a NULL pointer
 * @return dlinkedList* return a new dynamically allocated list or NULL if
 * allocation went wrong
 */
dlinkedList* dcreateLinkedList(int (*compareData)(const void *, const void *), void (*printData)(const void *), void (*freeData)(void *)) {
    // It is required for every linked list to have a compare and a print function
    // The free function is optional
    if (compareData == NULL || printData == NULL) {
        errno = EINVAL;
        perror("Compare or print functions undefined for linked list");
        return NULL;
    }

    // Allocate a new list on heap
    dlinkedList *newList = (dlinkedList *)malloc(sizeof(dlinkedList));

    // Check if new list was allocated
    if (newList) {
        // Set pointer functions in linked list class
        newList->compareData = compareData;
        newList->printData = printData;
        newList->freeData = freeData;

        // Initialize head, tail and size of new list
        newList->head = newList->tail = NULL;
        newList->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for list allocation");
    }

    // Return new allocated list or NULL
    return newList;
}

/**
 * @brief Create a Double Linked Node object. Creation of a
 * node will fail if the pointer to data is NULL or
 * heap memory is also full, in this case function will return a NULL
 * pointer
 * 
 * @param data pointer to address of a generic data
 * @param dataSize size of an element
 * @return TdlistNode* return a new allocated node object
 */
TdlistNode* dcreatelistNode(const void *data, size_t dataSize) {
    // It is required for data to be a valid pointer
    if (data == NULL)
        return NULL;

    // Allocate a new Node on heap
    TdlistNode *newNode = (TdlistNode *)malloc(sizeof(TdlistNode));

    // Check if new node was allocated
    if (newNode) {
        newNode->prev = newNode->next = NULL;

        // Allocate heap memory for data
        newNode->data = malloc(dataSize);

        // Check if 
        if (newNode->data)
            // Copy all bytes from data pointer
            // to memory allocated on heap
            memcpy(newNode->data, data, dataSize);
        else {
            errno = ENOMEM;
            perror("Not enough memory for node list allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    // Return a new created node or NULL
    return newNode;
}

/**
 * @brief Function prints all elements from beginning
 * to the end within the given list according to printData 
 * function provided by the user when list was created. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param list a double linked list objects
 */
void dlist_print_front(dlinkedList *list) {
    if (list) {
        // If list is empty, print []
        if (list->head == NULL)
            printf("[ ]");

        TdlistNode *iterator = list->head;

        // Print every node data
        while (iterator != NULL) {
            list->printData(iterator->data);
            iterator = iterator->next;
        }
    }
}

/**
 * @brief Function prints all elements from end
 * to the beginning within the given list according to printData 
 * function provided by the user when list was created. Function
 * may fail if list is not allocated in this case function
 * will not print anywith on output. In case if list is empty
 * then a set of paired square brakets will be printed on output.
 * 
 * @param list a double linked list objects
 */
void dlist_print_back(dlinkedList *list) {
    if (list) {
        // If list is empty, print []
        if (list->head == NULL)
            printf("[ ]");

        TdlistNode *iterator = list->tail;

        // Print every node data from back
        while (iterator != NULL) {
            list->printData(iterator->data);
            iterator = iterator->prev;
        }
    }
}

/**
 * @brief Function to free every byte of memory allocated for a specific
 * double linked list object. The function will iterate through all nodes and will
 * free the data content according to freeData function provided by user at
 * creation of linked list, however if no free function was provided it means
 * that data pointer does not contain any dinamically allocated elements.
 * 
 * @param list an allocated double linked list object. If list is not allocated
 * no operation will be needed
 */
void dlist_free_all(dlinkedList *list) {
    // Check if list needs to be deallocated
    if (list) {
        // Iterate through every node
        while (list->head != NULL) {
            TdlistNode *iterator = list->head;

            list->head = list->head->next;

            // Erase content of an element
            if (list->freeData != NULL && iterator->data != NULL)
                list->freeData(iterator->data);

            // Free data pointer
            if (iterator->data != NULL)
                free(iterator->data);

            // Set data pointer to NULL
            iterator->data = NULL;
            
            // Free node pointer
            if (iterator != NULL)
                free(iterator);

            // Set node pointer to NULL
            iterator = NULL;
        }

        // Free list
        free(list);

        // Set list pointer as NULL
        list = NULL;
    }
}

/**
 * @brief Function to check if a double linked list object
 * is empty or not. The function tests if head of list
 * is NULL in that case function will return true, otherwise
 * it will return false. A NULL list is also considered as an
 * empty list
 * 
 * @param list a double linked list object 
 * @return int true(1) if list is empty and false(0) if list is not
 * empty
 */
int is_dlist_empty(dlinkedList *list) {
    if (list == NULL || list->head == NULL)
        return 1;

    return 0;
}

/**
 * @brief Get the list size object. If list is not
 * allocated then function will return -1 value.
 * 
 * @param list a double linked list object
 * @return int -1 if list is not allocated or
 * list size
 */
int get_dlist_size(dlinkedList *list) {
    if (list == NULL)
        return -1;

    return list->size;
}

/**
 * @brief Get the list head object
 * 
 * @param list a double linked list object
 * @return TdlistNode* NULL if list is not allocated
 * or actual head of the list
 */
TdlistNode* get_dlist_head(dlinkedList *list) {
    if (list == NULL)
        return NULL;

    return list->head;
}

/**
 * @brief Get the list tail object
 * 
 * @param list a double linked list object
 * @return TdlistNode* NULL if list is not allocated
 * or actual tail of the list
 */
TdlistNode* get_dlist_tail(dlinkedList *list) {
    if (list == NULL)
        return NULL;

    return list->tail;
}

/**
 * @brief Function two swap data between two list nodes. If nodes
 * are NULL or the same then no operation will be executed. Function
 * will swap data pointers not node pointers. Function may fail if
 * list is not allocated
 * 
 * @param list a double linked list object
 * @param firstNode first double linked list node
 * @param secondNode second double linked list node
 */
void dlist_swap_data(dlinkedList *list, TdlistNode *firstNode, TdlistNode *secondNode) {
    // Check if list and input nodes are allocated
    if (list == NULL || firstNode == NULL || secondNode == NULL)
        return;

    // If nodes are the same then no swap is nedeed
    if (firstNode == secondNode)
        return;

    // Copy adress of first data pointer
    // and interchange data pointers
    void *temporar = firstNode->data;
    firstNode->data = secondNode->data;
    secondNode->data = temporar;
}

/**
 * @brief Function to change data of a specific double linked list node.
 * Base node data must be the same type as new data (size of evry data must
 * be the same). If data size between inputs are different than the function will fail
 * if new data size is bigger than the current one, however if new data size is smaller
 * than current one than function may have unknown behavior. Function may fail if instead
 * of baseNode pointer user sends a pointer(void *) to any different type of data.  
 * 
 * @param list a double linked list object
 * @param baseNode a double linked list node object to change its data
 * @param newData a pointer to new data
 * @param dataSize size of the new data
 * @return int function will return 1 if it fails and 0 otherwise
 */
int dlist_change_data(dlinkedList *list, TdlistNode *baseNode, const void *newData, size_t dataSize) {
    // Check if input is valid
    if (list == NULL || baseNode == NULL || newData == NULL)
        return 1;

    // Copy all bytes from new data to current data
    memmove(baseNode->data, newData, dataSize);

    return 0;
}

/**
 * @brief Function to insert an element to the end of the list
 * 
 * @param list a double linked list object
 * @param data a pointer for data to insert in list
 * @param dataSize the size of current data type
 * @return int 1 if function fails or 0 if insertion was successfully
 */
int dlist_insert(dlinkedList *list, const void *data, size_t dataSize) {
    // Check if list and data are valid
    if (list == NULL || data == NULL)
        return 1;

    // Create a new linked list node
    TdlistNode *newNode = dcreatelistNode(data, dataSize);

    // Check if node was allocated
    if (newNode == NULL)
        return 1;

    // Insert item in list
    if (list->head == NULL) {
        // List is empty so update head
        // and tail of the list
        list->head = newNode;
        list->tail = newNode;
    } else {
        // Insert element at the end of the list
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }

    // Increase size of the list
    ++(list->size);

    // Insertion went successfully
    return 0;
}

/**
 * @brief Function to insert an element in order in the list.
 * Function will find the position of the new elements according
 * to compareData function provided at the creation of the list
 * 
 * @param list a double linked list object
 * @param data a pointer for data to insert in list
 * @param dataSize the size of current data type
 * @return int 1 if function fails or 0 if insertion was successfully
 */
int dlist_insert_order(dlinkedList *list, const void *data, size_t dataSize) {
    // Check if list and data are valid
    if (list == NULL || data == NULL)
        return 1;

    // Create a new linked list node
    TdlistNode *newNode = dcreatelistNode(data, dataSize);

    // Check if node was allocated
    if (newNode == NULL)
        return 1;

    // Insert element into the list
    if (list->head == NULL) {
        // List is empty so update
        // head and tail of the list
        list->head = newNode;
        list->tail = newNode;
    } else {
        TdlistNode *iterator = list->head;

        // Find the position for the new element
        while (iterator != NULL && list->compareData(newNode->data, iterator->data) > 0)
            iterator = iterator->next;

        if (iterator == NULL) {
            // Insert element at the end of list
            list->tail->next = newNode;
            newNode->prev = list->tail;
            list->tail = newNode;
        } else if (iterator->prev == NULL) {
            // New node must be inserted at the begining
            // of the double linked list
            newNode->next = list->head;
            list->head->prev = newNode;
            list->head = newNode;
        } else {
            // Insert element at the new position
            newNode->next = iterator;
            newNode->prev = iterator->prev;
            iterator->prev->next = newNode;
            iterator->prev = newNode;
        }
    }

    // Increase size of the list
    ++(list->size);

    // Insertion went successfully
    return 0;
}

/**
 * @brief Function to insert an element in front of the list.
 * 
 * @param list a double linked list object
 * @param data a pointer for data to insert in list
 * @param dataSize the size of current data type
 * @return int 1 if function fails or 0 if insertion was successfully
 */
int dlist_insert_front(dlinkedList *list, const void *data, size_t dataSize) {
    // Check if list and data are valid
    if (list == NULL || data == NULL)
        return 1;

    // Create a new linked list node
    TdlistNode *newNode = dcreatelistNode(data, dataSize);

    // Check if node was allocated
    if (newNode == NULL)
        return 1;

    // Insert new element at the beginning
    // of the linked list
    if (list->head == NULL) {
        // List is empty so update
        // head and tail of the list
        list->head = newNode;
        list->tail = newNode;
    } else {
        // Link new node to the list
        // and update new head
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }

    // Increase list size
    ++(list->size);

    // Insertion went successfully
    return 0;
}

/**
 * @brief Function to insert an element at a specified index in the list.
 * If index is bigger than current list size than element will be inserted
 * at the end of the list.
 * 
 * @param list a double linked list object
 * @param data a pointer for data to insert in list
 * @param dataSize the size of current data type
 * @return int 1 if function fails or 0 if insertion was successfully
 */
int dlist_insert_index(dlinkedList *list, const void *data, size_t dataSize, size_t dataIndex) {
    // Check if list and data are valid
    if (list == NULL || data == NULL)
        return 1;

    // Insert element at the end of the list
    if (dataIndex >= list->size)
        return dlist_insert(list, data, dataSize);

    // Insert element at the beginning of the list
    if (dataIndex == 0)
        return dlist_insert_front(list, data, dataSize);

    // Create a new linked list node
    TdlistNode *newNode = dcreatelistNode(data, dataSize);

    // Check if new node was allocated
    if (newNode == NULL)
        return 1;

    TdlistNode *iterator = list->head;

    // Find node that point on the
    // specified index node
    while (--dataIndex)
        iterator = iterator->next;

    // Insert node and update links
    newNode->next = iterator->next;
    newNode->prev = iterator;
    newNode->next->prev = newNode;
    iterator->next = newNode;

    // Increase list size
    ++(list->size);

    // Insertion went successfully
    return 0;
}

/**
 * @brief Function to find a node from a list from
 * a given index. Function will fail if index is bigger than
 * current size of list or list is not allocated
 * 
 * @param list a double linked list object
 * @param dataIndex index to pick node from
 * @return TdlistNode* double linked list node from list at specified
 * index
 */
TdlistNode* dlist_find_index(dlinkedList *list, size_t dataIndex) {
    // Check if list and index are valid
    if (list == NULL || dataIndex >= list->size)
        return NULL;

    // Return last element
    if (dataIndex == list->size - 1)
        return list->tail;

    TdlistNode *iterator = list->head;

    // Iterate in list until hit indexed node
    while (dataIndex--)
        iterator = iterator->next;

    // Return a pointe to found node
    return iterator;
}

/**
 * @brief Function to find node that contains
 * specific data provided by user. It uses compareData
 * provided by user at the creation of the double linked list.
 * 
 * @param list a double linked list object
 * @param data pointer to a typed data
 * @return TdlistNode* NULL if data is not found or a pointer
 * to a double linked list node containing given data
 */
TdlistNode* dlist_find_data(dlinkedList *list, const void *data) {
    // Check if list and data are valid and
    // check if list is not empty
    if (list == NULL || list->head == NULL || data == NULL)
        return NULL;

    TdlistNode *iterator = list->head;

    // Find node
    while (iterator != NULL && list->compareData(iterator->data, data) != 0)
        iterator = iterator->next;

    // Return a pointer to node or NULL
    return iterator;
}

/**
 * @brief Function to delete a node based on a value. Program will
 * recieve a list and a pointer to data that user wants to be deleted.
 * However data pointer has to be valid and to exist in the current list
 * (If you are not sure that data exists you should not call dlist_find_data because
 * delete function will find it by itself and in case it does not exist it will return 1)
 * 
 * @param list a double linked list object
 * @param data a pointer to a typed data to be removed 
 * @return int 1 if function failed (list is NULL, list is empty, no valid data pointer
 * or element is not in the list) and 0 if function passed successfully
 */
int dlist_delete_data(dlinkedList *list, void *data) {
    // Check if list is allocated and it is not empty
    // Check if data pointer is valid
    if (list == NULL || list->head == NULL || data == NULL)
        return 1;

    TdlistNode *iterator = list->head;

    // Find list data associated with data pointer
    while (iterator != NULL && list->compareData(iterator->data, data) != 0)
        iterator = iterator->next;

    // List does not contain such element
    if (iterator == NULL)
        return 1;

    // Update head if necessary or previous link
    if (iterator->prev == NULL)
        list->head = iterator->next;
    else
        iterator->prev->next = iterator->next;

    // Update tail if necessary or next link
    if (iterator->next == NULL)
        list->tail = iterator->prev;
    else
        iterator->next->prev = iterator->prev;

    // Free content of data
    if (list->freeData)
        list->freeData(iterator->data);

    // Free data pointer and set to NULL
    free(iterator->data);
    iterator->data = NULL;

    // Free node pointer and set to NULL
    free(iterator);
    iterator = NULL;

    // Decrease list size
    --(list->size);

    // Deletion went successfully
    return 0;
}

/**
 * @brief Function to delete a node based on an index. Program will
 * recieve a list and a index from which element will be erased. If
 * dataIndex is bigger than actual size of the list then function will
 * fail its execution and will return 1. It is necessary for list to be
 * allocated and not be be empty (in this case 1 will be returned).
 * 
 * @param list a double linked list object
 * @param dataIndex node index in the list to be removed starts from 0
 * @return int 1 if function fails and 0 if deletion went successfully
 */
int dlist_delete_index(dlinkedList *list, size_t dataIndex) {
    // Check if list is allocated and it is not empty
    // Check if data pointer is valid
    if (list == NULL || list->head == NULL || dataIndex >= list->size)
        return 1;

    TdlistNode *iterator = list->head;

    // Find node from index
    while (dataIndex--)
        iterator = iterator->next;

    // Update head if necessary or previous link
    if (iterator->prev == NULL)
        list->head = iterator->next;
    else
        iterator->prev->next = iterator->next;

    // Update tail if necessary or next link
    if (iterator->next == NULL)
        list->tail = iterator->prev;
    else
        iterator->next->prev = iterator->prev;

    // Free content of data
    if (list->freeData)
        list->freeData(iterator->data);

    // Free data pointer and set to NULL
    free(iterator->data);
    iterator->data = NULL;

    // Free node pointer and set to NULL
    free(iterator);
    iterator = NULL;

    // Decrease list size
    --(list->size);

    // Deletion went successfully
    return 0;
}

/**
 * @brief Function to erase a set of nodes from range [leftIndex; rightIndex]
 * If leftIndex is greater than rightIndex that they will be swapped. If rightIndex
 * is bigger than actual size of the list rightIndex will be updated to the end of
 * the list. If both left and right index are bigger than actual list size than
 * the last element from double linked object will be removed.
 * 
 * @param list a double linked list object
 * @param leftIndex left index to start deletion
 * @param rightIndex right index to finish deletion
 * @return int 1 if function fails (list is empty or not allocated) and
 * 0 if deletion of the range went successfully
 */
int dlist_erase(dlinkedList *list, size_t leftIndex, size_t rightIndex) {
    // Check if list is allocated and it is not empty
    if (list == NULL || list->head == NULL)
        return 1;

    // Check if boundaries are set right
    // Swap if necessary
    if (leftIndex > rightIndex) {
        int temporar = leftIndex;
        leftIndex = rightIndex;
        rightIndex = temporar;
    }

    // Recalibrate left index if needed
    if (leftIndex >= list->size)
        leftIndex = list->size - 1;

    // Recalibrate right index if needed
    if (rightIndex >= list->size)
        rightIndex = list->size - 1;

    TdlistNode *iterator = list->head;

    // Compute number of nodes from range
    int deleteNumber = rightIndex - leftIndex + 1;

    // Update iterator pointer
    // to beginning of list deletion
    while (leftIndex--)
        iterator = iterator->next;

    // Deacrise list size
    list->size -= deleteNumber;

    // Delete every number from given range
    while (deleteNumber--) {
        // Update head if necessary or previous link
        if (iterator->prev == NULL)
            list->head = iterator->next;
        else
            iterator->prev->next = iterator->next;

        // Update tail if necessary or next link
        if (iterator->next == NULL)
            list->tail = iterator->prev;
        else
            iterator->next->prev = iterator->prev;

        // Save a pointer to memory to delete
        TdlistNode *delete_node = iterator;

        // Update iterator pointer to next cell
        iterator = iterator->next;

        // Free content of data
        if (list->freeData)
            list->freeData(delete_node->data);

        // Free data pointer and set to NULL
        free(delete_node->data);
        delete_node->data = NULL;

        // Free node pointer and set to NULL
        free(delete_node);
        delete_node = NULL;
    }

    // Deletion went successfully
    return 0;
}

/**
 * @brief Function to filter a given double linked list object. User
 * has to provide a function that return true(1) or false(0). If
 * filter function return 1 for an item then it will be added in a
 * new linked list, otherwise item will not be inserted. If no
 * element was inserted in the new double linked list than the list will
 * be automatically erased from memory
 * 
 * @param list a double linked list object
 * @param filterFunction a pointer to a filter function
 * @param dataSize size of a single element
 * @return dlinkedList* a filtered linked list object with smaller
 * or equal size of the original linked list object
 */
dlinkedList* dlist_filter(dlinkedList *list, int (*filterFunction)(const void *), size_t dataSize) {
    // Check if input is valid
    // Filter function has to be different from NULL pointer
    if (list == NULL || list->head == NULL || filterFunction == NULL)
        return NULL;

    // Create a new double linked list object
    dlinkedList *filterList = dcreateLinkedList(list->compareData, list->printData, list->freeData);

    // Check if list was created
    if (filterList) {
        TdlistNode *iterator = list->head;

        // Iterate through all list nodes
        while (iterator != NULL) {
            // Check if item is filtered or not
            if (filterFunction(iterator->data) == 1)
                dlist_insert(filterList, iterator->data, dataSize);

            iterator = iterator->next;
        }

        // If no element was added to list than free
        // space of the new double linked list and return NULL
        if (filterList->head == NULL) {
            dlist_free_all(filterList);
            filterList = NULL;
        }
    }

    // Return filtered list or NULL
    return filterList;
}

/**
 * @brief Function will map every single element into a new
 * element according to mapFunction that is provided by the user
 * mapFunction has to return a void* type. Function may fail
 * if elements are of different size and different type. This
 * function modifies list in-place
 * 
 * @param list a double linked list object
 * @param mapFunction a pointer to a mapping function
 * @param dataSize size of a single element
 */
void dlist_map(dlinkedList *list, void* (*mapFunction)(void *), size_t dataSize) {
    // Check if list is allocated and is not empty
    // Check if user provided a valid map function
    if (list == NULL || list->head == NULL || mapFunction == NULL)
        return;

    TdlistNode *iterator = list->head;

    // Iterate through every element in the list
    // and map every single object
    while (iterator != NULL) {
        // Copy mapped bytes in data bytes
        memmove(iterator->data, mapFunction(iterator->data), dataSize);
        iterator = iterator->next;
    }
}