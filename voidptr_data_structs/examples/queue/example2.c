#include <scl_datastruc.h>
#include <time.h>

void free_q(void * const q) {
    if (NULL != q) {
        queue_t ** const t_q = q;

        free_queue(*t_q);
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    queue_t *qqs = create_queue(&free_q, sizeof(qqs));

    if (NULL != qqs) {
        srand(time(NULL));

        scl_error_t err = SCL_OK;

        for (int i = 0; i < 3; ++i) {
            queue_t *qq = create_queue(NULL, sizeof(int));

            for (int j = 0; j < 10; ++j) {
                err = queue_push(qq, ltoptr(int, rand() % 100 + 10));

                if (SCL_OK != err) {
                    scl_error_message(err);
                }
            }

            err = queue_push(qqs, toptr(qq));

            if (SCL_OK != err) {
                scl_error_message(err);
            }
        }

        int queue_num = 0;

        while (!is_queue_empty(qqs)) {
            printf("Queue number %d:\n", queue_num++);

            queue_t * const * front = queue_front(qqs);

            while (!is_queue_empty(*front)) {
                const int *front_elem = queue_front(*front);

                printf("%d ", *front_elem);

                err = queue_pop(*front);

                if (SCL_OK != err) {
                    scl_error_message(err);
                }
            }

            printf("\n\n");

            err = queue_pop(qqs);

            if (SCL_OK != err) {
                scl_error_message(err);
            }
        }

        free_queue(qqs);
    }

    fclose(fout);

    return 0;
}