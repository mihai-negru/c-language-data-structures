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

/**
 * @brief Utility file containing macros that generate functions for generic
 * single linked list.
 *
 * @param ID the id of the structure in order to reduce name collisions.
 * @param T the type of the data stored inside the structure.
 */

/**
 * @brief Generates the `mlist_t` structure depending on the name and type.
 * Also generates basic function for creation and freeing memory for the
 * structure. This structure require a method for comparing data and for freeing
 * data memory, if data is not stored as a pointer (T <=> *M), then the free
 * function must be `NULL`. If data represents a structure which contains
 * pointers allocated, then the free function must free those fields.
 */
#define MLIST(ID, T)                                                           \
  CMP_FUNC(ID, T)                                                              \
  FREE_FUNC(ID, T)                                                             \
                                                                               \
  typedef struct ID##_mlist_node_s {                                           \
    T data;                                                                    \
    struct ID##_mlist_node_s *next;                                            \
  } ID##_mlist_node_ptr_t, *ID##_mlist_node_t;                                 \
                                                                               \
  typedef struct ID##_mlist_s {                                                \
    ID##_mlist_node_t head;                                                    \
    ID##_mlist_node_t tail;                                                    \
    ID##_compare_func cmp;                                                     \
    ID##_free_func frd;                                                        \
    size_t size;                                                               \
  } ID##_mlist_ptr_t, *ID##_mlist_t;                                           \
                                                                               \
  ID##_mlist_t ID##_mlist(ID##_compare_func cmp, ID##_free_func frd) {         \
    if (cmp == NULL) {                                                         \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mlist_t self = malloc(sizeof *self);                                  \
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
  ID##_mlist_node_t ID##_mlist_node(T data) {                                  \
    ID##_mlist_node_t self_node = malloc(sizeof *self_node);                   \
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
  merr_t ID##_mlist_free(ID##_mlist_t *self) {                                 \
    if ((self != NULL) || (*self != NULL)) {                                   \
      while ((*self)->head != NULL) {                                          \
        ID##_mlist_node_t iterator = (*self)->head;                            \
        (*self)->head = (*self)->head->next;                                   \
                                                                               \
        if ((*self)->frd != NULL) {                                            \
          (*self)->frd(&iterator->data);                                       \
        }                                                                      \
                                                                               \
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

/**
 * @brief Function to check if a linked list object is empty or not. The
 * function tests if head of list is `NULL` in that case function will return
 * true, otherwise it will return false. A `NULL` list is also considered as an
 * empty list.
 */
#define MLIST_EMPTY(ID, T)                                                     \
  mbool_t ID##_mlist_empty(const ID##_mlist_ptr_t *const self) {               \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

/**
 * @brief Get the list size object. If list is not allocated then function will
 * return SIZE_MAX value.
 */
#define MLIST_SIZE(ID, T)                                                      \
  size_t ID##_mlist_size(const ID##_mlist_ptr_t *const self) {                 \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

/**
 * @brief Get the list head node data, stored in an accumulator, the accumulator
 * should not be NULL.
 */
#define MLIST_HEAD(ID, T)                                                      \
  merr_t ID##_mlist_head(const ID##_mlist_ptr_t *const self, T *const acc) {   \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->head->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Get the list tail node data, stored in an accumulator, the accumulator
 * should not be NULL.
 */
#define MLIST_TAIL(ID, T)                                                      \
  merr_t ID##_mlist_tail(const ID##_mlist_ptr_t *const self, T *const acc) {   \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->tail == NULL) {                                                  \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->tail->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Function to find node that contains specific data provided by user. It
 * uses cmp function provided by user at the creation of the linked list.
 */
#define MLIST_FIND_NODE(ID, T)                                                 \
  ID##_mlist_node_t ID##_mlist_find_node(const ID##_mlist_ptr_t *const self,   \
                                         T data) {                             \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t iterator = self->head;                                   \
                                                                               \
    while ((iterator != NULL) && (self->cmp(&iterator->data, &data) != 0)) {   \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    return iterator;                                                           \
  }

/**
 * @brief Function to find a node from a list from a given index. Function will
 * fail if index is bigger than current size of list or list is not allocated,
 * the data is stored in a non NULL accumulator.
 */
#define MLIST_FIND_IDX(ID, T)                                                  \
  merr_t ID##_mlist_find_idx(const ID##_mlist_ptr_t *const self, size_t idx,   \
                             T *const acc) {                                   \
    if ((self == NULL) || (idx >= self->size) || (acc == NULL)) {              \
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
    ID##_mlist_node_t iterator = self->head;                                   \
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

/**
 * @brief Function to find node that contains specific data provided by user. It
 * uses cmp function provided by user at the creation of the linked list, the
 * data is palced in a non NULL accumulator.
 */
#define MLIST_FIND(ID, T)                                                      \
  merr_t ID##_mlist_find(const ID##_mlist_ptr_t *const self, T data,           \
                         T *const acc) {                                       \
    if ((self == NULL) || (self->head == NULL)) {                              \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t iterator = self->head;                                   \
                                                                               \
    while ((iterator != NULL) && (self->cmp(&iterator->data, &data) != 0)) {   \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (iterator != NULL) {                                                    \
      if (acc != NULL) {                                                       \
        *acc = iterator->data;                                                 \
      }                                                                        \
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_NOT_FOUND;                                                        \
  }

/**
 * @brief Function to swap data between two list nodes. If nodes are `NULL` or
 * the same then no operation will be executed. Function will swap data not node
 * pointers. Function may fail if list is not allocated
 */
#define MLIST_SWAP(ID, T)                                                      \
  merr_t ID##_mlist_swap(const ID##_mlist_t self, T fst, T snd) {              \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t fst_node = ID##_mlist_find_node(self, fst);              \
                                                                               \
    if (fst_node == NULL) {                                                    \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t snd_node = ID##_mlist_find_node(self, snd);              \
                                                                               \
    if (snd_node == NULL) {                                                    \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    if (fst_node != snd_node) {                                                \
      T temp = fst_node->data;                                                 \
      fst_node->data = snd_node->data;                                         \
      snd_node->data = temp;                                                   \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Function to swap data between two list nodes. The index sizes should
 * not exceed the size of the list.
 */
#define MLIST_SWAP_IDX(ID, T)                                                  \
  merr_t ID##_mlist_swap_idx(const ID##_mlist_t self, size_t fst,              \
                             size_t snd) {                                     \
    if ((self == NULL) || (fst > self->size - 1) || (snd > self->size - 1)) {  \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (fst == snd) {                                                          \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t fst_node = self->head;                                   \
    ID##_mlist_node_t snd_node = self->head;                                   \
                                                                               \
    while ((fst > 0) || (snd > 0)) {                                           \
      if (fst > 0) {                                                           \
        fst_node = fst_node->next;                                             \
        --fst;                                                                 \
      }                                                                        \
                                                                               \
      if (snd > 0) {                                                           \
        snd_node = snd_node->next;                                             \
        --snd;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    T temp = fst_node->data;                                                   \
    fst_node->data = snd_node->data;                                           \
    snd_node->data = temp;                                                     \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Changes the data to a new data. If the old data is not found an error
 * is returned.
 */
#define MLIST_CHANGE(ID, T)                                                    \
  merr_t ID##_mlist_change(const ID##_mlist_t self, T base, T new) {           \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t base_node = ID##_mlist_find_node(self, base);            \
                                                                               \
    if (base_node == NULL) {                                                   \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    base_node->data = new;                                                     \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Changes the data to a new data. If the idx is greater than the size of
 * the list an error is returned.
 */
#define MLIST_CHANGE_IDX(ID, T)                                                \
  merr_t ID##_mlist_change_idx(const ID##_mlist_t self, size_t idx, T new) {   \
    if ((self == NULL) || (idx > self->size - 1)) {                            \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t base_node = self->head;                                  \
                                                                               \
    while (idx--) {                                                            \
      base_node = base_node->next;                                             \
    }                                                                          \
                                                                               \
    base_node->data = new;                                                     \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Inserts an element to the end of the linked list.
 */
#define MLIST_PUSH(ID, T)                                                      \
  merr_t ID##_mlist_push(const ID##_mlist_t self, T data) {                    \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t self_node = ID##_mlist_node(data);                       \
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

/**
 * @brief Function to insert an element in order in the list. Function will find
 * the position of the new elements according to cmp function provided at the
 * creation of the list.
 */
#define MLIST_PUSH_ORDER(ID, T)                                                \
  merr_t ID##_mlist_push_order(const ID##_mlist_t self, T data) {              \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t self_node = ID##_mlist_node(data);                       \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      self->head = self_node;                                                  \
      self->tail = self_node;                                                  \
    } else {                                                                   \
      ID##_mlist_node_t iterator = self->head;                                 \
      ID##_mlist_node_t prev_iterator = NULL;                                  \
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

/**
 * @brief Inserts a new element in front of the list.
 */
#define MLIST_PUSH_FRONT(ID, T)                                                \
  merr_t ID##_mlist_push_front(const ID##_mlist_t self, T data) {              \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t self_node = ID##_mlist_node(data);                       \
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

/**
 * @brief Inserts an element at a specified index in the list. If index is
 * bigger than current list size than element will be inserted at the end of the
 * list.
 */
#define MLIST_PUSH_IDX(ID, T)                                                  \
  merr_t ID##_mlist_push_idx(const ID##_mlist_t self, T data, size_t idx) {    \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (idx >= self->size) {                                                   \
      return ID##_mlist_push(self, data);                                      \
    }                                                                          \
                                                                               \
    if (idx == 0)                                                              \
      return ID##_mlist_push_front(self, data);                                \
                                                                               \
    ID##_mlist_node_t self_node = ID##_mlist_node(data);                       \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t iterator = self->head;                                   \
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

/**
 * @brief Deletes a node based on a value. Program will recieve a list and a
 * pointer to data that user wants to be deleted. However data pointer has to be
 * valid and to exist in the current list (If you are not sure that data exists
 * you should not call mlist_find_data because delete function will find it by
 * itself and in case it does not exist it will return an error).
 */
#define MLIST_POP(ID, T)                                                       \
  merr_t ID##_mlist_pop(const ID##_mlist_t self, T data) {                     \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->head == NULL) {                                                  \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t iterator = self->head;                                   \
    ID##_mlist_node_t prev_iterator = NULL;                                    \
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

/**
 * @brief Deletes a node based on an index. Program will recieve a list and a
 * index from which element will be erased. If data_index is bigger than actual
 * size of the list then function will fail its execution and will return an
 * error. It is necessary for list to be allocated and not be be empty.
 */
#define MLIST_POP_IDX(ID, T)                                                   \
  merr_t ID##_mlist_pop_idx(const ID##_mlist_t self, size_t idx) {             \
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
    ID##_mlist_node_t iterator = self->head;                                   \
    ID##_mlist_node_t prev_iterator = NULL;                                    \
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

/**
 * @brief Erases a set of nodes from range [left_index; right_index]. If
 * left_index is greater than right_index that they will be swapped. If
 * right_index is bigger than actual size of the list right_index will be
 * updated to the end of the list. If both left and right index are bigger than
 * actual list size than the last element from linked object will be removed.
 */
#define MLIST_ERASE(ID, T)                                                     \
  merr_t ID##_mlist_erase(const ID##_mlist_t self, size_t lt, size_t rt) {     \
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
    ID##_mlist_node_t iterator = self->head;                                   \
    ID##_mlist_node_t prev_iterator = NULL;                                    \
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

/**
 * @brief Filters a given linked list object. User has to provide a function
 * that return mtrue(1) or mfalse(0). If filter function return 1 for an item
 * then it will be added in a new linked list, otherwise item will not be
 * inserted. If no element was inserted in the new linked list than the list
 * will be automatically erased from memory
 */
#define MLIST_FILTER(ID, T)                                                    \
  FILTER_FUNC(ID, T)                                                           \
                                                                               \
  ID##_mlist_t ID##_mlist_filter(const ID##_mlist_ptr_t *const self,           \
                                 ID##_filter_func f) {                         \
    if ((self == NULL) || (self->head == NULL) || (f == NULL)) {               \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mlist_t filter_list = ID##_mlist(self->cmp, self->frd);               \
                                                                               \
    if (filter_list == NULL) {                                                 \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mlist_node_t iterator = self->head;                                   \
                                                                               \
    while (iterator != NULL) {                                                 \
      if (f(&iterator->data) == mtrue) {                                       \
        ID##_mlist_push(filter_list, iterator->data);                          \
      }                                                                        \
                                                                               \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (filter_list->head == NULL) {                                           \
      ID##_mlist_free(&filter_list);                                           \
    }                                                                          \
                                                                               \
    return filter_list;                                                        \
  }

/**
 * @brief Traverses all list and do action on all data nodes.
 */
#define MLIST_TRAVERSE(ID, T)                                                  \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  merr_t ID##_mlist_traverse(const ID##_mlist_ptr_t *self,                     \
                             ID##_action_func action) {                        \
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
      ID##_mlist_node_t iterator = self->head;                                 \
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

/**
 * @brief Maps a linked list from first type to second type, by providing a
 * function that maps the data. By default a list has mapping from type to type,
 * if type1 is not equal to type2 then the user should declare a new `MLIST_MAP`
 * for the desired type.
 */
#define MLIST_MAP(ID1, T1, ID2, T2)                                            \
  MAP_FUNC(ID1, T1, ID2, T2)                                                   \
  CMP_FUNC(ID2, T2)                                                            \
  FREE_FUNC(ID2, T2)                                                           \
                                                                               \
  ID2##_mlist_t ID1##_to_##ID2##_mlist_map(                                    \
      const ID1##_mlist_ptr_t *const self, ID1##_to_##ID2##_map_func f,        \
      ID2##_compare_func cmp, ID2##_free_func frd) {                           \
    if ((self == NULL) || (self->head == NULL) || (f == NULL)) {               \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID2##_mlist_t map_list = ID2##_mlist(cmp, frd);                            \
                                                                               \
    if (map_list == NULL) {                                                    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID1##_mlist_node_t iterator = self->head;                                  \
                                                                               \
    while (iterator != NULL) {                                                 \
      ID2##_mlist_push(map_list, f(&iterator->data));                          \
      iterator = iterator->next;                                               \
    }                                                                          \
                                                                               \
    if (map_list == NULL) {                                                    \
      ID2##_mlist_free(&map_list);                                             \
    }                                                                          \
                                                                               \
    return map_list;                                                           \
  }

/**
 * @brief Adds the all API for the `mlist_t` structure (linked list). You will
 * not be always need to use all the API, in this case you must be sure that you
 * call `MLIST` for definition of the linked list, any other macro definitions
 * are to bring new functionalities. The code length may be reduced a lot if you
 * do not call `MLIST_ALL`, the code duplicated when calling `MLIST_ALL` for
 * different ids or different types, it is encoureged to not to use `MLIST_ALL`
 * or not to declare different ids for the same type. The ID protocol is used
 * when different files want to have the same two typed structures in order to
 * avoid name collisions.
 */
#define MLIST_ALL(ID, T)                                                       \
  MLIST(ID, T)                                                                 \
  MLIST_EMPTY(ID, T)                                                           \
  MLIST_SIZE(ID, T)                                                            \
  MLIST_HEAD(ID, T)                                                            \
  MLIST_TAIL(ID, T)                                                            \
  MLIST_FIND_NODE(ID, T)                                                       \
  MLIST_FIND_IDX(ID, T)                                                        \
  MLIST_FIND(ID, T)                                                            \
  MLIST_SWAP(ID, T)                                                            \
  MLIST_SWAP_IDX(ID, T)                                                        \
  MLIST_CHANGE(ID, T)                                                          \
  MLIST_CHANGE_IDX(ID, T)                                                      \
  MLIST_PUSH(ID, T)                                                            \
  MLIST_PUSH_ORDER(ID, T)                                                      \
  MLIST_PUSH_FRONT(ID, T)                                                      \
  MLIST_PUSH_IDX(ID, T)                                                        \
  MLIST_POP(ID, T)                                                             \
  MLIST_POP_IDX(ID, T)                                                         \
  MLIST_ERASE(ID, T)                                                           \
  MLIST_FILTER(ID, T)                                                          \
  MLIST_TRAVERSE(ID, T)                                                        \
  MLIST_MAP(ID, T, ID, T)

#endif /* MACROS_GENERIC_LIST_UTILS_H_ */
