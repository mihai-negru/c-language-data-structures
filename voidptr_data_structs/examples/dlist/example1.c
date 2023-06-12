#include <scl_datastruc.h>
#include <time.h>

int32_t fil(const void * const data) {
    if (NULL != data) {
        const int * const t_d = data;

        return (*t_d % 2);
    }

    return 0;
}

void map(void * const data) {
    if (NULL != data) {
        int * const t_d = data;

        *t_d = (*t_d) * (*t_d);

        print_int(data);
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    dlist_t *ll = create_dlist(&compare_int, NULL, sizeof(int));

    if (NULL == ll) {
        printf("Something went wrong on creation\n");
        fclose(fout);

        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    scl_error_t err = SCL_OK;

    printf("Inserting some random numbers:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;

        err = dlist_insert(ll, toptr(data));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random numbers in fornt of the dlist:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;

        err = dlist_insert_front(ll, toptr(data));

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random number in order:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;

        err = dlist_insert_order(ll, toptr(data));

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random number at different positions:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;
        size_t index = rand() % get_dlist_size(ll);

        err = dlist_insert_index(ll, toptr(data), index);

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    if (1 == is_dlist_empty(ll)) {
        printf("Something went wrong\n");
        free_dlist(ll);
        fclose(fout);

        exit(EXIT_FAILURE);
    }

    const int *head = get_dlist_head(ll);
    const int *tail = get_dlist_tail(ll);

    if (NULL != head) {
        printf("The head of the dlist is: %d\n", *head);
    } else {
        printf("The head could not be fetched\n");
    }

    if (NULL != tail) {
        printf("The tail of the dlist is: %d\n\n", *tail);
    } else {
        printf("The tail could not be fetched\n\n");
    }

    printf("Let's swap head with te tail:\n");
    err = dlist_swap_data(ll, head, tail);

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    printf("Now we will erase a good chunk from dlist from [10, 40] range:\n");
    err = dlist_erase(ll, 10, 20);

    if (err != SCL_OK) {
        scl_error_message(err);
    }

    dlist_traverse(ll, &print_int);
    printf("\n\n");

    int find1 = rand() % 100 + 10;
    int find2 = rand() % 100 + 10;

    printf("I want to find %d and %d in the dlist:\n", find1, find2);

    const int *find_1 = dlist_find_data(ll, &find1);
    const int *find_2 = dlist_find_data(ll, &find2);

    if (NULL != find_1) {
        printf("I found %d and got the direct pointer to memory location\n", find1);
    } else {
        printf("I did not find the %d value in current dlist\n", find1);
    }

    if (NULL != find_2) {
        printf("I found %d and got the direct pointer to memory location\n\n", find2);
    } else {
        printf("I did not find the %d value in current dlist\n\n", find2);
    }

    find_1 = dlist_find_index(ll, 4);
    find_2 = dlist_find_index(ll, 420);

    if (NULL != find_1) {
        printf("Data element from index 4 is %d\n", *find_1);
    } else {
        printf("Could not fetch index 4's data\n");
    }


    if (NULL != find_2) {
        printf("Data element from index 420 is %d\n", *find_2);
    } else {
        printf("Could not fetch index 420's data\n\n");
    }

    dlist_t *odd_dlist = dlist_filter(ll, &fil);

    printf("Created a new dlist with just odd elements:\n");
    dlist_traverse(odd_dlist, &print_int);
    printf("\n\n");

    printf("Let's find the square number of odd elements:\n");
    dlist_traverse(odd_dlist, &map);
    printf("\n");

    // As seen here is used one traverse call function to modify and to print the values

    free_dlist(ll);
    free_dlist(odd_dlist);

    fclose(fout);

    return 0;
}