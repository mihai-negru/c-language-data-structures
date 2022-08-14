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

    list_t *ll = create_list(&compare_int, NULL, sizeof(int));

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

        err = list_insert(ll, toptr(data));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random numbers in fornt of the list:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;

        err = list_insert_front(ll, toptr(data));

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random number in order:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;

        err = list_insert_order(ll, toptr(data));

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    printf("Inserting some random number at different positions:\n");
    for (int i = 0; i < 10; ++i) {
        int data = rand() % 100 + 10;
        size_t index = rand() % get_list_size(ll);

        err = list_insert_index(ll, toptr(data), index);

        if (err != SCL_OK) {
            scl_error_message(err);
        }
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    if (1 == is_list_empty(ll)) {
        printf("Something went wrong\n");
        free_list(ll);
        fclose(fout);

        exit(EXIT_FAILURE);
    }

    const int *head = get_list_head(ll);
    const int *tail = get_list_tail(ll);

    if (NULL != head) {
        printf("The head of the list is: %d\n", *head);
    } else {
        printf("The head could not be fetched\n");
    }

    if (NULL != tail) {
        printf("The tail of the list is: %d\n\n", *tail);
    } else {
        printf("The tail could not be fetched\n\n");
    }

    printf("Let's swap head with te tail:\n");
    err = list_swap_data(ll, head, tail);

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    printf("Now we will erase a good chunk from list from [10, 40] range:\n");
    err = list_erase(ll, 10, 20);

    if (err != SCL_OK) {
        scl_error_message(err);
    }

    list_traverse(ll, &print_int);
    printf("\n\n");

    int find1 = rand() % 100 + 10;
    int find2 = rand() % 100 + 10;

    printf("I want to find %d and %d in the list:\n", find1, find2);

    const int *find_1 = list_find_data(ll, &find1);
    const int *find_2 = list_find_data(ll, &find2);

    if (NULL != find_1) {
        printf("I found %d and got the direct pointer to memory location\n", find1);
    } else {
        printf("I did not find the %d value in current list\n", find1);
    }

    if (NULL != find_2) {
        printf("I found %d and got the direct pointer to memory location\n\n", find2);
    } else {
        printf("I did not find the %d value in current list\n\n", find2);
    }

    find_1 = list_find_index(ll, 4);
    find_2 = list_find_index(ll, 420);

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

    list_t *odd_list = list_filter(ll, &fil);

    printf("Created a new list with just odd elements:\n");
    list_traverse(odd_list, &print_int);
    printf("\n\n");

    printf("Let's find the square number of odd elements:\n");
    list_traverse(odd_list, &map);
    printf("\n");

    // As seen here is used one traverse call function to modify and to print the values

    free_list(ll);
    free_list(odd_list);

    fclose(fout);

    return 0;
}