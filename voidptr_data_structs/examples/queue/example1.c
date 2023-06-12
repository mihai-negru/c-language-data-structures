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

    queue_t *qq = create_queue(NULL, sizeof(int));

    if (NULL == qq) {
        exit(EXIT_FAILURE);
    }

    scl_error_t err = SCL_OK;

    for (int i = 0; i < 100; ++i) {
        int data = rand() % 100 + 10;

        err = queue_push(qq, toptr(data));
        // queue_push(qq, ltoptr(int, data));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    if (1 == is_queue_empty(qq)) {
        printf("Oops !!! Something went wring with insertion\n");
        exit(EXIT_FAILURE);
    }

    printf("Generated queue has %lu elements:\n", get_queue_size(qq));
    print_queue(qq, &print_int_br);
    printf("\n");

    const int *front = queue_front(qq);
    const int *back = queue_back(qq);

    if (NULL != front) {
        printf("Front element is %d\n", *front);
    } else {
        printf("Could not fetch front element\n");
    }

    if (NULL != back) {
        printf("Back element is %d\n\n", *back);
    } else {
        printf("Could not fetch back element\n\n");
    }

    printf("Let's pop half of the queue:\n");

    for (int i = 0; i < 50; ++i) {
        err = queue_pop(qq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    print_queue(qq, &print_int);
    printf("\n");

    free_queue(qq);

    fclose(fout);

    return 0;
}