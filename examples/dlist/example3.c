#include <scl_datastruc.h>
#include <time.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In this example we will try to do different operations on Double linked lists and measure the time\n\n");

    /* Init the working strucutres */
    double exec_time = 0.0;

    scl_error_t err = SCL_OK;

    dlist_t *ll = create_dlist(&compare_int, NULL, sizeof(int));
    /* End to init the working structures */

    /* Insert 100 ints into the dlist and count the time */
    clock_t begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = dlist_insert(ll, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    clock_t end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100 ints into dlist: %lf sec\n", exec_time);
    /* End to insert 100 ints into the dlist */

    /* Delete 100 heads from dlist and count the time */
    begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = dlist_delete_data(ll, ll->head->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100 ints heads from dlist: %lf sec\n", exec_time);
    /* End to delete 100 heads from dlist */

    /* Free dlist with size of 100 and count time */
    for (int i = 0; i < 100; ++i) {
        dlist_insert(ll, toptr(i));
    }

    begin = clock();

    free_dlist(ll);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing dlist with 100 ints: %lf sec\n\n", exec_time);
    
    ll = NULL;
    /* End to free dlist with size of 100 */

    ////////////////////////////////////////////////////////////// Increase size 1

    ll = create_dlist(&compare_int, NULL, sizeof(int));

    /* Insert 100000 ints into the dlist and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = dlist_insert(ll, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100000 ints into dlist: %lf sec\n", exec_time);
    /* End to insert 100000 ints into the dlist */

    /* Delete 100000 heads from dlist and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = dlist_delete_data(ll, ll->head->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100000 ints heads from dlist: %lf sec\n", exec_time);
    /* End to delete 100000 heads from dlist */

    /* Free dlist with size of 100000 and count time */
    for (int i = 0; i < 100000; ++i) {
        dlist_insert(ll, toptr(i));
    }

    begin = clock();

    free_dlist(ll);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing dlist with 100000 ints: %lf sec\n\n", exec_time);
    
    ll = NULL;
    /* End to free dlist with size of 100000 */


    ////////////////////////////////////////////////////////////// Increase size 2

    ll = create_dlist(&compare_int, NULL, sizeof(int));

    /* Insert 8000000 ints into the dlist and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = dlist_insert(ll, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 8000000 ints into dlist: %lf sec\n", exec_time);
    /* End to insert 8000000 ints into the dlist */

    /* Delete 8000000 heads from dlist and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = dlist_delete_data(ll, ll->head->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 8000000 ints heads from dlist: %lf sec\n", exec_time);
    /* End to delete 8000000 heads from dlist */

    /* Free dlist with size of 8000000 and count time */
    for (int i = 0; i < 8000000; ++i) {
        dlist_insert(ll, toptr(i));
    }

    begin = clock();

    free_dlist(ll);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing dlist with 8000000 ints: %lf sec\n", exec_time);
    
    ll = NULL;
    /* End to free dlist with size of 8000000 */
    
    /* Close output file */
    fclose(fout);

    return 0;
}