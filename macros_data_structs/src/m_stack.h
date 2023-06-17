/**
 * @file m_stack.h
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

#ifndef MACROS_GENERICS_STACK_UTILS_H_
#define MACROS_GENERICS_STACK_UTILS_H_

#include "./m_config.h"

/**
 * @brief Utility file containing macros that generate functions for generic
 * single linked list.
 *
 * @param ID the id of the structure in order to reduce name collisions.
 * @param T the type of the data stored inside the structure.
 */

/**
 * @brief Generates the `mstack_t` structure depending on the name and type.
 * Also generates basic function for creation and freeing memory for the
 * structure. This structure require a method for freeing data memory, if data
 * is not stored as a pointer (T <=> *M), then the free function must be `NULL`.
 * If data represents a structure which contains pointers allocated, then the
 * free function must free those fields.
 */
#define MSTACK(T, ID)                                                          \
  FREE_FUNC(ID, T)                                                             \
                                                                               \
  typedef struct ID##_mstack_node_s {                                          \
    T data;                                                                    \
    struct ID##_mstack_node_s *next;                                           \
  } ID##_mstack_node_ptr_t, *ID##_mstack_node_t;                               \
                                                                               \
  typedef struct ID##_mstack_s {                                               \
    ID##_mstack_node_t top;                                                    \
    ID##_free_func frd;                                                        \
    size_t size;                                                               \
  } ID##_mstack_ptr_t, *ID##_mstack_t;                                         \
                                                                               \
  ID##_mstack_t ID##_mstack(ID##_free_func frd) {                              \
    ID##_mstack_t self = malloc(sizeof *self);                                 \
                                                                               \
    if (self == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->frd = frd;                                                           \
                                                                               \
    self->top = NULL;                                                          \
    self->size = 0;                                                            \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  ID##_mstack_node_t ID##_mstack_node(T data) {                                \
    ID##_mstack_node_t self_node = malloc(sizeof *self_node);                  \
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
  merr_t ID##_mstack_free(ID##_mstack_t *self) {                               \
    if ((self != NULL) || (*self != NULL)) {                                   \
      while ((*self)->top != NULL) {                                           \
        ID##_mstack_node_t iterator = (*self)->top;                            \
        (*self)->top = (*self)->top->next;                                     \
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
 * @brief Traverses all stack and do action(basically it is used for printing
 * the stack) on all data nodes.
 */
#define MSTACK_TRAVERSE(T, ID)                                                 \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  merr_t ID##_mstack_traverse(const ID##_mstack_ptr_t *self,                   \
                              ID##_action_func action) {                       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->top == NULL) {                                                   \
      printf("[ ]\n");                                                         \
    } else {                                                                   \
      printf("[");                                                             \
      ID##_mstack_node_t iterator = self->top;                                 \
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
 * @brief Function to check if a stack object is empty or not. The
 * function tests if top of list is `NULL` in that case function will return
 * true, otherwise it will return false. A `NULL` stack is also considered as an
 * empty stack.
 */
#define MSTACK_EMPTY(T, ID)                                                    \
  mbool_t ID##_mstack_empty(const ID##_mstack_ptr_t *const self) {             \
    if ((self == NULL) || (self->top == NULL)) {                               \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

/**
 * @brief Get the stack size object. If stack is not allocated then function
 * will return SIZE_MAX value.
 */
#define MSTACK_SIZE(T, ID)                                                     \
  size_t ID##_mstack_size(const ID##_mstack_ptr_t *const self) {               \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

/**
 * @brief Get the stack top node data, stored in an accumulator, the accumulator
 * should not be NULL.
 */
#define MSTACK_TOP(T, ID)                                                      \
  merr_t ID##_mstack_top(const ID##_mstack_ptr_t *const self, T *const acc) {  \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->top == NULL) {                                                   \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->top->data;                                                    \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Inserts an element to the end of the stack.
 */
#define MSTACK_PUSH(T, ID)                                                     \
  merr_t ID##_mstack_push(const ID##_mstack_t self, T data) {                  \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mstack_node_t self_node = ID##_mstack_node(data);                     \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (self->top == NULL) {                                                   \
      self->top = self_node;                                                   \
    } else {                                                                   \
      self_node->next = self->top;                                             \
      self->top = self_node;                                                   \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Removed the top element from the stack object, or returs an error if
 * the stack was empty or `NULL`.
 */
#define MSTACK_POP(T, ID)                                                      \
  merr_t ID##_mstack_pop(const ID##_mstack_t self) {                           \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((self->top == NULL) || (self->size == 0)) {                            \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mstack_node_t self_node = self->top;                                  \
    self->top = self->top->next;                                               \
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
 * @brief Adds the all API for the `mstack_t` structure. You will not be always
 * need to use all the API, in this case you must be sure that you call `MSTACK`
 * for definition of the stack, any other macro definitions are to bring
 * new functionalities. The code length may be reduced a lot if you do not call
 * `MSTACK_ALL`, the code duplicated when calling `MSTACK_ALL` for different ids
 * or different types, it is encoureged to not to use `MSTACK_ALL` or not to
 * declare different ids for the same type. The ID protocol is used when
 * different files want to have the same two typed structures in order to avoid
 * name collisions.
 */
#define MSTACK_ALL(T, ID)                                                      \
  MSTACK(T, ID)                                                                \
  MSTACK_TRAVERSE(T, ID)                                                       \
  MSTACK_EMPTY(T, ID)                                                          \
  MSTACK_SIZE(T, ID)                                                           \
  MSTACK_TOP(T, ID)                                                            \
  MSTACK_PUSH(T, ID)                                                           \
  MSTACK_POP(T, ID)

#endif /* MACROS_GENERIC_STACK_UTILS_H_ */
