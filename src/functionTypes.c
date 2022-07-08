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

/**
 * @brief Default structures for initialization
 * of defined data types
 * Ex: UChar First = UChar_def();
 *  
 */
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
LongDouble  LongDouble_def      (void) { return (LongDouble){.pass = 12, .val = 0.0}; }

/**
 * @brief Function to print one data type
 * of a data structure that has stored different
 * data types in it. However user has to use predefined
 * data types from functionTypes.h, otherwise an error
 * will occur. 
 * 
 * @param data pointer to data type location
 */
void print_all_data(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Extract type from data
    const char pass = *(const char *)data;

    // Print data type based on pass variable
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

/**
 * @brief Function to print one short int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_short_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%hd ", *(const short int *)data);
}

/**
 * @brief Function to print one unsigned short int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_ushort_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%hd ", *(const unsigned short int *)data);
}

/**
 * @brief Function to print one unsigned int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_uint(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%u ", *(const unsigned int *)data);
}

/**
 * @brief Function to print one int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%d ", *(const int *)data);
}

/**
 * @brief Function to print one long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_long_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%ld ", *(const long int *)data);
}

/**
 * @brief Function to print one unsigned long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_ulong_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%lu ", *(const unsigned long int *)data);
}

/**
 * @brief Function to print one long long int
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_llong_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%lld ", *(const long long int *)data);
}

/**
 * @brief Function to print one unsigned
 * long long int data type. Function may 
 * fail if address of data type is not valid.
 * 
 * @param data pointer to data type location
 */
void print_ullong_int(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%llu ", *(const unsigned long long int *)data);
}

/**
 * @brief Function to print one char
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_char(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%c ", *(const char *)data);
}

/**
 * @brief Function to print one unsigned char
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_uchar(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%c ", *(const unsigned char *)data);
}

/**
 * @brief Function to print one float
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_float(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%f ", *(const float *)data);
}

/**
 * @brief Function to print one double
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_double(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%lf ", *(const double *)data);
}

/**
 * @brief Function to print one long double
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_long_double(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%Lf ", *(const long double *)data);
}

/**
 * @brief Function to print one string
 * data type. Function may fail if address
 * of data type is not valid
 * 
 * @param data pointer to data type location
 */
void print_string(const void *data) {
    // Check if data is valid
    if (data == NULL)
        return;

    // Print data
    printf("%s ", (const char *)data);
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
int compare_short_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const short int *typed_data1 = (const short int *)data1;
    const short int *typed_data2 = (const short int *)data2;

    // Compare data type
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
int compare_ushort_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const unsigned short int *typed_data1 = (const unsigned short int *)data1;
    const unsigned short int *typed_data2 = (const unsigned short int *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_uint(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const unsigned int *typed_data1 = (const unsigned int *)data1;
    const unsigned int *typed_data2 = (const unsigned int *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const int *typed_data1 = (const int *)data1;
    const int *typed_data2 = (const int *)data2;

    // Compare data type
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
int compare_long_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const long int *typed_data1 = (const long int *)data1;
    const long int *typed_data2 = (const long int *)data2;

    // Compare data type
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
int compare_ulong_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const unsigned long int *typed_data1 = (const unsigned long int *)data1;
    const unsigned long int *typed_data2 = (const unsigned long int *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_llong_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const long long int *typed_data1 = (const long long int *)data1;
    const long long int *typed_data2 = (const long long int *)data2;

    // Compare data type
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
int compare_ullong_int(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const unsigned long long int *typed_data1 = (const unsigned long long int *)data1;
    const unsigned long long int *typed_data2 = (const unsigned long long int *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_char(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const char *typed_data1 = (const char *)data1;
    const char *typed_data2 = (const char *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_uchar(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const unsigned char *typed_data1 = (const unsigned char *)data1;
    const unsigned char *typed_data2 = (const unsigned char *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_float(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const float *typed_data1 = (const float *)data1;
    const float *typed_data2 = (const float *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_double(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const double *typed_data1 = (const double *)data1;
    const double *typed_data2 = (const double *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_long_double(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const long double *typed_data1 = (const long double *)data1;
    const long double *typed_data2 = (const long double *)data2;

    // Compare data type
    if (*typed_data1 > *typed_data2)
        return 1;
    else if (*typed_data1 < *typed_data2)
        return -1;
    else
        return 0;
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
int compare_string_size(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    size_t data1_len = strlen((const char *)data1);
    size_t data2_len = strlen((const char *)data2);

    // Compare data type
    if (data1_len > data2_len)
        return 1;
    else if (data1_len < data2_len)
        return -1;
    else
        return 0;
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
int compare_string_lexi(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Compare data type
    return strcmp((const char *)data1, (const char *)data2);
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
int compare_string(const void *data1, const void *data2) {
    // Check if data1 and data2 are valid
    if (data1 == NULL || data2 == NULL) {
        errno = ENODATA;
        perror("Data is not allocated");
        exit(2);
    }

    // Cast data types to working type
    const char* typed_data1 = (const char*)data1;
    const char* typed_data2 = (const char*)data2;

    size_t data1_size = strlen(typed_data1);
    size_t data2_size = strlen(typed_data2);

    // Compare data type
    if (data1_size > data2_size)
        return 1;
    else if (data1_size < data2_size)
        return -1;
    else
        return strcmp(typed_data1, typed_data2);
}