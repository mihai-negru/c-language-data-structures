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

#define MLIST(name, type)                                                      \
  CMP_FUNC(name, type)                                                         \
  FREE_FUNC(name, type)                                                        \
                                                                               \
  typedef struct name##_mlist_node_s {                                         \
    type data;                                                                 \
    struct name##_mlist_node_s *next;                                          \
  } name##_mlist_node_ptr_t, *name##_mlist_node_t;                             \
                                                                               \
  typedef struct name##_mlist_s {                                              \
    name##_mlist_node_t head;                                                  \
    name##_mlist_node_t tail;                                                  \
    name##_compare_func cmp;                                                   \
    name##_free_func frd;                                                      \
    size_t size;                                                               \
  } name##_mlist_ptr_t, *name##_mlist_t;                                       \
                                                                               \
  name##_mlist_t name##_mlist(name##_compare_func cmp, name##_free_func frd) { \
    if (cmp == NULL) {                                                         \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name##_mlist_t self = malloc(sizeof *self);                                \
                                                                               \
    if (self == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->cmp = cmp;                                                           \
    self->frd = frd;                                                           \
                                                                               \
    self->head = self->tail = NULL;                                            \
    self->size = 0;                                                            \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  name##_mlist_node_t name##_mlist_node(type data) {                           \
    name##_mlist_node_t self_node = malloc(sizeof *self_node);                 \
                                                                               \
    if (self_node == NULL) {                                                   \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self_node->next = NULL;                                                    \
    self_node->data = data;                                                    \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t name##_mlist_free(name##_mlist_t *self) {                             \
    if ((self != NULL) || (*self != NULL)) {                                   \
      while ((*self)->head != NULL) {                                          \
        name##_mlist_node_t iterator = (*self)->head;                          \
        (*self)->head = (*self)->head->next;                                   \
                                                                               \
        if ((*self)->frd != NULL) {                                            \
          (*self)->frd(&iterator->data);                                       \
        }                                                                      \
        free(iterator);                                                        \
      }                                                                        \
                                                                               \
      free(*self);                                                             \
      *self = NULL;                                                            \
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }

#define MLIST_EMPTY(name, type)                                                \
  mbool_t name##_mlist_empty(const name##_mlist_ptr_t *const self) {           \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

#define MLIST_SIZE(name, type)                                                 \
  size_t name##_mlist_size(const name##_mlist_ptr_t *const self) {             \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

#define MLIST_HEAD(name, type)                                                 \
  merr_t name##_mlist_head(const name##_mlist_ptr_t *const self,               \
                           type *const acc) {                                  \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = self->head->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_TAIL(name, type)                                                 \
  merr_t name##_mlist_tail(const name##_mlist_ptr_t *const self,               \
                           type *const acc) {                                  \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = self->tail->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_FIND_NODE(name, type)                                            \
  name##_mlist_node_t name##_mlist_find_node(                                  \
      const name##_mlist_ptr_t *const self, type data) {                       \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
                                                                               \
    while ((iterator != NULL) && (self->cmp(&iterator->data, &data) != 0)) {   \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    return iterator;                                                           \
  }

#define MLIST_FIND_IDX(name, type)                                             \
  merr_t name##_mlist_find_idx(const name##_mlist_ptr_t *const self,           \
                               size_t idx, type *const acc) {                  \
    if ((self == NULL) || (idx >= self->size)) {                               \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (idx == (self->size - 1)) {                                             \
      if (self->tail != NULL) {                                                \
        *acc = self->tail->data;                                               \
        return M_OK;                                                           \
      }                                                                        \
                                                                               \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
                                                                               \
    while (idx--) {                                                            \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (iterator != NULL) {                                                    \
      *acc = iterator->data;                                                   \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_NOT_FOUND;                                                        \
  }

#define MLIST_FIND(name, type)                                                 \
  merr_t name##_mlist_find(const name##_mlist_ptr_t *const self, type data,    \
                           type *const acc) {                                  \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
                                                                               \
    while ((iterator != NULL) && (self->cmp(&iterator->data, &data) != 0)) {   \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (iterator != NULL) {                                                    \
      *acc = iterator->data;                                                   \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_NOT_FOUND;                                                        \
  }

#define MLIST_SWAP(name, type)                                                 \
  merr_t name##_mlist_swap(const name##_mlist_t self, type fst, type snd) {    \
    name##_mlist_node_t fst_node = name##_mlist_find_node(self, fst);          \
                                                                               \
    if (fst_node == NULL) {                                                    \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    name##_mlist_node_t snd_node = name##_mlist_find_node(self, snd);          \
                                                                               \
    if (snd_node == NULL) {                                                    \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    if (fst_node != snd_node) {                                                \
      type temp = fst_node->data;                                              \
      fst_node->data = snd_node->data;                                         \
      snd_node->data = temp;                                                   \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_CHANGE(name, type)                                               \
  merr_t name##_mlist_change(const name##_mlist_t self, type base, type new) { \
    name##_mlist_node_t base_node = name##_mlist_find_node(self, base);        \
                                                                               \
    if (base_node == NULL) {                                                   \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    base_node->data = new;                                                     \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_PUSH(name, type)                                                 \
  merr_t name##_mlist_push(const name##_mlist_t self, type data) {             \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    name##_mlist_node_t self_node = name##_mlist_node(data);                   \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      self->head = self_node;                                                  \
      self->tail = self_node;                                                  \
    } else {                                                                   \
      self->tail->next = self_node;                                            \
      self->tail = self_node;                                                  \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_PUSH_ORDER(name, type)                                           \
  merr_t name##_mlist_push_order(const name##_mlist_t self, type data) {       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    name##_mlist_node_t self_node = name##_mlist_node(data);                   \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      self->head = self_node;                                                  \
      self->tail = self_node;                                                  \
    } else {                                                                   \
      name##_mlist_node_t iterator = self->head;                               \
      name##_mlist_node_t prev_iterator = NULL;                                \
                                                                               \
      while ((iterator != NULL) &&                                             \
             (self->cmp(&self_node->data, &iterator->data) > 0)) {             \
        prev_iterator = iterator;                                              \
        iterator = iterator->next;                                             \
      }                                                                        \
                                                                               \
      if (prev_iterator == NULL) {                                             \
        self_node->next = self->head;                                          \
        self->head = self_node;                                                \
      } else {                                                                 \
        self_node->next = iterator;                                            \
        prev_iterator->next = self_node;                                       \
                                                                               \
        if (iterator == NULL) {                                                \
          self->tail = self_node;                                              \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_PUSH_FRONT(name, type)                                           \
  merr_t name##_mlist_push_front(const name##_mlist_t self, type data) {       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    name##_mlist_node_t self_node = name##_mlist_node(data);                   \
                                                                               \
    if (self == NULL) {                                                        \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      self->head = self_node;                                                  \
      self->tail = self_node;                                                  \
    } else {                                                                   \
      self_node->next = self->head;                                            \
      self->head = self_node;                                                  \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_PUSH_IDX(name, type)                                             \
  merr_t name##_mlist_push_idx(const name##_mlist_t self, type data,           \
                               size_t idx) {                                   \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (idx >= self->size) {                                                   \
      return name##_mlist_push(self, data);                                    \
    }                                                                          \
                                                                               \
    if (idx == 0)                                                              \
      return name##_mlist_push_front(self, data);                              \
                                                                               \
    name##_mlist_node_t self_node = name##_mlist_node(data);                   \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
                                                                               \
    while (--idx) {                                                            \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    self_node->next = iterator->next;                                          \
    iterator->next = self_node;                                                \
                                                                               \
    if (self_node->next == NULL) {                                             \
      self->head = self_node;                                                  \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_POP(name, type)                                                  \
  merr_t name##_mlist_pop(const name##_mlist_t self, type data) {              \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
    name##_mlist_node_t prev_iterator = NULL;                                  \
                                                                               \
    while ((iterator != NULL) && (self->cmp(&iterator->data, &data) != 0)) {   \
      prev_iterator = iterator;                                                \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (iterator == NULL) {                                                    \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    if (prev_iterator == NULL) {                                               \
      self->head = self->head->next;                                           \
    } else {                                                                   \
      prev_iterator->next = iterator->next;                                    \
                                                                               \
      if (iterator->next == NULL) {                                            \
        self->tail = prev_iterator;                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&iterator->data);                                              \
    }                                                                          \
                                                                               \
    free(iterator);                                                            \
                                                                               \
    --(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_POP_IDX(name, type)                                              \
  merr_t name##_mlist_pop_idx(const name##_mlist_t self, size_t idx) {         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    if (idx >= self->size) {                                                   \
      return M_IDX_OVERFLOW;                                                   \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
    name##_mlist_node_t prev_iterator = NULL;                                  \
                                                                               \
    while (idx--) {                                                            \
      prev_iterator = iterator;                                                \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (prev_iterator == NULL) {                                               \
      self->head = self->head->next;                                           \
    } else {                                                                   \
      prev_iterator->next = iterator->next;                                    \
                                                                               \
      if (iterator->next == NULL) {                                            \
        self->tail = prev_iterator;                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&iterator->data);                                              \
    }                                                                          \
                                                                               \
    free(iterator);                                                            \
                                                                               \
    --(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_ERASE(name, type)                                                \
  merr_t name##_mlist_erase(const name##_mlist_t self, size_t lt, size_t rt) { \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    if (lt > rt) {                                                             \
      size_t temp = lt;                                                        \
      lt = rt;                                                                 \
      rt = temp;                                                               \
    }                                                                          \
                                                                               \
    if (lt >= self->size) {                                                    \
      lt = self->size - 1;                                                     \
    }                                                                          \
                                                                               \
    if (rt >= self->size) {                                                    \
      rt = self->size - 1;                                                     \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
    name##_mlist_node_t prev_iterator = NULL;                                  \
                                                                               \
    size_t dl = rt - lt + 1;                                                   \
                                                                               \
    while (lt--) {                                                             \
      prev_iterator = iterator;                                                \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    self->size -= dl;                                                          \
                                                                               \
    while (dl--) {                                                             \
      if (prev_iterator == NULL) {                                             \
        self->head = self->head->next;                                         \
      } else {                                                                 \
        prev_iterator->next = iterator->next;                                  \
                                                                               \
        if (iterator->next == NULL) {                                          \
          self->tail = prev_iterator;                                          \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (self->frd != NULL) {                                                 \
        self->frd(&iterator->data);                                            \
      }                                                                        \
                                                                               \
      free(iterator);                                                          \
                                                                               \
      if (prev_iterator == NULL) {                                             \
        iterator = self->head;                                                 \
      } else {                                                                 \
        iterator = prev_iterator->next;                                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_FILTER(name, type)                                               \
  FILTER_FUNC(name, type)                                                      \
                                                                               \
  name##_mlist_t name##_mlist_filter(const name##_mlist_ptr_t *const self,     \
                                     name##_filter_func f) {                   \
    if ((self == NULL) || (self->head == NULL) || (f == NULL)) {               \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name##_mlist_t filter_list = name##_mlist(self->cmp, self->frd);           \
                                                                               \
    if (filter_list == NULL) {                                                 \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name##_mlist_node_t iterator = self->head;                                 \
                                                                               \
    while (iterator != NULL) {                                                 \
      if (f(&iterator->data) == mtrue) {                                       \
        name##_mlist_push(filter_list, iterator->data);                        \
      }                                                                        \
                                                                               \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (filter_list->head == NULL) {                                           \
      name##_mlist_free(&filter_list);                                         \
    }                                                                          \
                                                                               \
    return filter_list;                                                        \
  }

#define MLIST_MAP(name1, type1, name2, type2)                                  \
  MAP_FUNC(name1, type1, name2, type2)                                         \
  CMP_FUNC(name2, type2)                                                       \
  FREE_FUNC(name2, type2)                                                      \
                                                                               \
  name2##_mlist_t name1##_to_##name2##_mlist_map(                              \
      const name1##_mlist_ptr_t *const self, name1##_to_##name2##_map_func f,  \
      name2##_compare_func cmp, name2##_free_func frd) {                       \
    if ((self == NULL) || (self->head == NULL) || (f == NULL)) {               \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name2##_mlist_t map_list = name2##_mlist(cmp, frd);                        \
                                                                               \
    if (map_list == NULL) {                                                    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    name1##_mlist_node_t iterator = self->head;                                \
                                                                               \
    while (iterator != NULL) {                                                 \
      name2##_mlist_push(map_list, f(&iterator->data));                        \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (map_list == NULL) {                                                    \
      name2##_mlist_free(&map_list);                                           \
    }                                                                          \
                                                                               \
    return map_list;                                                           \
  }

#define MLIST_TRAVERSE(name, type)                                             \
  ACTION_FUNC(name, type)                                                      \
  merr_t name##_mlist_traverse(const name##_mlist_ptr_t *self,                 \
                               name##_action_func action) {                    \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      printf("[ ]\n");                                                         \
    } else {                                                                   \
      printf("[");                                                             \
      name##_mlist_node_t iterator = self->head;                               \
                                                                               \
      while (iterator != NULL) {                                               \
        action(&iterator->data);                                               \
        iterator = iterator->next;                                             \
      }                                                                        \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MLIST_ALL(name, type)                                                  \
  MLIST(name, type)                                                            \
  MLIST_EMPTY(name, type)                                                      \
  MLIST_SIZE(name, type)                                                       \
  MLIST_HEAD(name, type)                                                       \
  MLIST_TAIL(name, type)                                                       \
  MLIST_FIND_NODE(name, type)                                                  \
  MLIST_FIND_IDX(name, type)                                                   \
  MLIST_FIND(name, type)                                                       \
  MLIST_SWAP(name, type)                                                       \
  MLIST_CHANGE(name, type)                                                     \
  MLIST_PUSH(name, type)                                                       \
  MLIST_PUSH_ORDER(name, type)                                                 \
  MLIST_PUSH_FRONT(name, type)                                                 \
  MLIST_PUSH_IDX(name, type)                                                   \
  MLIST_POP(name, type)                                                        \
  MLIST_POP_IDX(name, type)                                                    \
  MLIST_ERASE(name, type)                                                      \
  MLIST_FILTER(name, type)                                                     \
  MLIST_TRAVERSE(name, type)                                                   \
  MLIST_MAP(name, type, name, type)

#endif /* MACROS_GENERIC_LIST_UTILS_H_ */