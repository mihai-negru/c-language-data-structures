/**
 * @file scl_sort_algo.c
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2022-06-21
 * 
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of C-language-Data-Structures.
 *
 * C-language-Data-Structures is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C-language-Data-Structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with C-language-Data-Structures.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "./include/scl_sort_algo.h"
#include "./include/scl_queue.h"

/**
 * @brief Function to swap bytes between two
 * memory locations 
 * 
 * @param first_node pointer to first memory location
 * @param second_node pointer to second memory location
 * @param node_size number of bytes to be swapped
 */
static void swap_array_nodes(void *first_node, void *second_node, size_t node_size) {
    /* Cast pointers to uint8_t type */
    uint8_t *typed_first_node = first_node;
    uint8_t *typed_second_node = second_node;

    /* Check if memory location is the same */
    if (typed_first_node == typed_second_node) {
        return;
    }

    /* Swap bytes */
    while (node_size-- > 0) {
        uint8_t temp = *typed_first_node;
        *typed_first_node++ = *typed_second_node;
        *typed_second_node++ = temp;
    }
}

/**
 * @brief Function to arrange elements less and greater than selected
 * pivot to its left and right and then to return pointer to the new
 * location of the pivot.
 * 
 * @param arr_left pointer to the most left element from an array
 * @param arr_right pointer to the most right element from an array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return void* pointer to the pivot element after reshaping array
 */
static void* quick_sort_partition(void *arr_left, void *arr_right, size_t arr_elem_size, compare_func cmp) {
    /* Set swap pointer and pivot pointer*/
    uint8_t *next_pivot_ptr = arr_left;
    uint8_t *pivot_ptr = arr_right;

    /* Flag to check if two data were swapped */
    uint8_t was_first_swap = 0;

    /* Reshape array */
    for (uint8_t *iter = next_pivot_ptr; iter < pivot_ptr; iter = iter + arr_elem_size) {
        if (cmp(iter, pivot_ptr) <= -1) {
            if (0 == was_first_swap) {
                was_first_swap = 1;
            } else {
                next_pivot_ptr = next_pivot_ptr + arr_elem_size;
            }
            
            /* Swap next swap memory location with iterator pointer's location */
            swap_array_nodes(next_pivot_ptr, iter, arr_elem_size);
        }
    }

    /* Swap pivot to it's position and return pointer to pivot location */
    if (0 == was_first_swap) {
        swap_array_nodes(next_pivot_ptr, pivot_ptr, arr_elem_size);

        return next_pivot_ptr;
    }
        
    swap_array_nodes(next_pivot_ptr + arr_elem_size, pivot_ptr, arr_elem_size);
    
    return next_pivot_ptr + arr_elem_size;
}

/**
 * @brief Helper function for quick_sort procedure to sort recursevily,
 * the elements of the array. 
 * 
 * @param arr_left pointer to the most left element from an array
 * @param arr_right pointer to the most right element from an array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 */
