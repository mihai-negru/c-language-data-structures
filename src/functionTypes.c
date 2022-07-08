/**
 * @file functionTypes.c
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

#include "./include/functionTypes.h"

ShortInt    ShortInt_def        (void) { return (ShortInt){.pass = 0, .val = 0}; }
UShortInt   UShortInt_def       (void) { return (UShortInt){.pass = 1, .val = 0}; }
UInt        UInt_def            (void) { return (UInt){.pass = 2, .val = 0}; }
Int         Int_def             (void) { return (Int){.pass = 3, .val = 0}; }
LongInt     LongInt_def         (void) { return (LongInt){.pass = 4, .val = 0}; }
ULongInt    ULongInt_def        (void) { return (ULongInt){.pass = 5, .val = 0}; }
LLInt       LLInt_def           (void) { return (LLInt){.pass = 6, .val = 0}; }
ULLInt      ULLInt_def          (void) { return (ULLInt){.pass = 7, .val = 0}; }
Char        Char_def            (void) { return (Char){.pass = 8, .val = '0'}; }
UChar       UChar_def           (void) { return (UChar){.pass = 9, .val = '0'}; }
Float       Float_def           (void) { return (Float){.pass = 10, .val = 0.0}; }
Double      Double_def          (void) { return (Double){.pass = 11, .val = 0.0}; }
LongDouble  LongDouble_def      (void) { return (LongDouble){.pass = 0, .val = 0.0}; }

void print_all_data(const void *data) {
    if (data == NULL)
        return;

    const char pass = *(const char *)data;

    switch (pass) {
        case 0:
            printf("%hd ", ((const ShortInt *)data)->val);
            break;
        case 1:
            printf("%hd ", ((const UShortInt *)data)->val);
            break;
        case 2:
            printf("%u ", ((const UInt *)data)->val);
            break;
        case 3:
            printf("%d ", ((const Int *)data)->val);
            break;
        case 4:
            printf("%ld ", ((const LongInt *)data)->val);
            break;
        case 5:
            printf("%lu ", ((const ULongInt *)data)->val);
            break;
        case 6:
            printf("%lld ", ((const LLInt *)data)->val);
            break;
        case 7:
            printf("%llu ", ((const ULLInt *)data)->val);
            break;
        case 8:
            printf("%c ", ((const Char *)data)->val);
            break;
        case 9:
            printf("%c ", ((const UChar *)data)->val);
            break;
        case 10:
            printf("%f ", ((const Float *)data)->val);
            break;
        case 11:
            printf("%lf ", ((const Double *)data)->val);
            break;
        case 12:
            printf("%Lf ", ((const LongDouble *)data)->val);
            break;
        default:
            errno = EINVAL;
            perror("Data type undefined");
    }
}

void print_short_int(const void *data) {
    if (data == NULL)
        return;

    printf("%hd ", *(const short int *)data);
}

void print_ushort_int(const void *data) {
    if (data == NULL)
        return;

    printf("%hd ", *(const unsigned short int *)data);
}

void print_uint(const void *data) {
    if (data == NULL)
        return;

    printf("%u ", *(const unsigned int *)data);
}

void print_int(const void *data) {
    if (data == NULL)
        return;

    printf("%d ", *(const int *)data);
}

void print_long_int(const void *data) {
    if (data == NULL)
        return;

    printf("%ld ", *(const long int *)data);
}

void print_ulong_int(const void *data) {
    if (data == NULL)
        return;

    printf("%lu ", *(const unsigned long int *)data);
}

void print_llong_int(const void *data) {
    if (data == NULL)
        return;

    printf("%lld ", *(const long long int *)data);
}

void print_ullong_int(const void *data) {
    if (data == NULL)
        return;

    printf("%llu ", *(const unsigned long long int *)data);
}

void print_char(const void *data) {
    if (data == NULL)
        return;

    printf("%c ", *(const char *)data);
}

void print_uchar(const void *data) {
    if (data == NULL)
        return;

    printf("%c ", *(const unsigned char *)data);
}

void print_float(const void *data) {
    if (data == NULL)
        return;

    printf("%f ", *(const float *)data);
}

void print_double(const void *data) {
    if (data == NULL)
        return;

    printf("%lf ", *(const double *)data);
}

void print_long_double(const void *data) {
    if (data == NULL)
        return;

    printf("%Lf ", *(const long double *)data);
}

int compare_short_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const short int *typed_data1 = (const short int *)data1;
    const short int *typed_data2 = (const short int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ushort_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const unsigned short int *typed_data1 = (const unsigned short int *)data1;
    const unsigned short int *typed_data2 = (const unsigned short int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_uint(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const unsigned int *typed_data1 = (const unsigned int *)data1;
    const unsigned int *typed_data2 = (const unsigned int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const int *typed_data1 = (const int *)data1;
    const int *typed_data2 = (const int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_long_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const long int *typed_data1 = (const long int *)data1;
    const long int *typed_data2 = (const long int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ulong_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const unsigned long int *typed_data1 = (const unsigned long int *)data1;
    const unsigned long int *typed_data2 = (const unsigned long int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_llong_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const long long int *typed_data1 = (const long long int *)data1;
    const long long int *typed_data2 = (const long long int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ullong_int(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const unsigned long long int *typed_data1 = (const unsigned long long int *)data1;
    const unsigned long long int *typed_data2 = (const unsigned long long int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_char(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const char *typed_data1 = (const char *)data1;
    const char *typed_data2 = (const char *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_uchar(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const unsigned char *typed_data1 = (const unsigned char *)data1;
    const unsigned char *typed_data2 = (const unsigned char *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_float(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const float *typed_data1 = (const float *)data1;
    const float *typed_data2 = (const float *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_double(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const double *typed_data1 = (const double *)data1;
    const double *typed_data2 = (const double *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_long_double(const void *data1, const void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    const long double *typed_data1 = (const long double *)data1;
    const long double *typed_data2 = (const long double *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}