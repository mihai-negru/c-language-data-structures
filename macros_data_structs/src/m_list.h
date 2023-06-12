/**
 * @file m_list.h
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

#ifndef MACROS_GENERICS_LIST_UTILS_H_
#define MACROS_GENERICS_LIST_UTILS_H_

#include "./m_config.h"

#define DEFINE_PTR_FUNC(type)                                                  \
  typedef int32_t (*type##_compare_func)(const type *const,                    \
                                         const type *const);                   \
  typedef void (*type##_free_func)(type *);

#define DEFINE_GENERIC_LIST(type)                                              \
  typedef struct type##_mlist_node_s {                                         \
    type data;                                                                 \
    struct type##_mlist_node_s *next;                                          \
  } type##_mlist_node_ptr_t, *type##_mlist_node_t;                             \
                                                                               \
  typedef struct type##_mlist_s {                                              \
    type##_mlist_node_t head;                                                  \
    type##_mlist_node_t tail;                                                  \
    type##_compare_func cmp;                                                   \
    type##_free_func frd;                                                      \
    size_t size;                                                               \
  } type##_mlist_ptr_t, *type##_mlist_t;                                       \
                                                                               \
  m_err_t type##_mlist(type##_mlist_t *__restrict__ list,                      \
                       type##_compare_func cmp, type##_free_func frd) {        \
    if (list == NULL || (*list) != NULL) {                                     \
      return M_MUST_BE_NULL;                                                   \
    }                                                                          \
                                                                               \
    *list = malloc(sizeof *list);                                              \
                                                                               \
    if (*list == NULL) {                                                       \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    (*list)->cmp = cmp;                                                        \
    (*list)->frd = frd;                                                        \
                                                                               \
    (*list)->head = (*list)->tail = NULL;                                      \
    (*list)->size = 0;                                                         \
                                                                               \
    return M_OK;                                                               \
  }                                                                            \
                                                                               \
  m_err_t type##_free_mlist(type##_mlist_t *__restrict__ list) {               \
    if ((list == NULL) || (*list == NULL)) {                                   \
      while ((*list)->head != NULL) {                                          \
        type##_mlist_node_t iterator = (*list)->head;                          \
        (*list)->head = (*list)->head->next;                                   \
                                                                               \
        if ((*list)->frd != NULL) {                                            \
          (*list)->frd(&iterator->data);                                       \
        }                                                                      \
        free(iterator);                                                        \
      }                                                                        \
                                                                               \
      free(*list);                                                             \
      *list = NULL;                                                            \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }

#endif /* MACROS_GENERIC_LIST_UTILS_H_ */