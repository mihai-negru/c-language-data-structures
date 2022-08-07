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

#define         toptr(var)                      (&(var))
#define         ltoptr(type, lval)              (&(type){(lval)})
#define         make_pair(m1, m2)               (m1), (m2)

void            print_all_data                  (void * const data);
void            print_short_int                 (void * const data);
void            print_ushort_int                (void * const data);
void            print_uint                      (void * const data);
void            print_int                       (void * const data);
void            print_long_int                  (void * const data);
void            print_ulong_int                 (void * const data);
void            print_llong_int                 (void * const data);
void            print_ullong_int                (void * const data);
void            print_char                      (void * const data);
void            print_uchar                     (void * const data);
void            print_float                     (void * const data);
void            print_double                    (void * const data);
void            print_long_double               (void * const data);
void            print_string                    (void * const data);

int32_t         compare_short_int               (const void * const data1, const void * const data2);
int32_t         compare_ushort_int              (const void * const data1, const void * const data2);
int32_t         compare_uint                    (const void * const data1, const void * const data2);
int32_t         compare_int                     (const void * const data1, const void * const data2);
int32_t         compare_long_int                (const void * const data1, const void * const data2);
int32_t         compare_ulong_int               (const void * const data1, const void * const data2);
int32_t         compare_llong_int               (const void * const data1, const void * const data2);
int32_t         compare_ullong_int              (const void * const data1, const void * const data2);
int32_t         compare_char                    (const void * const data1, const void * const data2);
int32_t         compare_uchar                   (const void * const data1, const void * const data2);
int32_t         compare_float                   (const void * const data1, const void * const data2);
int32_t         compare_double                  (const void * const data1, const void * const data2);
int32_t         compare_long_double             (const void * const data1, const void * const data2);
int32_t         compare_string_size             (const void * const data1, const void * const data2);
int32_t         compare_string_lexi             (const void * const data1, const void * const data2);
int32_t         compare_string                  (const void * const data1, const void * const data2);

#endif /* _FUNCTION_TYPES_H_ */