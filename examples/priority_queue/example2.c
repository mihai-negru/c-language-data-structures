#include <scl_datastruc.h>
#include <time.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In this example we will try to do different operations on Priority Queues and measure the time\n\n");

    /* Init the working strucutres */
    double exec_time = 0.0;

    scl_error_t err = SCL_OK;

    priority_queue_t *pq = create_priority_queue(100, &compare_int, NULL, NULL, NULL, sizeof(int), 0);
    /* End to init the working structures */

    /* Insert 100 ints into the pri queue and count the time */
    clock_t begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = pri_queue_push(pq, toptr(i), NULL);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    clock_t end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100 ints into pri queue: %lf sec\n", exec_time);
    /* End to insert 100 ints into the pri queue */

    /* Delete 100 fronts from pri queue and count the time */
    begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = pri_queue_pop(pq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100 ints fronts from pri queue: %lf sec\n", exec_time);
    /* End to delete 100 fronts from pri queue */

    /* Free pri queue with size of 100 and count time */
    for (int i = 0; i < 100; ++i) {
        pri_queue_push(pq, toptr(i), NULL);
    }

    begin = clock();

    free_priority_queue(pq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing pri queue with 100 ints: %lf sec\n\n", exec_time);
    
    pq = NULL;
    /* End to free pri queue with size of 100 */

    ////////////////////////////////////////////////////////////// Increase size 1

    pq = create_priority_queue(100000, &compare_int, NULL, NULL, NULL, sizeof(int), 0);

    /* Insert 100000 ints into the pri queue and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = pri_queue_push(pq, toptr(i), NULL);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100000 ints into pri queue: %lf sec\n", exec_time);
    /* End to insert 100000 ints into the pri queue */

    /* Delete 100000 fronts from pri queue and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = pri_queue_pop(pq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100000 ints fronts from pri queue: %lf sec\n", exec_time);
    /* End to delete 100000 fronts from pri queue */

    /* Free pri queue with size of 100000 and count time */
    for (int i = 0; i < 100000; ++i) {
        pri_queue_push(pq, toptr(i), NULL);
    }

    begin = clock();

    free_priority_queue(pq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing pri queue with 100000 ints: %lf sec\n\n", exec_time);
    
    pq = NULL;
    /* End to free pri queue with size of 100000 */


    ////////////////////////////////////////////////////////////// Increase size 2

    pq = create_priority_queue(8000000, &compare_int, NULL, NULL, NULL, sizeof(int), 0);

    /* Insert 8000000 ints into the pri queue and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = pri_queue_push(pq, toptr(i), NULL);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 8000000 ints into pri queue: %lf sec\n", exec_time);
    /* End to insert 8000000 ints into the pri queue */

    /* Delete 8000000 fronts from pri queue and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = pri_queue_pop(pq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 8000000 ints fronts from pri queue: %lf sec\n", exec_time);
    /* End to delete 8000000 fronts from pri queue */

    /* Free pri queue with size of 8000000 and count time */
    for (int i = 0; i < 8000000; ++i) {
        pri_queue_push(pq, toptr(i), NULL);
    }

    begin = clock();

    free_priority_queue(pq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing pri queue with 8000000 ints: %lf sec\n", exec_time);
    
    pq = NULL;
    /* End to free pri queue with size of 8000000 */
    
    /* Close output file */
    fclose(fout);

    return 0;
}