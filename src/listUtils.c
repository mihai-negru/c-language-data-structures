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
        newList->head = NULL;
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

int list_insert(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return -1;

    TlistNode *newNode = createlistNode(data, dataSize);
    
    if (newNode == NULL)
        return -1;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        TlistNode *iterator = list->head;

        while (iterator->next != NULL)
            iterator = iterator->next;

        iterator->next = newNode;
    }

    ++(list->size);

    return 0;
}

int list_insert_order(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return -1;

    TlistNode *newNode = createlistNode(data, dataSize);
    
    if (newNode == NULL)
        return -1;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        TlistNode *iterator = list->head;
        TlistNode *prev_iterator = NULL;

        while (iterator != NULL && list->compareData(newNode->data, iterator->data) > 0) {
            prev_iterator = iterator;
            iterator = iterator->next;
        }

        newNode->next = iterator;

        if (prev_iterator)
            prev_iterator->next = newNode;
        else
            list->head = newNode;
    }

    ++(list->size);

    return 0;
}

int list_insert_front(linkedList *list, void *data, size_t dataSize) {
    if (list == NULL || data == NULL)
        return -1;

    TlistNode *newNode = createlistNode(data, dataSize);
    
    if (newNode == NULL)
        return -1;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        newNode->next = list->head;
        list->head = newNode;
    }

    ++(list->size);

    return 0;
}

int list_delete_data(linkedList *list, void *data) {
    (void)list;
    (void)data;

    return 0;
}

int list_delete_index(linkedList *list, int dataIndex) {
    (void)list;
    (void)dataIndex;

    return 0;
}

TlistNode* list_find_index(linkedList *list, int dataIndex) {
    (void)list;
    (void)dataIndex;
    return NULL;
}

TlistNode* list_find_data(linkedList *list, void *data) {
    (void)list;
    (void)data;
    return NULL;
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