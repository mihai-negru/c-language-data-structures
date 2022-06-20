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
    
    if (list == NULL) return 0;
    FILE *fin = NULL;

    if ((fin = fopen("ztest.txt", "r")) == NULL) {
        perror("Could not open the file");
        return -1;
    }

    student some;
    int age;

    while (fscanf(fin, "%d", &age) > 0) {
        some.nume = malloc(100);
        snprintf(some.nume, 100, "%c", age);
        some.age = age;
        list_insert(list, &some, sizeof(student));
    }

    fclose(fin);
    list_print(list);
    printf("\n");
    list_free_all(list);
}