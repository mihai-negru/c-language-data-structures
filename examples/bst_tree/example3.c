#include <scl_datastruc.h>
#include <time.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In this example we will try to do different operations on bst Tree and measure the time\n");

    /* Init the working strucutres */
    double exec_time = 0.0;

    scl_error_t err = SCL_OK;

    bst_tree_t *my_tree = create_bst(&compare_int, NULL, sizeof(int));
    /* End to init the working structures */

    /* Insert 100 ints into the bst and count the time */
    clock_t begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = bst_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    clock_t end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 100 ints into bst: %lf sec\n", exec_time);
    /* End to insert 100 ints into the bst */

    /* Delete 100 roots from bst and count the time */
    begin = clock();

    for (int i = 0; i < 100; ++i) {
        err = bst_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 100 ints roots from bst: %lf sec\n", exec_time);
    /* End to delete 100 roots from bst */

    /* Free bst tree with size of 100 and count time */
    for (int i = 0; i < 100; ++i) {
        bst_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_bst(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing bst with 100 ints: %lf sec\n\n", exec_time);
    
    my_tree = NULL;
    /* End to free bst tree with size of 100 */

    ////////////////////////////////////////////////////////////// Increase size 1

    my_tree = create_bst(&compare_int, NULL, sizeof(int));

    /* Insert 1000 ints into the bst and count the time */
    begin = clock();

    for (int i = 0; i < 1000; ++i) {
        err = bst_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 1000 ints into bst: %lf sec\n", exec_time);
    /* End to insert 1000 ints into the bst */

    /* Delete 1000 roots from bst and count the time */
    begin = clock();

    for (int i = 0; i < 1000; ++i) {
        err = bst_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 1000 ints roots from bst: %lf sec\n", exec_time);
    /* End to delete 1000 roots from bst */

    /* Free bst tree with size of 1000 and count time */
    for (int i = 0; i < 1000; ++i) {
        bst_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_bst(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing bst with 1000 ints: %lf sec\n\n", exec_time);
    
    my_tree = NULL;
    /* End to free bst tree with size of 1000 */


    ////////////////////////////////////////////////////////////// Increase size 2

    my_tree = create_bst(&compare_int, NULL, sizeof(int));

    /* Insert 10000 ints into the bst and count the time */
    begin = clock();

    for (int i = 0; i < 10000; ++i) {
        err = bst_insert(my_tree, toptr(i));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Inserting 10000 ints into bst: %lf sec\n", exec_time);
    /* End to insert 10000 ints into the bst */

    /* Delete 10000 roots from bst and count the time */
    begin = clock();

    for (int i = 0; i < 10000; ++i) {
        err = bst_delete(my_tree, my_tree->root->data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    end = clock();

    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deleting 10000 ints roots from bst: %lf sec\n", exec_time);
    /* End to delete 10000 roots from bst */

    /* Free bst tree with size of 10000 and count time */
    for (int i = 0; i < 10000; ++i) {
        bst_insert(my_tree, toptr(i));
    }

    begin = clock();

    free_bst(my_tree);
    
    end = clock();
    
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Freeing bst with 10000 ints: %lf sec\n", exec_time);
    
    my_tree = NULL;
    /* End to free bst tree with size of 10000 */
    
    /* Close output file */
    fclose(fout);

    return 0;
}