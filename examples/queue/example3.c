#include <scl_datastruc.h>
#include <time.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In this example we will try to do different operations on Queues and measure the time\n\n");

    /* Init the working strucutres */
    double exec_time = 0.0;

    scl_error_t err = SCL_OK;

    queue_t *qq = create_queue(NULL, sizeof(int));
    /* End to init the working structures */

    /* Insert 100 ints into the queue and count the time */
    clock_t begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = queue_push(qq, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    clock_t end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100 ints into queue: %lf sec\n", exec_time);
    /* End to insert 100 ints into the queue */

    /* Delete 100 fronts from queue and count the time */
    begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = queue_pop(qq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100 ints fronts from queue: %lf sec\n", exec_time);
    /* End to delete 100 fronts from queue */

    /* Free queue with size of 100 and count time */
    for (int i = 0; i < 100; ++i) {
        queue_push(qq, toptr(i));
    }

    begin = clock();

    free_queue(qq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing queue with 100 ints: %lf sec\n\n", exec_time);
    
    qq = NULL;
    /* End to free queue with size of 100 */

    ////////////////////////////////////////////////////////////// Increase size 1

    qq = create_queue(NULL, sizeof(int));

    /* Insert 100000 ints into the queue and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = queue_push(qq, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100000 ints into queue: %lf sec\n", exec_time);
    /* End to insert 100000 ints into the queue */

    /* Delete 100000 fronts from queue and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = queue_pop(qq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100000 ints fronts from queue: %lf sec\n", exec_time);
    /* End to delete 100000 fronts from queue */

    /* Free queue with size of 100000 and count time */
    for (int i = 0; i < 100000; ++i) {
        queue_push(qq, toptr(i));
    }

    begin = clock();

    free_queue(qq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing queue with 100000 ints: %lf sec\n\n", exec_time);
    
    qq = NULL;
    /* End to free queue with size of 100000 */


    ////////////////////////////////////////////////////////////// Increase size 2

    qq = create_queue(NULL, sizeof(int));

    /* Insert 8000000 ints into the queue and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = queue_push(qq, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 8000000 ints into queue: %lf sec\n", exec_time);
    /* End to insert 8000000 ints into the queue */

    /* Delete 8000000 fronts from queue and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = queue_pop(qq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 8000000 ints fronts from queue: %lf sec\n", exec_time);
    /* End to delete 8000000 fronts from queue */

    /* Free queue with size of 8000000 and count time */
    for (int i = 0; i < 8000000; ++i) {
        queue_push(qq, toptr(i));
    }

    begin = clock();

    free_queue(qq);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing queue with 8000000 ints: %lf sec\n", exec_time);
    
    qq = NULL;
    /* End to free queue with size of 8000000 */
    
    /* Close output file */
    fclose(fout);

    return 0;
}