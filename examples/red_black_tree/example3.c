#include <scl_datastruc.h>
#include <time.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In this example we will try to do different operations on rbk Tree and measure the time\n");

    /* Init the working strucutres */
    double exec_time = 0.0;

    scl_error_t err = SCL_OK;

    rbk_tree_t *my_tree = create_rbk(&compare_int, NULL, sizeof(int));
    /* End to init the working structures */

    /* Insert 100 ints into the rbk and count the time */
    clock_t begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = rbk_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    clock_t end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100 ints into rbk: %lf sec\n", exec_time);
    /* End to insert 100 ints into the rbk */

    /* Delete 100 roots from rbk and count the time */
    begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = rbk_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100 ints roots from rbk: %lf sec\n", exec_time);
    /* End to delete 100 roots from rbk */

    /* Free rbk tree with size of 100 and count time */
    for (int i = 0; i < 100; ++i) {
        rbk_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_rbk(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing rbk with 100 ints: %lf sec\n\n", exec_time);
    
    my_tree = NULL;
    /* End to free rbk tree with size of 100 */

    ////////////////////////////////////////////////////////////// Increase size 1

    my_tree = create_rbk(&compare_int, NULL, sizeof(int));

    /* Insert 100000 ints into the rbk and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = rbk_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100000 ints into rbk: %lf sec\n", exec_time);
    /* End to insert 100000 ints into the rbk */

    /* Delete 100000 roots from rbk and count the time */
    begin = clock();

    for (int i = 0; i < 100000; ++i) {
        err = rbk_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100000 ints roots from rbk: %lf sec\n", exec_time);
    /* End to delete 100000 roots from rbk */

    /* Free rbk tree with size of 100000 and count time */
    for (int i = 0; i < 100000; ++i) {
        rbk_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_rbk(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing rbk with 100000 ints: %lf sec\n\n", exec_time);
    
    my_tree = NULL;
    /* End to free rbk tree with size of 100000 */


    ////////////////////////////////////////////////////////////// Increase size 2

    my_tree = create_rbk(&compare_int, NULL, sizeof(int));

    /* Insert 8000000 ints into the rbk and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = rbk_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 8000000 ints into rbk: %lf sec\n", exec_time);
    /* End to insert 8000000 ints into the rbk */

    /* Delete 8000000 roots from rbk and count the time */
    begin = clock();

    for (int i = 0; i < 8000000; ++i) {
        err = rbk_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 8000000 ints roots from rbk: %lf sec\n", exec_time);
    /* End to delete 8000000 roots from rbk */

    /* Free rbk tree with size of 8000000 and count time */
    for (int i = 0; i < 8000000; ++i) {
        rbk_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_rbk(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing rbk with 8000000 ints: %lf sec\n", exec_time);
    
    my_tree = NULL;
    /* End to free rbk tree with size of 8000000 */
    
    /* Close output file */
    fclose(fout);

    return 0;
}