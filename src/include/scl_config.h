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

typedef enum {
    SCL_OK                          = 0,
    SCL_NULL_LIST                   = -1,
    SCL_NULL_DLIST                  = -2,
    SCL_NULL_STACK                  = -3,
    SCL_NULL_QUEUE                  = -4,
    SCL_NULL_PRIORITY_QUEUE         = -5,
    SCL_NULL_BST                    = -6,
    SCL_NULL_AVL                    = -7,
    SCL_NULL_RBK                    = -8,
    SCL_INVALID_DATA                = -9,
    SCL_INVALID_INPUT               = -10,
    SCL_INVALID_PRIORITY            = -11,
    SCL_NOT_ENOUGHT_MEM_FOR_NODE    = -12,
    SCL_NOT_ENOUGHT_MEM_FOR_OBJ     = -13,
    SCL_CANNOT_CHANGE_DATA          = -14,
    SCL_FIXING_NULL_TREE_NODE       = -15,
    SCL_NULL_ACTION_FUNC            = -16,
    SCL_DATA_SIZE_ZERO              = -17,
    SCL_DELETE_FROM_EMPTY_OBJECT    = -18,
    SCL_DATA_NOT_FOUND_FOR_DELETE   = -19,
    SCL_CANNOT_SWAP_DATA            = -20,
    SCL_INDEX_OVERFLOWS_SIZE        = -21,
    SCL_MAP_EMPTY_OBJECT            = -22,
    SCL_SWAP_SAME_DATA              = -23,
    SCL_NULL_PQUEUE_NODES           = -24,
    SCL_NULL_COMPARE_PRIORITY_FUNC  = -25,
    SCL_CHANGE_PRIORITY_TO_NULL     = -26,
    SCL_PRIORITY_SIZE_ZERO          = -27,
    SCL_UNDEFINED_PRIORITY          = -28,
    SCL_UNDEFINED_DATA              = -29,
    SCL_PQUEUE_CAPACITY_ZERO        = -30,
    SCL_REALLOC_PQNODES_FAIL        = -31,
    SCL_UNKNOWN_RBK_COLOR           = -32
} scl_error_t;

typedef int (*compare_func)(const void*, const void*);
typedef void (*free_func)(void*);
typedef void (*simple_action)(const void*);
typedef int (*filter_func)(const void*);
typedef const void* (*map_func)(void*);

#endif /* _CONFIG_UTILS_H_ */