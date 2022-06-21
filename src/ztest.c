#include "./include/listUtils.h"

typedef struct {
    char *nume;
    int age;
} student;

int compareInt(void *a, void *b) {
    student *fa = (student *)a;
    student *fb = (student *)b;

    return fa->age - fb->age;
}

void printInt(void *a) {
    student *fa = (student *)a;
    printf("%s %d\n", fa->nume, fa->age);
}

void freeInt(void *a) {
    student *fa = (student *)a;
    free(fa->nume);
}

int main(void) {
    linkedList *list = createLinkedList(&compareInt, &printInt, &freeInt);
    
    list_free_all(list);
}