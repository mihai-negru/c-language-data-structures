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

typedef enum merr_s {
  M_OK,

  M_MUST_BE_NULL,
  M_MALLOC_FAILED,
  M_FREE_NULL,
  M_NULL_INPUT
} merr_t;

#define DEFINE_PTR_FUNC(type)                                                  \
  typedef int32_t (*type##_compare_func)(const type *const,                    \
                                         const type *const);                   \
  typedef void (*type##_free_func)(type *)

#endif /* MACROS_GENERICS_CONFIG_H_ */