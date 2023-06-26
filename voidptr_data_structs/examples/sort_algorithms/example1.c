#include <scl_datastruc.h>
#include <time.h>

#define MAX_ARR 40

int32_t cmp(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(EXIT_FAILURE);

        return 0;
    }

    /* Cast data types to working type */
    const int * const typed_data1 = data1;
    const int * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return -1;
    } else if (*typed_data1 < *typed_data2) {
        return 1;
    } else {
        return 0;
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    int arr[MAX_ARR];

    srand(time(NULL));

    printf("Generated array is:\n");
    for (int i = 0; i < MAX_ARR; ++i) {
        arr[i] = rand() % 100 + 10;

        printf("%d ", arr[i]);
    }

    printf("\n\n");

    printf("Let's sort the first 10 elements by quick sort (Increase order):\n");
    quick_sort(arr, 10, sizeof(*arr), &compare_int);
    for (int i = 0; i < MAX_ARR; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("Let's sort the next 10 elements by heap sort (Deacrease order):\n");

    // Because heap sort is a priority queue and by default
    // a pri queue is a max pri queue then you should specify
    // the same compare_int function for heap sort to sort
    // elements into deacreasing order.
    heap_sort(arr + 10, 10, sizeof(*arr), &compare_int);
    for (int i = 0; i < MAX_ARR; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("Let's reverse the part of the array that we just sorted:\n");
    reverse_array(arr + 10, 10, sizeof(*arr));
    for (int i = 0; i < MAX_ARR; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("Let's sort now all array by quick sort again (Deacrease):\n");
    quick_sort(arr, MAX_ARR, sizeof(*arr), &cmp);
    for (int i = 0; i < MAX_ARR; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("Let's find some values in array with binary search method:\n");

    for (int i = 10; i <= 110; ++i) {

        // Function specified with binary search to compare data
        // has to be same as the function specified in the sorting method
        // otherwise unknown result can be printed
        int *find = binary_search(arr, toptr(i), MAX_ARR, sizeof(*arr), &cmp);

        if (NULL != find) {
            printf("Binary search found %d value in array\n", i);
        } else {
            printf("Binary search did not find %d value in array\n", i);
        }
    }

    fclose(fout);

    return 0;
}