static void quick_sort_helper(void *arr_left, void *arr_right, size_t arr_elem_size, compare_func cmp) {
    /* Set left and right pointers */
    uint8_t *left_ptr = arr_left;
    uint8_t *right_ptr = arr_right;

    /* Check if sorting may happen */
    if (left_ptr < right_ptr) {

        /* Get pivot pointer */
        uint8_t* partition_ptr = quick_sort_partition(left_ptr, right_ptr, arr_elem_size, cmp);

        /* Sort left part (smaller than pivot) */
        if (partition_ptr > left_ptr) {
            quick_sort_helper(left_ptr, partition_ptr - arr_elem_size, arr_elem_size, cmp); 
        }
        
        /* Sort right part (greater than pivot) */
        if (partition_ptr < right_ptr) {
            quick_sort_helper(partition_ptr + arr_elem_size, right_ptr, arr_elem_size, cmp); 
        }
    }
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by quick sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t quick_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* Call helper function */
    quick_sort_helper(arr, (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size, arr_elem_size, cmp);

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function two merge two subarrays into one big sorted array.
 * 
 * @param arr_left pointer to the most left element from an array
 * @param arr_middle pointer to the middle element from an array
 * @param arr_right pointer to the most right element from an array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 */
static void merge(void *arr_left, void *arr_middle, void *arr_right, size_t arr_elem_size, compare_func cmp) {
    /* Compute sizes of first and second subarray */
    size_t left_subarray_size = ((uint8_t *)arr_middle - (uint8_t *)arr_left) + arr_elem_size;
    size_t right_subarray_size = ((uint8_t *)arr_right - (uint8_t *)arr_middle);

    /* Allocate on heap left and right subarray */
    uint8_t *left_subarray = malloc(left_subarray_size);
    uint8_t *right_subarray = malloc(right_subarray_size);

    /* Check if subarrays were allocated successfully */
    if (NULL != left_subarray) {
        uint8_t *temp_left_subarray = left_subarray;

        if (NULL != right_subarray) {
            uint8_t *temp_right_subarray = right_subarray;

            /* Set the elements of the subarrays */
            memcpy(left_subarray, (uint8_t *)arr_left, left_subarray_size);
            memcpy(right_subarray, (uint8_t *)arr_middle + arr_elem_size, right_subarray_size);

            /* Compute the numbers of the elements from each subarray */
            left_subarray_size /= arr_elem_size;
            right_subarray_size /= arr_elem_size;

            size_t iter_i = 0;
            size_t iter_j = 0;

            /* Merge data from both subarrays into one array */
            while ((iter_i < left_subarray_size) && (iter_j < right_subarray_size)) {

                /* Check if left element should be added into big array or the right element */
                if (cmp(left_subarray, right_subarray) <= 0) {

                    /* Copy data to big array */
                    memcpy((uint8_t *)arr_left, left_subarray, arr_elem_size);
                    
                    /* Increment to the next position */
                    arr_left = (uint8_t *)arr_left + arr_elem_size;
                    left_subarray = left_subarray + arr_elem_size;
                    
                    ++iter_i;
                } else {

                    /* Copy data to big array */
                    memcpy((uint8_t *)arr_left, right_subarray, arr_elem_size);
                    
                    /* Increment to the next position */
                    arr_left = (uint8_t *)arr_left + arr_elem_size;
                    right_subarray = right_subarray + arr_elem_size;
                    
                    ++iter_j;
                }
            }

            /* Copy rest of elements from first subarray to big sorted array */
            while (iter_i < left_subarray_size) {

                /* Copy data to big array */
                memcpy((uint8_t *)arr_left, left_subarray, arr_elem_size);
                
                /* Increment to the next position */
                arr_left = (uint8_t *)arr_left + arr_elem_size;
                left_subarray = left_subarray + arr_elem_size;
                
                ++iter_i;
            }

            /* Copy rest of the elements from second subarrat to big sorted array */
            while (iter_j < right_subarray_size) {

                /* Copy data to big array */
                memcpy((uint8_t *)arr_left, right_subarray, arr_elem_size);
                
                /* Increment to the next position */
                arr_left = (uint8_t *)arr_left + arr_elem_size;
                right_subarray = right_subarray + arr_elem_size;
                
                ++iter_j;
            }

            /* Free right subarray from memory */
            free(temp_right_subarray); 
        }

        /* Free left subarray from memory */
        free(temp_left_subarray);
    }
}

/**
 * @brief Helper function for merge_sort procedure to sort recursevily,
 * the elements of the array. 
 * 
 * @param arr_left pointer to the most left element from an array
 * @param arr_right pointer to the most right element from an array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 */
static void merge_sort_helper(void *arr_left, void *arr_right, size_t arr_elem_size, compare_func cmp) {
    /* Set left and right pointers */
    uint8_t *left_ptr = arr_left;
    uint8_t *right_ptr = arr_right;

    /* Check if sorting may happen */
    if (left_ptr < right_ptr) {

        /* Compute middle pointer */
        uint8_t* middle_ptr = left_ptr + arr_elem_size * ((right_ptr - left_ptr) / (2 * arr_elem_size));

        /* Sort left subarray */
        merge_sort_helper(left_ptr, middle_ptr, arr_elem_size, cmp);

        /* Sort right subarray */
        if (middle_ptr < right_ptr) {
            merge_sort_helper(middle_ptr + arr_elem_size, right_ptr, arr_elem_size, cmp);
        }

        /* Merge all subarrays recursively until get while array */
        merge(left_ptr, middle_ptr, right_ptr, arr_elem_size, cmp);
    }
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by merge sorting algorithm. 
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t merge_sort(void *arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* Call helper function */
    merge_sort_helper(arr, (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size, arr_elem_size, cmp);

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by bubble sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bubble_sort(void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* Set pointer to the last array element */
    uint8_t *arr_end = (uint8_t*)arr + (number_of_elem - 1) * arr_elem_size;

    /* Set flag to check if data swapping occured */
    uint8_t swap_flag = 0;

    /* Sort array */
    for (uint8_t *iter_i = arr; iter_i < arr_end; iter_i = iter_i + arr_elem_size) {
        
        /* Set swap flag as false */
        swap_flag = 0;

        /* Compute new last element pointer form array */
        uint8_t *second_arr_end = (uint8_t*)arr + (arr_end - iter_i);
        
        /* Check bubbles */
        for (uint8_t *iter_j = arr; iter_j < second_arr_end; iter_j = iter_j + arr_elem_size) {
            if (cmp(iter_j, iter_j + arr_elem_size) >= 1) {

                /* Swap bubbles */
                swap_array_nodes(iter_j, iter_j + arr_elem_size, arr_elem_size);

                swap_flag = 1;
            }
        }

        /* No swap occured so array is already sorted */
        if (0 == swap_flag) {
            break;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief 
 * 
 * @param arr 
 * @param number_of_elem 
 * @return uint64_t 
 */
static uint64_t maximum_array_element(uint64_t* arr, size_t number_of_elem){
	uint64_t max_elem = arr[0];

	for (size_t iter_i = 1; iter_i < number_of_elem; ++iter_i) {
		if(arr[iter_i] > max_elem) {
			max_elem = arr[iter_i];
        }
	}

	return max_elem;
}

/**
 * @brief Get the count of the digits from a whole number
 * 
 * @param number whole number to count its digits
 * @return size_t count of digits from selected number
 */
static size_t get_whole_number_digits(uint64_t number) {
    /* Initialize digits count */
    size_t number_digits = 0;

    /* Compute digits count */
	while (0 != number) {
		number /= 10;
		++number_digits;
	}

    /* Return digits count */
	return number_digits;
}

/**
 * @brief Function to get the digit from a number
 * at a specified position in the input data
 * 
 * @param number whole number to extract one digit
 * @param digit_pos position of the digit from the number
 * @return uint64_t extracted digit from the number
 */
static uint64_t get_whole_number_digit(uint64_t number, size_t digit_pos) {
    /* Initialize digit from number as 0 */
	int number_digit = 0;

    /* Extract digit from number */
	while(digit_pos-- > 0){
		number_digit = number % 10;
		number /= 10;
	}

    /* Return extracted digit */
	return number_digit;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by radix sorting algorithm.
 * 
 * @param arr an array of uint64_t type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t radix_sort(uint64_t* arr, size_t number_of_elem) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == arr + number_of_elem - 1)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Allocate ten queues for sorting */
    queue_t **queues = malloc(sizeof(*queues) * 10);

    /* Check if queues pointer was allocated */
    if (NULL != queues) {

        /* Create 10 queues */
        for (size_t iter_i = 0; iter_i < 10; ++iter_i) {
            
            /* Create one queue from scl_queue.h */
            queues[iter_i] = create_queue(NULL);

            /* Check if current queue was created successully */
            if (NULL == queues[iter_i]) {
                return SCL_NOT_ENOUGHT_MEM_FOR_OBJ;
            }
        }
    }

    /* Find maximum element from current working array */
    uint64_t max_elem = maximum_array_element(arr, number_of_elem);

    /* Compute number of digits of the maximum number */
    size_t digits_count = get_whole_number_digits(max_elem);

    /* Sort numbers*/
    for (size_t iter_i = 1; iter_i <= digits_count; ++iter_i) {

        /* Push numbers into queues */
        for (size_t iter_j = 0; iter_j < number_of_elem; ++iter_j) {

            /* Get digit from number at the iter_i position */
            uint64_t number_digit = get_whole_number_digit(arr[iter_j], iter_i);

            /* Push array element to queue */
            queue_push(queues[number_digit], &arr[iter_j], sizeof(arr[iter_j]));
        }

        size_t iter_k = 0;

        /* Extract numbers from the queues */
        for (size_t iter_j = 0; iter_j < 10; ++iter_j) {
            while (0 == is_queue_empty(queues[iter_j])) {

                /* Get first element from queue */
                arr[iter_k++] = *(uint64_t *)queue_front(queues[iter_j]);

                /* Get error from popping */
                scl_error_t err = queue_pop(queues[iter_j]);

                /* If popping generated an error return it */
                if (SCL_OK != err) {
                    return err;
                }
            }
        }
    }

    /* Free memory allocated by the queues */
    for (size_t iter_i = 0; iter_i < 10; ++iter_i) {

        /* Free queue if it is not NULL */
        if (NULL != queues[iter_i]) {
            free_queue(queues[iter_i]);
        }
    }

    /* Free queues pointer and set to NULL*/
    if (NULL != queues) {
        free(queues);
        queues = NULL;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by bucket sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t bucket_sort(void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by shell sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t shell_sort(void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by insertion sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t insertion_sort(void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to sort a continuous memory location
 * represented as an array statically or dynamically allocated
 * by selection sorting algorithm.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t selection_sort(void* arr, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to reverse a continous memory location
 * represented as an array statically or dynamically allocated.
 * 
 * @param arr an array of any type to sort its elements
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @return scl_error_t enum object for handling errors
 */
scl_error_t reverse_array(void* arr, size_t number_of_elem, size_t arr_elem_size) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return SCL_NULL_SIMPLE_ARRAY;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return SCL_NUMBER_OF_ELEMS_ZERO;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO;
    }

    /* Set left and right element pointers */
    uint8_t* left_iter = arr;
    uint8_t* right_iter = (uint8_t*)arr + (number_of_elem - 1) * arr_elem_size;

    /* Swap elements */
    while (left_iter < right_iter) {

        /* Swap left with right */
        swap_array_nodes(left_iter, right_iter, arr_elem_size);

        /* Increment left pointer */
        left_iter = left_iter + arr_elem_size;
        
        /* Decrement right pointer */
        right_iter = right_iter - arr_elem_size;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to find location of the data pointer in
 * a SORTED array. If data was not found than NULL pointer is
 * returned so user has to check for NULL than to cast to 
 * desired type.
 * 
 * @param arr an array of any type to sort its elements
 * @param data pointer to data to find from array
 * @param number_of_elem number of elements within the selected array
 * @param arr_elem_size size of one element from selected array
 * @param cmp pointer to a function to compare two sets of data from array
 * @return void* pointer to data memory location from array or NULL if
 * data was not found in the array
 */
void* binary_search(void* arr, void* data, size_t number_of_elem, size_t arr_elem_size, compare_func cmp) {
    /* Check if most left pointer and most right pointer of the array are valid */
    if ((NULL == arr) || (NULL == (uint8_t *)arr + (number_of_elem - 1) * arr_elem_size)) {
        return NULL;
    }

    /* Check if data is valid */
    if (NULL == data) {
        return NULL;
    }

    /* Check if there are elements to sort */
    if (0 == number_of_elem) {
        return NULL;
    }

    /* Check if the size of one element is valid */
    if (0 == arr_elem_size) {
        return NULL;
    }

    /* Check if comparision function is valid */
    if (NULL == cmp) {
        return NULL;
    }

    /* Set left and right element pointers */
    uint8_t* left_ptr = arr;
    uint8_t* right_ptr = (uint8_t*)arr + (number_of_elem - 1) * arr_elem_size;

    /*
     * Find data in the array 
     * Left and right pointer should not be equal
     * becase it may access a location that we should
     * not access so will check later for equality
     */
    while (left_ptr < right_ptr) {

        /* Compute the middle element pointer */
        uint8_t* middle_ptr = left_ptr + arr_elem_size * ((right_ptr - left_ptr) / (2 * arr_elem_size));

        /* Compare middle data with input data */
        int32_t compare_data = cmp(middle_ptr, data);

        /* Check the result of the above comparision */
        if (compare_data <= -1) {
            left_ptr = middle_ptr + arr_elem_size;
        } else if (compare_data >= 1) {
            right_ptr = middle_ptr - arr_elem_size;
        } else {

            /* Data was found return pointer to memory location */
            return middle_ptr;
        }
    }

    /* Check if data is in array for the last time (Last chance :) ) */
    if ((left_ptr == right_ptr) && (0 == cmp(left_ptr, data))) {
        return left_ptr;
    }

    /* Data was not found in the array */
    return NULL;
}