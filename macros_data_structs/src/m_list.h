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

#define DEFINE_MLIST(type)                                                     \
  DEFINE_PTR_FUNC(type);                                                       \
                                                                               \
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
  } type##_mlist_ptr_t, *type##_mlist_t

#define GENERATE_MLIST(type)                                                   \
  type##_mlist_t type##_mlist(type##_compare_func cmp, type##_free_func frd) { \
    type##_mlist_t list = malloc(sizeof *list);                                \
                                                                               \
    if (list == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    list->cmp = cmp;                                                           \
    list->frd = frd;                                                           \
                                                                               \
    list->head = list->tail = NULL;                                            \
    list->size = 0;                                                            \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  static type##_mlist_node_t type##_mlist_node(type data) {                    \
    type##_mlist_node_t new_node = malloc(sizeof *new_node);                   \
                                                                               \
    if (new_node == NULL) {                                                    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    new_node->next = NULL;                                                     \
    new_node->data = data;                                                     \
                                                                               \
    return new_node;                                                           \
  }                                                                            \
                                                                               \
  merr_t type##_free_mlist(type##_mlist_t *__restrict__ list) {                \
    if ((list != NULL) || (*list != NULL)) {                                   \
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
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }

#define REFLEXION_MLIST(type)                                                  \
  mbool_t type##_mlist_empty(const type##_mlist_t const __restrict__ list) {   \
    if ((list == NULL) || (list->head == NULL)) {                              \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }                                                                            \
                                                                               \
  size_t type##_mlist_size(const type##_mlist_t const __restrict__ list) {     \
    if (list == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return list->size;                                                         \
  }                                                                            \
                                                                               \
  merr_t type##_mlist_head(const type##_mlist_t const __restrict__ list,       \
                           type *const acc) {                                  \
    if (list == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = list->head->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }                                                                            \
                                                                               \
  merr_t type##_mlist_tail(const type##_mlist_t const __restrict__ list,       \
                           type *const acc) {                                  \
    if (list == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = list->tail->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

#define FIND_MLIST(type)
#define MODIFY_MLIST(type)                                                     \
  merr_t type##_mlist_insert(type##_mlist_t const __restrict__ list,           \
                             type data) {                                      \
    if (list == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    type##_mlist_node_t new_node = type##_mlist_node(data);                    \
                                                                               \
    if (new_node == NULL) {                                                    \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (list->head == NULL) {                                                  \
      list->head = new_node;                                                   \
      list->tail = new_node;                                                   \
    } else {                                                                   \
      list->tail->next = new_node;                                             \
      list->tail = new_node;                                                   \
    }                                                                          \
                                                                               \
    ++(list->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define FUNCTIONAL_MLIST(type)

#define ALL_MLIST(type)                                                        \
  DEFINE_MLIST(type);                                                          \
  GENERATE_MLIST(type);                                                        \
  REFLEXION_MLIST(type);                                                       \
  FIND_MLIST(type);                                                            \
  MODIFY_MLIST(type);                                                          \
  FUNCTIONAL_MLIST(type);

#endif /* MACROS_GENERIC_LIST_UTILS_H_ */