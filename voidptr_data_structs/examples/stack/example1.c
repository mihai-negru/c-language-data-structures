#include <scl_datastruc.h>
#include <time.h>

void print_int_br(void * const data) {
    if (NULL != data) {
        static int br = 0;

        if ((br % 10 == 0) && (0 != br)) {
            printf("\n");
        }

        printf("%d ", *(const int * const)data);    

        ++br;    
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    sstack_t *st = create_stack(NULL, sizeof(int));

    if (NULL == st) {
        exit(EXIT_FAILURE);
    }

    scl_error_t err = SCL_OK;

    for (int i = 0; i < 100; ++i) {
        err = stack_push(st, ltoptr(int, rand() % 100 + 10));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    if (1 == is_stack_empty(st)) {
        printf("Oops !!! Something went wrong with insertion\n");
        exit(EXIT_FAILURE);
    }

    printf("Generated stack has %lu elements:\n", get_stack_size(st));
    print_stack(st, &print_int_br);
    printf("\n\n");

    const int *top = stack_top(st);

    if (NULL != top) {
        printf("Top element is %d\n\n", *top);
    } else {
        printf("Could not fetch top element\n\n");
    }

    printf("Let's pop half of the stack:\n");

    for (int i = 0; i < 50; ++i) {
        err = stack_pop(st);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    print_stack(st, &print_int);
    printf("\n");

    free_stack(st);

    fclose(fout);

    return 0;
}