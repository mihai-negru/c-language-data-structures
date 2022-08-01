# Documentation for sorting algorithms (scl_sort_algo.h)

## What sort methods am I able to use ?

Well you have a range of some sort methods, as follows:

```C
    scl_error_t quick_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);

    scl_error_t merge_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);

    scl_error_t bubble_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);

    scl_error_t radix_sort(uint64_t *arr, size_t number_of_elem);

    scl_error_t insertion_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);

    scl_error_t selection_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);
```

>**NOTE:** I tried to preserve the syntax as standart qsort method for sorting

>**NOTE:** Radix sort is just for sorting uint64_t (unsigned long) data types.

## I want to sort just a part of the array not the entire array what should I do ?

The solution is very simple just to as follows:

```C
    // let the array be {23, 54, 11, 76, -1, -22, 43, 76, -19}
    int arr = {23, 54, 11, 76, -1, -22, 43, 76, -19};

    // Now let's start sorting from index 4 and sort 3 elements

    quick_sort(arr + 4, 3, sizeof(*arr), &compare_int); // compare_int is a function from func_types.h

    // So after running it you should get

    // arr = {23, 54, 11, 76, -22, -1, 43, 76, -19}

    // You shall not pass the boundaries of the array because you will get a segmenation fault
    // like quick_sort(arr + 4, 10, arr_size, &compare_int);
```

>**NOTE:** You can use the same definition for every sorting method to sort just parts of the array not the intire array


## Some function not related to sorting

```C
    scl_error_t reverse_array(void *arr, size_t number_of_elem, size_t arr_elem_size);
    void* binary_search(void *arr, void *data, size_t number_of_elem, size_t arr_elem_size, compare_func cmp);
```

These two function are to reverse an array or just a part of it and the second is to find data from a sorted array (or subarray)

Examples:

```C
    int arr = {23, 54, 11, 76, -1, -22, 43, 76, -19};

    reverse_array(arr + 2, 4, sizeof(*arr));

    // arr = {23, 54, -22, -1, 76, 11, 43, 76, -19}
```

```C
    int arr = {23, 54, 11, 76, -1, -22, 43, 76, -19};

    // First we sort the array or the subarray needed
    quick_sort(arr, sizeof(arr)/sizeof(*arr), sizeof(*arr), &compare_int);

    int *my_data = binary_search(arr + 1, &int{23}, 8, &compare_int); // compare function should match as compare function of the sorting method

    if (NULL != my_data) {
        *my_data = *my_data + 1000;
    }

    // If binary search function didn't fail the final result should be

    // arr = {-22, -19, -1, 11, 1023, 43, 54, 76, 76}
```

## For some other examples of using sorting methods you can look up at /examples/sort_algorithms