/**
 * @file scl_func_types.c
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

#include "./include/scl_func_types.h"

/**
 * @brief Default structures for initialization
 * of defined data types
 * Ex: uchar_t First = UChar_def();
 *  
 */
short_int_t     ShortInt_def        (void) { return (short_int_t){.pass = 0, .val = 0}; }
ushort_int_t    UShortInt_def       (void) { return (ushort_int_t){.pass = 1, .val = 0}; }
uint_t          UInt_def            (void) { return (uint_t){.pass = 2, .val = 0}; }
int_t           Int_def             (void) { return (int_t){.pass = 3, .val = 0}; }
long_int_t      LongInt_def         (void) { return (long_int_t){.pass = 4, .val = 0}; }
ulong_int_t     ULongInt_def        (void) { return (ulong_int_t){.pass = 5, .val = 0}; }
ll_int_t        LLInt_def           (void) { return (ll_int_t){.pass = 6, .val = 0}; }
ull_int_t       ULLInt_def          (void) { return (ull_int_t){.pass = 7, .val = 0}; }
char_t          Char_def            (void) { return (char_t){.pass = 8, .val = '0'}; }
uchar_t         UChar_def           (void) { return (uchar_t){.pass = 9, .val = '0'}; }
float_t         Float_def           (void) { return (float_t){.pass = 10, .val = 0.0}; }
double_t        Double_def          (void) { return (double_t){.pass = 11, .val = 0.0}; }
long_double_t   LongDouble_def      (void) { return (long_double_t){.pass = 12, .val = 0.0}; }

/**
 * @brief Function to print one data type
 * of a data structure that has stored different
 * data types in it. However user has to use predefined
 * data types from functionTypes.h, otherwise an error
 * will occur. 
 * 
 * @param data pointer to data type location
 */
void print_all_data(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Extract type from data */
    const char pass = *(const char *)data;

    /* Print data type based on pass variable */
    switch (pass) {
        case 0:
            printf("%hd ", ((const short_int_t * const)data)->val);
            break;
        case 1:
            printf("%hd ", ((const ushort_int_t * const)data)->val);
            break;
        case 2:
            printf("%u ", ((const uint_t * const)data)->val);
            break;
        case 3:
            printf("%d ", ((const int_t * const)data)->val);
            break;
        case 4:
            printf("%ld ", ((const long_int_t * const)data)->val);
            break;
        case 5:
            printf("%lu ", ((const ulong_int_t * const)data)->val);
            break;
        case 6:
            printf("%lld ", ((const ll_int_t * const)data)->val);
            break;
        case 7:
            printf("%llu ", ((const ull_int_t * const)data)->val);
            break;
        case 8:
            printf("%c ", ((const char_t * const)data)->val);
            break;
        case 9:
            printf("%c ", ((const uchar_t * const)data)->val);
            break;
        case 10:
            printf("%f ", ((const float_t * const)data)->val);
            break;
        case 11:
            printf("%lf ", ((const double_t * const)data)->val);
            break;
        case 12:
            printf("%Lf ", ((const long_double_t * const)data)->val);
            break;
        default:
            errno = EINVAL;
            perror("Data type undefined");
    }
}

/**
 * @brief Function to print one short int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_short_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%hd ", *(const short int * const)data);
}

/**
 * @brief Function to print one unsigned short int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_ushort_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%hd ", *(const unsigned short int * const)data);
}

/**
 * @brief Function to print one unsigned int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_uint(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%u ", *(const unsigned int * const)data);
}

/**
 * @brief Function to print one int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%d ", *(const int * const)data);
}

/**
 * @brief Function to print one long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_long_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%ld ", *(const long int * const)data);
}

/**
 * @brief Function to print one unsigned long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_ulong_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%lu ", *(const unsigned long int * const)data);
}

/**
 * @brief Function to print one long long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_llong_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%lld ", *(const long long int * const)data);
}

/**
 * @brief Function to print one unsigned
 * long long int data type. Function may 
 * fail if address of data type is not valid.
 * 
 * @param data pointer to data type location
 */
void print_ullong_int(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%llu ", *(const unsigned long long int * const)data);
}

