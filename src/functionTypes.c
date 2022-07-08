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

void print_all_data(void *data) {
    if (data == NULL)
        return;

    char pass = *(char *)data;

    switch (pass) {
        case 0:
            printf("%hd ", ((ShortInt *)data)->val);
            break;
        case 1:
            printf("%hd ", ((UShortInt *)data)->val);
            break;
        case 2:
            printf("%u ", ((UInt *)data)->val);
            break;
        case 3:
            printf("%d ", ((Int *)data)->val);
            break;
        case 4:
            printf("%ld ", ((LongInt *)data)->val);
            break;
        case 5:
            printf("%lu ", ((ULongInt *)data)->val);
            break;
        case 6:
            printf("%lld ", ((LLInt *)data)->val);
            break;
        case 7:
            printf("%llu ", ((ULLInt *)data)->val);
            break;
        case 8:
            printf("%c ", ((Char *)data)->val);
            break;
        case 9:
            printf("%c ", ((UChar *)data)->val);
            break;
        case 10:
            printf("%f ", ((Float *)data)->val);
            break;
        case 11:
            printf("%lf ", ((Double *)data)->val);
            break;
        case 12:
            printf("%Lf ", ((LongDouble *)data)->val);
            break;
        default:
            errno = EINVAL;
            perror("Data type undefined");
    }
}

void print_short_int(void *data) {
    if (data == NULL)
        return;

    printf("%hd ", *(short int *)data);
}

void print_ushort_int(void *data) {
    if (data == NULL)
        return;

    printf("%hd ", *(unsigned short int *)data);
}

void print_uint(void *data) {
    if (data == NULL)
        return;

    printf("%u ", *(unsigned int *)data);
}

void print_int(void *data) {
    if (data == NULL)
        return;

    printf("%d ", *(int *)data);
}

void print_long_int(void *data) {
    if (data == NULL)
        return;

    printf("%ld ", *(long int *)data);
}

void print_ulong_int(void *data) {
    if (data == NULL)
        return;

    printf("%lu ", *(unsigned long int *)data);
}

void print_llong_int(void *data) {
    if (data == NULL)
        return;

    printf("%lld ", *(long long int *)data);
}

void print_ullong_int(void *data) {
    if (data == NULL)
        return;

    printf("%llu ", *(unsigned long long int *)data);
}

void print_char(void *data) {
    if (data == NULL)
        return;

    printf("%c ", *(char *)data);
}

void print_uchar(void *data) {
    if (data == NULL)
        return;

    printf("%c ", *(unsigned char *)data);
}

void print_float(void *data) {
    if (data == NULL)
        return;

    printf("%f ", *(float *)data);
}

void print_double(void *data) {
    if (data == NULL)
        return;

    printf("%lf ", *(double *)data);
}

void print_long_double(void *data) {
    if (data == NULL)
        return;

    printf("%Lf ", *(long double *)data);
}

int compare_short_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    short int *typed_data1 = (short int *)data1;
    short int *typed_data2 = (short int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ushort_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    unsigned short int *typed_data1 = (unsigned short int *)data1;
    unsigned short int *typed_data2 = (unsigned short int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_uint(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    unsigned int *typed_data1 = (unsigned int *)data1;
    unsigned int *typed_data2 = (unsigned int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    int *typed_data1 = (int *)data1;
    int *typed_data2 = (int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_long_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    long int *typed_data1 = (long int *)data1;
    long int *typed_data2 = (long int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ulong_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    unsigned long int *typed_data1 = (unsigned long int *)data1;
    unsigned long int *typed_data2 = (unsigned long int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_llong_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    long long int *typed_data1 = (long long int *)data1;
    long long int *typed_data2 = (long long int *)data2;

    return (*typed_data1 - *typed_data2);
}

int compare_ullong_int(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    unsigned long long int *typed_data1 = (unsigned long long int *)data1;
    unsigned long long int *typed_data2 = (unsigned long long int *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_char(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    char *typed_data1 = (char *)data1;
    char *typed_data2 = (char *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_uchar(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    unsigned char *typed_data1 = (unsigned char *)data1;
    unsigned char *typed_data2 = (unsigned char *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_float(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    float *typed_data1 = (float *)data1;
    float *typed_data2 = (float *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_double(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    double *typed_data1 = (double *)data1;
    double *typed_data2 = (double *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}

int compare_long_double(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    long double *typed_data1 = (long double *)data1;
    long double *typed_data2 = (long double *)data2;

    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
}