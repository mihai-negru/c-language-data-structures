/**
 * @file functionTypes.h
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
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef _FUNCTION_TYPES_H_
#define _FUNCTION_TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char pass;
    short int val;
} ShortInt;
ShortInt ShortInt_def(void);

typedef struct {
    char pass;
    unsigned short int val;
} UShortInt;
UShortInt UShortInt_def(void);

typedef struct {
    char pass;
    unsigned int val;
} UInt;
UInt UInt_def(void);

typedef struct {
    char pass;
    int val;
} Int;
Int Int_def(void);

typedef struct {
    char pass;
    long int val;
} LongInt;
LongInt LongInt_def(void);

typedef struct {
    char pass;
    unsigned long int val;
} ULongInt;
ULongInt ULongInt_def(void);

typedef struct {
    char pass;
    long long int val;
} LLInt;
LLInt LLInt_def(void);

typedef struct {
    char pass;
    unsigned long long int val;
} ULLInt;
ULLInt ULLInt_def(void);

typedef struct {
    char pass;
    char val;
} Char;
Char Char_def(void);

typedef struct {
    char pass;
    unsigned char val;
} UChar;
UChar UChar_def(void);

typedef struct {
    char pass;
    float val;
} Float;
Float Float_def(void);

typedef struct {
    char pass;
    double val;
} Double;
Double Double_def(void);

typedef struct {
    char pass;
    long double val;
} LongDouble;
LongDouble LongDouble_def(void);

void print_all_data     (const void *data);
void print_short_int    (const void *data);
void print_ushort_int   (const void *data);
void print_uint         (const void *data);
void print_int          (const void *data);
void print_long_int     (const void *data);
void print_ulong_int    (const void *data);
void print_llong_int    (const void *data);
void print_ullong_int   (const void *data);
void print_char         (const void *data);
void print_uchar        (const void *data);
void print_float        (const void *data);
void print_double       (const void *data);
void print_long_double  (const void *data);
void print_string       (const void *data);

int compare_short_int   (const void *data1, const void *data2);
int compare_ushort_int  (const void *data1, const void *data2);
int compare_uint        (const void *data1, const void *data2);
int compare_int         (const void *data1, const void *data2);
int compare_long_int    (const void *data1, const void *data2);
int compare_ulong_int   (const void *data1, const void *data2);
int compare_llong_int   (const void *data1, const void *data2);
int compare_ullong_int  (const void *data1, const void *data2);
int compare_char        (const void *data1, const void *data2);
int compare_uchar       (const void *data1, const void *data2);
int compare_float       (const void *data1, const void *data2);
int compare_double      (const void *data1, const void *data2);
int compare_long_double (const void *data1, const void *data2);
int compare_string_size (const void *data1, const void *data2);
int compare_string_lexi (const void *data1, const void *data2);
int compare_string      (const void *data1, const void *data2);

#endif // _FUNCTION_TYPES_H_