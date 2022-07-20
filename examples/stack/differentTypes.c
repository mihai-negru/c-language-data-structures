#include "stackUtils.h"

typedef struct {
    int check; // 0
    int elem;
} Int;

typedef struct {
    int check; // 1
    float elem;
} Float;

typedef struct {
    int check; // 2
    char elem;
} Char;

void printData(void *a) {
    if (a == NULL) return;

    int check = *(int *)a;

    if (check == 0) {
        Int *fb = (Int *)a;

        printf("%d ", fb->elem);
    } else if (check == 1) {
        Float *fb = (Float *)a;

        printf("%f ", fb->elem);
    } else if (check == 2) {
        Char *fb = (Char *)a;

        printf("%c ", fb->elem);
    } else {
        printf("Not known check var\n");
    }
}

int main(void) {
    TStack *stack = create_stack(0);

    Int first;
    first.elem = 1;
    first.check = 0;

    Float second;
    second.elem = 2.5;
    second.check = 1;

    Char third;
    third.elem = 'M';
    third.check = 2;

    stack_push(stack, &first, sizeof(Int));
    stack_push(stack, &second, sizeof(Float));
    stack_push(stack, &third, sizeof(Char));

    print_stack(stack, &printData);
    printf("\n");

    free_stack(stack);

    return 0;
}