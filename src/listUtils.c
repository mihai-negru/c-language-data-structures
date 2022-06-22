/**
 * @file listUtils.c
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

#include "./include/listUtils.h"

linkedList* createLinkedList(int (*compareData)(void *, void *), void (*printData)(void *), void (*freeData)(void *)) {
    if (compareData == NULL || printData == NULL) {
        errno = EINVAL;
        perror("Compare or print functions undefined for linked list");
        return NULL;
    }

    linkedList *newList = (linkedList *)malloc(sizeof(linkedList));

    if (newList) {
        newList->compareData = compareData;
        newList->printData = printData;
        newList->freeData = freeData;
        newList->head = newList->tail = NULL;
        newList->size = 0;
    } else {
        errno = ENOMEM;
        perror("Not enough memory for list allocation");
    }

    return newList;
}

TlistNode* createlistNode(void *data, size_t dataSize) {
    if (data == NULL || dataSize <= 0)
        return NULL;

    TlistNode *newNode = (TlistNode *)malloc(sizeof(TlistNode));

    if (newNode) {
        newNode->next = NULL;
        newNode->data = malloc(dataSize);
        memcpy(newNode->data, data, dataSize);
    } else {
        errno = ENOMEM;
        perror("Not enough memory for node list allocation");
    }

    return newNode;
}

void list_print(linkedList *list) {
    if (list) {
        TlistNode *iterator = list->head;

        while (iterator != NULL) {
            list->printData(iterator->data);
            iterator = iterator->next;
        }
    }
}

void list_free_all(linkedList *list) {
    if (list) {
        while (list->head != NULL) {
            TlistNode *iterator = list->head;

            list->head = list->head->next;

            if (list->freeData != NULL && iterator->data != NULL)
                list->freeData(iterator->data);

            if (iterator->data != NULL) {
                free(iterator->data);
                iterator->data = NULL;
            }
            
            if (iterator != NULL) {
                free(iterator);
                iterator = NULL;
            }
        }

        free(list);
    }
}

int is_list_empty(linkedList *list) {
    if (list == NULL || list->head == NULL)
        return 1;

    return 0;
}

int get_list_size(linkedList *list) {
    if (list == NULL)
        return -1;

    return list->size;
}

TlistNode* get_list_head(linkedList *list) {
    if (list == NULL)
        return NULL;

    return list->head;
}

TlistNode* get_list_tail(linkedList *list) {
    if (list == NULL)
        return NULL;

    return list->tail;
}

void list_swap_data(linkedList *list, TlistNode *firstNode, TlistNode *secondNode) {
    if (list == NULL || firstNode == NULL || secondNode == NULL)
        return;

    if (firstNode == secondNode)
        return;

    void *temporar = firstNode->data;
    firstNode->data = secondNode->data;
    secondNode->data = temporar;
}

int list_change_data(linkedList *list, TlistNode *baseNode, void *newData, size_t dataSize) {
    if (list == NULL || baseNode == NULL || newData == NULL)
        return 1;

    memcpy(baseNode->data, newData, dataSize);

    return 0;
}

int list_insert(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return 1;

    TlistNode *newNode = createlistNode(data, dataSize);

    if (newNode == NULL)
        return 1;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    ++(list->size);

    return 0;
}

int list_insert_order(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return 1;

    TlistNode *newNode = createlistNode(data, dataSize);

    if (newNode == NULL)
        return 1;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        TlistNode *iterator = list->head;
        TlistNode *prevIterator = NULL;

        while (iterator != NULL && list->compareData(newNode->data, iterator->data) > 0) {
            prevIterator = iterator;
            iterator = iterator->next;
        }

        if (prevIterator == NULL) {
            newNode->next = list->head;
            list->head = newNode;
        } else {
            prevIterator->next = newNode;
        }
    }

    ++(list->size);

    return 0;
}

int list_insert_front(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return 1;

    TlistNode *newNode = createlistNode(data, dataSize);

    if (newNode == NULL)
        return 1;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head = newNode;
    }

    ++(list->size);

    return 0;
}

int list_insert_index(linkedList *list, void *data, size_t dataSize, size_t dataIndex) {
    if (list == NULL || data == NULL)
        return 1;

    if (dataIndex >= list->size)
        return list_insert(list, data, dataSize);

    if (dataIndex == 0)
        return list_insert_front(list, data, dataSize);

    TlistNode *newNode = createlistNode(data, dataSize);

    if (newNode == NULL)
        return 1;

    TlistNode *iterator = list->head;

    while (--dataIndex)
        iterator = iterator->next;

    newNode->next = iterator->next;
    iterator->next = newNode;

    return 0;
}

TlistNode* list_find_index(linkedList *list, size_t dataIndex) {
    if (list == NULL || dataIndex >= list->size)
        return NULL;

    TlistNode *iterator = list->head;

    while (dataIndex--)
        iterator = iterator->next;

    return iterator;
}

TlistNode* list_find_data(linkedList *list, void *data) {
    if (list == NULL || list->head == NULL || data == NULL)
        return NULL;

    TlistNode *iterator = list->head;

    while (iterator != NULL && list->compareData(iterator->data, data) != 0)
        iterator = iterator->next;

    return iterator;
}

int list_delete_data(linkedList *list, void *data) {
    if (list == NULL || list->head == NULL || data == NULL)
        return 1;

    TlistNode *iterator = list->head;
    TlistNode *prevIterator = NULL;

    while (iterator != NULL && list->compareData(iterator->data, data) != 0) {
        prevIterator = iterator;
        iterator = iterator->next;
    }

    if (iterator == NULL)
        return 1;

    if (prevIterator == NULL) {
        list->head = list->head->next;
    } else {
        prevIterator->next = iterator->next;

        if (iterator->next == NULL)
            list->tail = prevIterator;
    }

    if (list->freeData)
        list->freeData(iterator->data);

    free(iterator->data);
    iterator->data = NULL;

    free(iterator);
    iterator = NULL;

    return 0;
}

int list_delete_index(linkedList *list, size_t dataIndex) {
    if (list == NULL || list->head == NULL || dataIndex >= list->size)
        return 1;

    TlistNode *iterator = list->head;
    TlistNode *prevIterator = NULL;

    while (dataIndex--) {
        prevIterator = iterator;
        iterator = iterator->next;
    }

    if (iterator == NULL)
        return 1;

    if (prevIterator == NULL) {
        list->head = list->head->next;
    } else {
        prevIterator->next = iterator->next;

        if (iterator->next == NULL)
            list->tail = prevIterator;
    }

    if (list->freeData)
        list->freeData(iterator->data);

    free(iterator->data);
    iterator->data = NULL;

    free(iterator);
    iterator = NULL;

    return 0;
}

int list_erase(linkedList *list, size_t leftIndex, size_t rightIndex) {
    if (list == NULL || list->head == NULL)
        return 1;

    if (leftIndex > rightIndex) {
        int temporar = leftIndex;
        leftIndex = rightIndex;
        rightIndex = temporar;
    }

    if (leftIndex >= list->size)
        leftIndex = list->size - 1;

    if (rightIndex >= list->size)
        rightIndex = list->size - 1;

    TlistNode *iterator = list->head;
    TlistNode *prevIterator = NULL;
    int deleteNumber = rightIndex - leftIndex + 1;

    while (leftIndex--) {
        prevIterator = iterator;
        iterator = iterator->next;
    }

    while (deleteNumber--) {
        if (prevIterator == NULL) {
            list->head = list->head->next;
        } else {
            prevIterator->next = iterator->next;

            if (iterator->next == NULL)
                list->tail = prevIterator;
        }

        if (list->freeData)
            list->freeData(iterator->data);

        free(iterator->data);
        iterator->data = NULL;

        free(iterator);
        iterator = NULL;

        if (prevIterator == NULL)
            iterator = list->head;
        else
            iterator = prevIterator->next;
    }

    return 0;
}

linkedList* list_filter(linkedList *list, int (*filterFunction)(void *), size_t dataSize) {
    if (list == NULL || list->head == NULL || filterFunction == NULL)
        return NULL;

    linkedList *filterList = createLinkedList(list->compareData, list->printData, list->freeData);

    if (filterList) {
        TlistNode *iterator = list->head;

        while (iterator != NULL) {
            if (filterFunction(iterator->data) == 1)
                list_insert(filterList, iterator->data, dataSize);

            iterator = iterator->next;
        }

        if (filterList->head == NULL) {
            list_free_all(filterList);
            filterList = NULL;
        }
    }

    return filterList;
}

void list_map(linkedList *list, void* (*mapFunction)(void *), size_t dataSize) {
    if (list == NULL || list->head == NULL || mapFunction == NULL)
        return;

    TlistNode *iterator = list->head;

    while (iterator != NULL) {
        memcpy(iterator->data, mapFunction(iterator->data), dataSize);
        iterator = iterator->next;
    }
}

void list_sort(linkedList *list) {
    (void)list;
}