#ifndef LIST_UTILS_H_
#define LIST_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "functionTypes.h"

typedef struct listNode {
    void *data;
    struct listNode *next;
} TlistNode;

typedef struct {
    TlistNode *head;
    int (*compareData)(void *, void *);
    void (*printData)(void *);
    void (*freeData)(void *);
    size_t size;
} linkedList;

linkedList* createLinkedList(int (*compareData)(void *, void *),
                            void (*printData)(void *),
                            void (*freeData)(void *));

TlistNode* createlistNode(void *data, size_t dataSize);

int list_insert(linkedList *list, void *data, size_t dataSize);

int list_insert_order(linkedList *list, void *data, size_t dataSize);

int list_insert_front(linkedList *list, void *data, size_t dataSize);

int list_delete_data(linkedList *list, void *data);

int list_delete_index(linkedList *list, int dataIndex);

TlistNode* list_find_index(linkedList *list, int dataIndex);

TlistNode* list_find_data(linkedList *list, void *data);

void list_print(linkedList *list);

void list_free_all(linkedList *list);

#endif // LIST_UTILS_H_