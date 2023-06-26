/**
 * @file m_config.h
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2023-06-12
 *
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of c-language-data-structures.
 *
 * c-language-data-structures is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * c-language-data-structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with c-language-data-structures.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MACROS_GENERICS_CONFIG_H_
#define MACROS_GENERICS_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum mbool_s { mfalse = 0, mtrue = 1 } mbool_t;

/**
 * @brief Enum for error handling.
 */
typedef enum merr_s {
  M_OK = 0,
  M_MUST_BE_NULL = -1,
  M_MALLOC_FAILED = -2,
  M_REALLOC_FAILED = -3,
  M_FREE_NULL = -4,
  M_NULL_INPUT = -5,
  M_NOT_FOUND = -6,
  M_POP_FROM_EMPTY = -7,
  M_IDX_OVERFLOW = -8,
  M_NULL_ACTION = -9,
  M_EMPTY_STRUCTURE = -10,
  M_INVALID_INPUT = -11,
  M_UNDEFINED_BEHAVIOUR = -12
} merr_t;

#define CMP_FUNC(ID, T)                                                        \
  typedef int32_t (*ID##_compare_func)(const T *const, const T *const);

#define FREE_FUNC(ID, T) typedef void (*ID##_free_func)(T *);

#define FILTER_FUNC(ID, T) typedef mbool_t (*ID##_filter_func)(const T *const);

#define MAP_FUNC(ID1, T1, ID2, T2)                                             \
  typedef T2 (*ID1##_to_##ID2##_map_func)(const T1 *const);

#define ACTION_FUNC(ID, T) typedef void (*ID##_action_func)(const T *const);

#define MERROR(error)                                                          \
  do {                                                                         \
    switch (error) {                                                           \
    case M_OK:                                                                 \
      fprintf(stderr, "[OK] No errors generated.\n");                          \
      break;                                                                   \
    case M_MUST_BE_NULL:                                                       \
      fprintf(stderr, "[ERROR] The input must be `NULL`.\n");                  \
      break;                                                                   \
    case M_MALLOC_FAILED:                                                      \
      fprintf(stderr, "[ERROR] Failed to malloc heap memory for object.\n");   \
      break;                                                                   \
    case M_REALLOC_FAILED:                                                     \
      fprintf(stderr, "[ERROR] Failed to realloc a memory zone.\n");           \
      break;                                                                   \
    case M_FREE_NULL:                                                          \
      fprintf(stderr, "[ERROR] Freeing a `NULL` object.\n");                   \
      break;                                                                   \
    case M_NULL_INPUT:                                                         \
      fprintf(stderr, "[ERROR] The input is `NULL`.\n");                       \
      break;                                                                   \
    case M_NOT_FOUND:                                                          \
      fprintf(stderr, "[ERROR] The data was not found inside the object.\n");  \
      break;                                                                   \
    case M_POP_FROM_EMPTY:                                                     \
      fprintf(stderr, "[ERROR] Poping from an empty object.\n");               \
      break;                                                                   \
    case M_IDX_OVERFLOW:                                                       \
      fprintf(stderr, "[ERROR] Index is to large for the object.\n");          \
      break;                                                                   \
    case M_NULL_ACTION:                                                        \
      fprintf(stderr, "[ERROR] Action function is `NULL`.\n");                 \
      break;                                                                   \
    case M_EMPTY_STRUCTURE:                                                    \
      fprintf(stderr, "[ERROR] Fetching data from an empty structure.\n");     \
      break;                                                                   \
    case M_INVALID_INPUT:                                                      \
      fprintf(stderr, "[ERROR] Input is invalid for the method type.\n");      \
      break;                                                                   \
    case M_UNDEFINED_BEHAVIOUR:                                                \
      fprintf(stderr, "[ERROR] The method enters in an undefined state "       \
                      "becasue of the input.\n");                              \
      break;                                                                   \
    default:                                                                   \
      fprintf(stderr, "[ERROR] The error code is not known.\n");               \
    }                                                                          \
  } while (0)

#endif /* MACROS_GENERICS_CONFIG_H_ */