/**
 * @brief Function to print one char
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_char(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%c ", *(const char * const)data);
}

/**
 * @brief Function to print one unsigned char
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_uchar(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%c ", *(const unsigned char * const)data);
}

/**
 * @brief Function to print one float
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_float(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%f ", *(const float * const)data);
}

/**
 * @brief Function to print one double
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_double(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%lf ", *(const double * const)data);
}

/**
 * @brief Function to print one long double
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_long_double(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%Lf ", *(const long double * const)data);
}

/**
 * @brief Function to print one string
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_string(const void * const data) {
    /* Check if data is valid */
    if (NULL == data) {
        return;
    }

    /* Print data */
    printf("%s ", (const char * const)data);
}

/**
 * @brief Function to compare two elements of
 * the short int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_short_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const short int * const typed_data1 = data1;
    const short int * const typed_data2 = data2;

    /* Compare data type */
    return (*typed_data1 - *typed_data2);
}

/**
 * @brief Function to compare two elements of
 * the unsigned short int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_ushort_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const unsigned short int * const typed_data1 = data1;
    const unsigned short int * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the unsigned int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_uint(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const unsigned int * const typed_data1 = data1;
    const unsigned int * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const int * const typed_data1 = data1;
    const int * const typed_data2 = data2;

    /* Compare data type */
    return (*typed_data1 - *typed_data2);
}

/**
 * @brief Function to compare two elements of
 * the long int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_long_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const long int * const typed_data1 = data1;
    const long int * const typed_data2 = data2;

    /* Compare data type */
    return (*typed_data1 - *typed_data2);
}

/**
 * @brief Function to compare two elements of
 * the unsigned long int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_ulong_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const unsigned long int * const typed_data1 = data1;
    const unsigned long int * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the long long int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_llong_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const long long int * const typed_data1 = data1;
    const long long int * const typed_data2 = data2;

    /* Compare data type */
    return (*typed_data1 - *typed_data2);
}

/**
 * @brief Function to compare two elements of
 * the unsigned long long int data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_ullong_int(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const unsigned long long int * const typed_data1 = data1;
    const unsigned long long int * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the char data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_char(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const char * const typed_data1 = data1;
    const char * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the unsigned char data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_uchar(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const unsigned char * const typed_data1 = data1;
    const unsigned char * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the float data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_float(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const float * const typed_data1 = data1;
    const float * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the double data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_double(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const double *const typed_data1 = data1;
    const double *const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the long double data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_long_double(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const long double * const typed_data1 = data1;
    const long double * const typed_data2 = data2;

    /* Compare data type */
    if (*typed_data1 > *typed_data2) {
        return 1;
    } else if (*typed_data1 < *typed_data2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the string data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 length is greater
 * than data2 length, -1 if data2 length is greater
 * than data1 length, 0 if data1 length is equal
 * with data2 length
 */
int compare_string_size(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    size_t data1_len = strlen((const char * const)data1);
    size_t data2_len = strlen((const char * const)data2);

    /* Compare data type */
    if (data1_len > data2_len) {
        return 1;
    } else if (data1_len < data2_len) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Function to compare two elements of
 * the string data type.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2. Elements are compared
 * lexicographycally
 */
int compare_string_lexi(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Compare data type */
    return strcmp((const char * const)data1, (const char * const)data2);
}

/**
 * @brief Function to compare two elements of
 * the string data type. This function is a
 * combination of compare_string_size and
 * compare_string_lexi. First function will
 * compare their length and then will compare
 * them lexicographycally.
 * 
 * @param data1 pointer to data type 1 location
 * @param data2 pointer to data type 2 location
 * @return int 1 if data1 is greater than data2,
 * -1 if data2 is greater than data1, 0 if
 * data1 is equal with data2
 */
int compare_string(const void * const data1, const void * const data2) {
    /* Check if data1 and data2 are valid */
    if ((NULL == data1) || (NULL == data2)) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    /* Cast data types to working type */
    const char * const typed_data1 = data1;
    const char * const typed_data2 = data2;

    size_t data1_size = strlen(typed_data1);
    size_t data2_size = strlen(typed_data2);

    /* Compare data type */
    if (data1_size > data2_size) {
        return 1;
    } else if (data1_size < data2_size) {
        return -1;
    } else {
        return strcmp(typed_data1, typed_data2);
    }
}