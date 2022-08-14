#include <scl_datastruc.h>
#include <time.h>

void free_st(void * const st) {
    if (NULL != st) {
        stack_t ** const t_st = st;

        free_stack(*t_st);
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    stack_t *sts = create_stack(&free_st, sizeof(sts));

    if (NULL != sts) {
        srand(time(NULL));

        scl_error_t err = SCL_OK;

        for (int i = 0; i < 3; ++i) {
            stack_t *st = create_stack(NULL, sizeof(int));

            for (int j = 0; j < 10; ++j) {
                err = stack_push(st, ltoptr(int, rand() % 100 + 10));

                if (SCL_OK != err) {
                    scl_error_message(err);
                }
            }

            err = stack_push(sts, toptr(st));

            if (SCL_OK != err) {
                scl_error_message(err);
            }
        }

        int stacks_num = 0;

        while (!is_stack_empty(sts)) {
            printf("Queue number %d:\n", stacks_num++);

            stack_t * const * top = stack_top(sts);

            while (!is_stack_empty(*top)) {
                const int *top_elem = stack_top(*top);

                printf("%d ", *top_elem);

                err = stack_pop(*top);

                if (SCL_OK != err) {
                    scl_error_message(err);
                }
            }

            printf("\n\n");

            // Popping the stack has to be the last operation
            // because popping will remove stack from memory
            // If you do not want pop to remove stack from heap
            // You shall not specify a free function into the
            // craettion of stack, and then the free will be
            // done manually by you
            err = stack_pop(sts);

            if (SCL_OK != err) {
                scl_error_message(err);
            }
        }

        free_stack(sts);
    }

    fclose(fout);

    return 0;
}