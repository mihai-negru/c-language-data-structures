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
    TlistNode *tail;
    int (*compareData)(void *, void *);
    void (*printData)(void *);
    void (*freeData)(void *);
    size_t size;
} linkedList;

linkedList* createLinkedList(int (*compareData)(void *, void *),
                            void (*printData)(void *),
                            void (*freeData)(void *));

TlistNode* createlistNode(void *data, size_t dataSize);

int is_list_empty(linkedList *list);

int get_list_size(linkedList *list);

void list_swap_data(linkedList *list, TlistNode *firstNode, TlistNode *secondNode);

int list_change_data(linkedList *list, TlistNode *baseNode, void *newData);

int list_insert(linkedList *list, void *data, size_t dataSize);

int list_insert_order(linkedList *list, void *data, size_t dataSize);

int list_insert_front(linkedList *list, void *data, size_t dataSize);

int list_delete_data(linkedList *list, void *data);

int list_delete_index(linkedList *list, int dataIndex);

TlistNode* list_find_index(linkedList *list, int dataIndex);

TlistNode* list_find_data(linkedList *list, void *data);

linkedList* list_erase(linkedList *list, int leftIndex, int rightIndex);

linkedList* list_filter(linkedList *list, int (*filterFunction)(void *));

linkedList* list_map(linkedList *list, int (*mapFunction)(void *));

void list_print(linkedList *list);

void list_free_all(linkedList *list);

#endif // LIST_UTILS_H_