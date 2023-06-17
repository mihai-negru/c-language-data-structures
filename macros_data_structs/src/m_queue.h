/**
 * @file m_queue.h
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

#ifndef MACROS_GENERICS_QUEUE_UTILS_H_
#define MACROS_GENERICS_QUEUE_UTILS_H_

#include "./m_config.h"

/**
 * @brief Utility file containing macros that generate functions for generic
 * single linked list.
 *
 * @param ID the id of the structure in order to reduce name collisions.
 * @param T the type of the data stored inside the structure.
 */

/**
 * @brief Generates the `mqueue_t` structure depending on the name and type.
 * Also generates basic function for creation and freeing memory for the
 * structure. This structure require a method for freeing data memory, if data
 * is not stored as a pointer (T <=> *M), then the free function must be `NULL`.
 * If data represents a structure which contains pointers allocated, then the
 * free function must free those fields.
 */
#define MQUEUE(ID, T)                                                          \
  FREE_FUNC(ID, T)                                                             \
                                                                               \
  typedef struct ID##_mqueue_node_s {                                          \
    T data;                                                                    \
    struct ID##_mqueue_node_s *next;                                           \
  } ID##_mqueue_node_ptr_t, *ID##_mqueue_node_t;                               \
                                                                               \
  typedef struct ID##_mqueue_s {                                               \
    ID##_mqueue_node_t front;                                                  \
    ID##_mqueue_node_t back;                                                   \
    ID##_free_func frd;                                                        \
    size_t size;                                                               \
  } ID##_mqueue_ptr_t, *ID##_mqueue_t;                                         \
                                                                               \
  ID##_mqueue_t ID##_mqueue(ID##_free_func frd) {                              \
    ID##_mqueue_t self = malloc(sizeof *self);                                 \
                                                                               \
    if (self == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->frd = frd;                                                           \
                                                                               \
    self->front = self->back = NULL;                                           \
    self->size = 0;                                                            \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  ID##_mqueue_node_t ID##_mqueue_node(T data) {                                \
    ID##_mqueue_node_t self_node = malloc(sizeof *self_node);                  \
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
  merr_t ID##_mqueue_free(ID##_mqueue_t *self) {                               \
    if ((self != NULL) && (*self != NULL)) {                                   \
      while ((*self)->front != NULL) {                                         \
        ID##_mqueue_node_t iterator = (*self)->front;                          \
        (*self)->front = (*self)->front->next;                                 \
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
 * @brief Traverses all queue and do action(basically it is used for printing
 * the queue) on all data nodes.
 */
#define MQUEUE_TRAVERSE(ID, T)                                                 \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  merr_t ID##_mqueue_traverse(const ID##_mqueue_ptr_t *self,                   \
                              ID##_action_func action) {                       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->front == NULL) {                                                 \
      printf("[ ]\n");                                                         \
    } else {                                                                   \
      printf("[");                                                             \
      ID##_mqueue_node_t iterator = self->front;                               \
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
 * @brief Function to check if a queue object is empty or not. The
 * function tests if top of list is `NULL` in that case function will return
 * true, otherwise it will return false. A `NULL` queue is also considered as an
 * empty queue.
 */
#define MQUEUE_EMPTY(ID, T)                                                    \
  mbool_t ID##_mqueue_empty(const ID##_mqueue_ptr_t *const self) {             \
    if ((self == NULL) || (self->front == NULL)) {                             \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

/**
 * @brief Get the queue size object. If queue is not allocated then function
 * will return SIZE_MAX value.
 */
#define MQUEUE_SIZE(ID, T)                                                     \
  size_t ID##_mqueue_size(const ID##_mqueue_ptr_t *const self) {               \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

/**
 * @brief Get the queue front node data, stored in an accumulator, the
 * accumulator should not be NULL.
 */
#define MQUEUE_FRONT(ID, T)                                                    \
  merr_t ID##_mqueue_front(const ID##_mqueue_ptr_t *const self,                \
                           T *const acc) {                                     \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->front == NULL) {                                                 \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->front->data;                                                  \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Get the queue back node data, stored in an accumulator, the
 * accumulator should not be NULL.
 */
#define MQUEUE_BACK(ID, T)                                                     \
  merr_t ID##_mqueue_back(const ID##_mqueue_ptr_t *const self, T *const acc) { \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->back == NULL) {                                                  \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->back->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Inserts an element to the end of the queue.
 */
#define MQUEUE_PUSH(ID, T)                                                     \
  merr_t ID##_mqueue_push(const ID##_mqueue_t self, T data) {                  \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mqueue_node_t self_node = ID##_mqueue_node(data);                     \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->front == NULL) {                                                 \
      self->front = self_node;                                                 \
      self->back = self_node;                                                  \
    } else {                                                                   \
      self->back->next = self_node;                                            \
      self->back = self_node;                                                  \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Removed the front element from the queue object, or returs an error if
 * the queue was empty or `NULL`.
 */
#define MQUEUE_POP(ID, T)                                                      \
  merr_t ID##_mqueue_pop(const ID##_mqueue_t self) {                           \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((self->front == NULL) || (self->size == 0)) {                          \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mqueue_node_t self_node = self->front;                                \
    self->front = self->front->next;                                           \
    --(self->size);                                                            \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&self_node->data);                                             \
    }                                                                          \
                                                                               \
    free(self_node);                                                           \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Adds the all API for the `mqueue_t` structure. You will not be always
 * need to use all the API, in this case you must be sure that you call `MQUEUE`
 * for definition of the queue, any other macro definitions are to bring
 * new functionalities. The code length may be reduced a lot if you do not call
 * `MQUEUE_ALL`, the code duplicated when calling `MQUEUE_ALL` for different ids
 * or different types, it is encoureged to not to use `MQUEUE_ALL` or not to
 * declare different ids for the same type. The ID protocol is used when
 * different files want to have the same two typed structures in order to avoid
 * name collisions.
 */
#define MQUEUE_ALL(ID, T)                                                      \
  MQUEUE(ID, T)                                                                \
  MQUEUE_TRAVERSE(ID, T)                                                       \
  MQUEUE_EMPTY(ID, T)                                                          \
  MQUEUE_SIZE(ID, T)                                                           \
  MQUEUE_FRONT(ID, T)                                                          \
  MQUEUE_BACK(ID, T)                                                           \
  MQUEUE_PUSH(ID, T)                                                           \
  MQUEUE_POP(ID, T)

#endif /* MACROS_GENERIC_QUEUE_UTILS_H_ */
