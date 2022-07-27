/**
 * @file scl_sort_algo.h
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

#ifndef SORT_ALGORITHMS_H_
#define SORT_ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_priority_queue.h"
#include "scl_config.h"

scl_error_t         quick_sort          (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         merge_sort          (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         bubble_sort         (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         radix_sort          (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         bucket_sort         (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         shell_sort          (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         insertion_sort      (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         selection_sort      (void* arr, size_t number_of_arr, size_t arr_elem_size, compare_func cmp);
scl_error_t         reverse_array       (void* arr, size_t number_of_arr, size_t arr_elem_size);

#endif /* SORT_ALGORITHMS_H_ */