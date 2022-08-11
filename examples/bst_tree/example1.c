#include <scl_datastruc.h>
#include <time.h>

void mapping_func(void * const data_node) {
    if (NULL != data_node) {
        int * const t_data = data_node;

        // Let's multiply the current value
        // with 10 and add 8

        *t_data = (*t_data) * 3 + 15;
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    bst_tree_t *my_tree = create_bst(&compare_int, NULL, sizeof(int));

    if (NULL == my_tree) {
        printf("bst tree was not allocated\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    scl_error_t err = SCL_OK;

    size_t number_of_elem = 100;

    printf("Generated data:\n");
    for (size_t i = 1; i <= number_of_elem; ++i) {
        if (i % 10 == 0) {
            printf("\n");
        }

        int data = rand() % 2000 + 100;
    
        printf("%d ", data);

        err = bst_insert(my_tree, toptr(data));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }
    printf("\n");

    if (1 == is_bst_empty(my_tree)) {
        printf("Something went really wrong try once again\n");
        exit(EXIT_FAILURE);
    }

    printf("\nThe bst tree printed by level method is:\n");
    bst_traverse_level(my_tree, &print_int);
    printf("\n\n");

    printf("In the bst tree there exist %lu unique random integers\n\n", get_bst_size(my_tree));

    const int *root_data = get_bst_root(my_tree);

    if (NULL != root_data) {
        printf("The root value of the bst tree is %d\n", *root_data);
        printf("The maximum random number generated is %d\n", *(const int *)bst_max_data(my_tree, root_data));
        printf("The minimum random number generated is %d\n", *(const int *)bst_min_data(my_tree, root_data));

        const int *root_pred = bst_predecessor_data(my_tree, root_data);
        const int *root_succ = bst_successor_data(my_tree, ltoptr(int, *root_data));

        if (NULL != root_pred) {
            printf("The inorder predecessor of the root value is %d\n", *root_pred);
        } else {
            printf("The root node has no predecessor\n");
        }

        if (NULL != root_succ) {
            printf("The inorder successor of the root value is %d\n\n", *root_succ);
        } else {
            printf("The root node has no successor\n\n");
        }

        const int *pred_succ_ancestor = bst_lowest_common_ancestor_data(my_tree, root_pred, root_succ);

        if (NULL != pred_succ_ancestor) {
            printf("The lowest common ancestor for the predecessor and successor of the root is %d\n\n", *pred_succ_ancestor);
        } else {
            printf("Could not find the lowest comon ancestor for predecessor and successor of the root node\n\n");
        }
    } else {
        printf("Could not find the root node\n\n");
    }

    // Let's try to find some random data values;

    int find_1 = rand() % 2000 + 100;
    int find_2 = rand() % 2000 + 100;

    printf("I want to find %d and %d in the bst tree\n", find_1, find_2);

    const int *find1_ptr = bst_find_data(my_tree, toptr(find_1));
    const int *find2_ptr = bst_find_data(my_tree, toptr(find_2));

    if (NULL != find1_ptr) {
        printf("I found %d and it has the level <%d> in the bst tree\n", find_1, bst_data_level(my_tree, find1_ptr));
    } else {
        printf("I didn't found %d in the current tree\n", find_1);
    }

    
    if (NULL != find2_ptr) {
        printf("I found %d and it has the level <%d> in the bst tree\n\n", find_2, bst_data_level(my_tree, ltoptr(int, *find2_ptr)));
    } else {
        printf("I didn't found %d in the current tree\n\n", find_2);
    }

    printf("Let's remove some elements:\n");

    for (size_t i = 0; i < 10; ++i) {
        const int *del_ptr = get_bst_root(my_tree);

        if (NULL != del_ptr) {
            printf("Removing %d value from bst\n", *del_ptr);
            
            err = bst_delete(my_tree, del_ptr);

            if (SCL_OK != err) {
                scl_error_message(err);
            }
        }
    }

    printf("\n");

    printf("After deletion the bst tree shows like (inorder):\n");
    bst_traverse_inorder(my_tree, &print_int);
    printf("\n\n");

    printf("Let's remap ALL bst tree data nodes accroding to mapping function and to print the tree\n\n");
    bst_traverse_inorder(my_tree, &mapping_func);
    
    printf("Printing the bst in inorder:\n");
    bst_traverse_inorder(my_tree, &print_int);

    fclose(fout);
    free_bst(my_tree);

    return 0;
}
