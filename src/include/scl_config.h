/**
 * @file scl_config.h
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

#ifndef _CONFIG_UTILS_H_
#define _CONFIG_UTILS_H_

#include <stdio.h>

/**
 * @brief Definition of error table handler
 * 
 */
typedef enum scl_error_s {
    SCL_OK                                      = 0,

    SCL_INVALID_INPUT                           = -1,
    
    SCL_INVALID_DATA                            = -2,
    
    SCL_CANNOT_SWAP_DATA                        = -3,
    SCL_DATA_SIZE_ZERO                          = -4,
    SCL_SWAP_SAME_DATA                          = -5,
    SCL_CANNOT_CHANGE_DATA                      = -6,
    SCL_UNDEFINED_DATA                          = -7,
    SCL_DATA_NOT_FOUND_FOR_DELETE               = -8,
    SCL_DATA_NOT_FOUND                              ,
    
    SCL_NULL_LIST                               = -9,
    SCL_NULL_DLIST                              = -10,

    SCL_NULL_STACK                              = -11,
    SCL_NULL_QUEUE                              = -12,
    
    SCL_NULL_PRIORITY_QUEUE                     = -13,
    SCL_NULL_PQUEUE_NODES                       = -14,
    SCL_PRIORITY_SIZE_ZERO                      = -15,
    SCL_CHANGE_PRIORITY_TO_NULL                 = -16,
    SCL_REALLOC_PQNODES_FAIL                    = -17,
    SCL_PQUEUE_CAPACITY_ZERO                    = -18,
    SCL_UNDEFINED_PRIORITY                      = -19,
    SCL_INVALID_PRIORITY                        = -20,
    SCL_NULL_COMPARE_PRIORITY_FUNC              = -21,

    SCL_NULL_BST                                = -22,
    SCL_NULL_RBK                                = -23,
    SCL_UNKNOWN_RBK_COLOR                       = -24,
    SCL_NULL_AVL                                = -25,
    SCL_FIXING_NULL_TREE_NODE                   = -26,

    SCL_MAP_EMPTY_OBJECT                        = -27,
    SCL_NULL_ACTION_FUNC                        = -28,

    SCL_NOT_ENOUGHT_MEM_FOR_NODE                = -29,
    SCL_NOT_ENOUGHT_MEM_FOR_OBJ                 = -30,

    SCL_DELETE_FROM_EMPTY_OBJECT                = -31,
    
    SCL_INDEX_OVERFLOWS_SIZE                    = -32,

    SCL_NULL_SIMPLE_ARRAY                       = -33,
    SCL_NUMBER_OF_ELEMS_ZERO                    = -34,
    SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO             = -35,
    SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL           = -36,

    SCL_NULL_HASH_TABLE                         = -37,
    SCL_UNKNOWN_HASH_NODE_COLOR                 = -38,
    SCL_INVALID_KEY                             = -39,
    SCL_BAD_HASH_FUNCTION                       = -40,
    SCL_NULL_HASH_ROOTS                         = -41,
    SCL_NULL_HASH_FUNCTION                      = -42,
    SCL_INVALID_NEW_CAPACITY                    = -43,
    SCL_REALLOC_HASH_ROOTS_FAIL                 = -44,
    SCL_REHASHING_FAILED                        = -45,
    SCL_KEY_SIZE_ZERO                           = -46
} scl_error_t;

/**
 * @brief Definition of the most used functions
 * 
 */
typedef         size_t          (*hash_func)            (const void * const);
typedef         int             (*compare_func)         (const void * const, const void * const);
typedef         void            (*free_func)            (void *);
typedef         void            (*action_func)          (void * const);
typedef         int             (*filter_func)          (const void * const);

void            scl_error_message           (scl_error_t error_message);

#endif /* _CONFIG_UTILS_H_ */