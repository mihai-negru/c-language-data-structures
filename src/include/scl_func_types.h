/**
 * @file scl_func_types.h
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

#ifndef _FUNCTION_TYPES_H_
#define _FUNCTION_TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief short_int_t data type definition
 * default pass variable = 0
 */
typedef struct {
    char pass;
    short int val;
} short_int_t;
short_int_t short_int_def(void);

/**
 * @brief ushort_int_t data type definition
 * default pass variable = 1
 */
typedef struct {
    char pass;
    unsigned short int val;
} ushort_int_t;
ushort_int_t ushort_int_def(void);

/**
 * @brief uint_t data type definition
 * default pass variable = 2
 */
typedef struct {
    char pass;
    unsigned int val;
} uint_t;
uint_t uint_def(void);

/**
 * @brief int_t data type definition
 * default pass variable = 3
 */
typedef struct {
    char pass;
    int val;
} int_t;
int_t int_def(void);

/**
 * @brief long_int_t data type definition
 * default pass variable = 4
 */
typedef struct {
    char pass;
    long int val;
} long_int_t;
long_int_t long_int_def(void);

/**
 * @brief ulong_int_t data type definition
 * default pass variable = 5
 */
typedef struct {
    char pass;
    unsigned long int val;
} ulong_int_t;
ulong_int_t ulong_int_def(void);

/**
 * @brief ll_int_t data type definition
 * default pass variable = 6
 */
typedef struct {
    char pass;
    long long int val;
} ll_int_t;
ll_int_t ll_int_def(void);

/**
 * @brief ull_int_t data type definition
 * default pass variable = 7
 */
typedef struct {
    char pass;
    unsigned long long int val;
} ull_int_t;
ull_int_t ull_int_def(void);

/**
 * @brief char_t data type definition
 * default pass variable = 8
 */
typedef struct {
    char pass;
    char val;
} char_t;
char_t char_def(void);

/**
 * @brief uchar_t data type definition
 * default pass variable = 9
 */
typedef struct {
    char pass;
    unsigned char val;
} uchar_t;
uchar_t uchar_def(void);

/**
 * @brief float_t data type definition
 * default pass variable = 10
 */
typedef struct {
    char pass;
    float val;
} float_t;
float_t float_def(void);

/**
 * @brief double_t data type definition
 * default pass variable = 11
 */
typedef struct {
    char pass;
    double val;
} double_t;
double_t double_def(void);

/**
 * @brief long_double_t data type definition
 * default pass variable = 12
 */
typedef struct {
    char pass;
    long double val;
} long_double_t;
long_double_t long_double_def(void);

void print_all_data     (const void * const data);
void print_short_int    (const void * const data);
void print_ushort_int   (const void * const data);
void print_uint         (const void * const data);
void print_int          (const void * const data);
void print_long_int     (const void * const data);
void print_ulong_int    (const void * const data);
void print_llong_int    (const void * const data);
void print_ullong_int   (const void * const data);
void print_char         (const void * const data);
void print_uchar        (const void * const data);
void print_float        (const void * const data);
void print_double       (const void * const data);
void print_long_double  (const void * const data);
void print_string       (const void * const data);

int compare_short_int   (const void * const data1, const void * const data2);
int compare_ushort_int  (const void * const data1, const void * const data2);
int compare_uint        (const void * const data1, const void * const data2);
int compare_int         (const void * const data1, const void * const data2);
int compare_long_int    (const void * const data1, const void * const data2);
int compare_ulong_int   (const void * const data1, const void * const data2);
int compare_llong_int   (const void * const data1, const void * const data2);
int compare_ullong_int  (const void * const data1, const void * const data2);
int compare_char        (const void * const data1, const void * const data2);
int compare_uchar       (const void * const data1, const void * const data2);
int compare_float       (const void * const data1, const void * const data2);
int compare_double      (const void * const data1, const void * const data2);
int compare_long_double (const void * const data1, const void * const data2);
int compare_string_size (const void * const data1, const void * const data2);
int compare_string_lexi (const void * const data1, const void * const data2);
int compare_string      (const void * const data1, const void * const data2);

#endif /* _FUNCTION_TYPES_H_ */