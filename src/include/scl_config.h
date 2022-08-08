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
#include <stdint.h>

/**
 * @brief Definition of error table handler
 * 
 */
typedef enum scl_error_s {
    SCL_OK                                      = 0,

    SCL_INVALID_INPUT                           = -1,
    SCL_INVALID_DATA                            = -2,
    
    SCL_CANNOT_SWAP_DATA                        = -3,
    SCL_SWAP_SAME_DATA                          = -4,
    SCL_CANNOT_CHANGE_DATA                      = -5,
    SCL_UNDEFINED_DATA                          = -6,
    SCL_DATA_NOT_FOUND_FOR_DELETE               = -7,
    SCL_DATA_NOT_FOUND                          = -8,
    
    SCL_NULL_LIST                               = -9,
    SCL_NULL_DLIST                              = -10,

    SCL_NULL_STACK                              = -11,
    SCL_NULL_QUEUE                              = -12,
    
    SCL_NULL_PRIORITY_QUEUE                     = -13,
    SCL_NULL_PQUEUE_NODES                       = -14,
    SCL_CHANGE_PRIORITY_TO_NULL                 = -15,
    SCL_REALLOC_PQNODES_FAIL                    = -16,
    SCL_PQUEUE_CAPACITY_ZERO                    = -17,
    SCL_UNDEFINED_PRIORITY                      = -18,
    SCL_INVALID_PRIORITY                        = -19,
    SCL_NULL_COMPARE_PRIORITY_FUNC              = -20,
    SCL_NOT_EMPTY_PRIORITY_QUEUE                = -21,

    SCL_NULL_BST                                = -22,
    SCL_NULL_RBK                                = -23,
    SCL_UNKNOWN_RBK_COLOR                       = -24,
    SCL_NULL_AVL                                = -25,
    SCL_FIXING_NULL_TREE_NODE                   = -26,

    SCL_NULL_ACTION_FUNC                        = -27,

    SCL_NOT_ENOUGHT_MEM_FOR_NODE                = -28,
    SCL_NOT_ENOUGHT_MEM_FOR_OBJ                 = -29,

    SCL_DELETE_FROM_EMPTY_OBJECT                = -30,
    
    SCL_INDEX_OVERFLOWS_SIZE                    = -31,

    SCL_NULL_SIMPLE_ARRAY                       = -32,
    SCL_NUMBER_OF_ELEMS_ZERO                    = -33,
    SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO             = -34,
    SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL           = -35,

    SCL_NULL_HASH_TABLE                         = -36,
    SCL_UNKNOWN_HASH_NODE_COLOR                 = -37,
    SCL_INVALID_KEY                             = -38,
    SCL_NULL_HASH_ROOTS                         = -39,
    SCL_NULL_HASH_FUNCTION                      = -40,
    SCL_REHASHING_FAILED                        = -41,

    SCL_NULL_GRAPH                              = -42,
    SCL_INVALID_EDGE_LENGTH                     = -43,
    SCL_NULL_GRAPH_VERTICES                     = -44,
    SCL_NULL_GRAPH_VERTEX                       = -45,
    SCL_REALLOC_GRAPH_VERTICES_FAIL             = -46,
    SCL_VERTEX_OUT_OF_BOUND                     = -47,
    SCL_EDGE_NOT_FOUND                          = -48,
    SCL_NULL_VERTICES_DISTANCES                 = -49,
    SCL_NULL_VERTICES_PARENTS                   = -50,
    SCL_NULL_PATH_MATRIX                        = -51,
    SCL_GRAPH_INVALID_NEW_VERTICES              = -52
} scl_error_t;

/**
 * @brief Definition of the most used functions
 * 
 */
typedef         size_t          (*hash_func)            (const void * const);
typedef         int32_t         (*compare_func)         (const void * const, const void * const);
typedef         void            (*free_func)            (void *);
typedef         void            (*action_func)          (void * const);
typedef         int32_t         (*filter_func)          (const void * const);

void                            scl_error_message       (scl_error_t error_message);

#endif /* _CONFIG_UTILS_H_ */