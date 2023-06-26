/**
 * @file m_pqueue.h
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

#ifndef MACROS_GENERICS_PRIORITY_QUEUE_UTILS_H_
#define MACROS_GENERICS_PRIORITY_QUEUE_UTILS_H_

#include "./m_config.h"

/**
 * @brief Utility file containing macros that generate functions for generic
 * single linked list.
 *
 * @param ID the id of the structure in order to reduce name collisions.
 * @param T the type of the data stored inside the structure.
 */

#define MPQUEUE_DEFAULT_CAPACITY 10
#define MPQUEUE_DEFAULT_REALLOC_RATIO 2

#define MPQUEUE_INTERNAL_LT_IDX(idx) (2 * (idx) + 1)
#define MPQUEUE_INTERNAL_RT_IDX(idx) (2 * (idx) + 2)
#define MPQUEUE_INTERNAL_PR_IDX(idx) (((idx)-1) / 2)

/**
 * @brief Defines the structure for a priority queue as a heap, the compare
 * functions defines if the heap is a max or a min heap (natural order $1 > $2
 * -> 1 means max, otherwise means min). The priority queue can be used just
 * with priorities and data field can be skipped, however it limits the api, by
 * impossibility to use some functions like `find_idx`, which relates to data
 * not to priorities. The priorities should be non-transparent for the user,
 * however the api let's the user to access, not to modify the priorities of the
 * heap. This structure require two methods for freeing data memory, if
 * data(priority) is not stored as a pointer (T <=> *M), then the free function
 * must be `NULL`. If data represents a structure which contains pointers
 * allocated, then the free function must free those fields.
 */
#define MPQUEUE(ID, K, V)                                                      \
  CMP_FUNC(ID##K, K)                                                           \
  FREE_FUNC(ID##K, K)                                                          \
                                                                               \
  CMP_FUNC(ID##V, V)                                                           \
  FREE_FUNC(ID##V, V)                                                          \
                                                                               \
  typedef struct ID##_mpqueue_node_s {                                         \
    K prio;                                                                    \
    V data;                                                                    \
  } ID##_mpqueue_node_ptr_t, *ID##_mpqueue_node_t;                             \
                                                                               \
  typedef struct ID##_mpqueue_s {                                              \
    ID##_mpqueue_node_t *nodes;                                                \
    ID##K##_compare_func cmp_prio;                                             \
    ID##K##_free_func frd_prio;                                                \
    ID##V##_compare_func cmp_data;                                             \
    ID##V##_free_func frd_data;                                                \
    size_t capacity;                                                           \
    size_t size;                                                               \
  } ID##_mpqueue_ptr_t, *ID##_mpqueue_t;                                       \
                                                                               \
  ID##_mpqueue_t ID##_mpqueue(                                                 \
      size_t init_capacity, ID##K##_compare_func cmp_prio,                     \
      ID##K##_free_func frd_prio, ID##V##_compare_func cmp_data,               \
      ID##V##_free_func frd_data) {                                            \
    if (cmp_prio == NULL) {                                                    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    if (init_capacity < MPQUEUE_DEFAULT_CAPACITY) {                            \
      init_capacity = MPQUEUE_DEFAULT_CAPACITY;                                \
    }                                                                          \
                                                                               \
    ID##_mpqueue_t self = malloc(sizeof *self);                                \
                                                                               \
    if (self == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->nodes = malloc(sizeof *self->nodes * init_capacity);                 \
                                                                               \
    if (self->nodes == NULL) {                                                 \
      free(self);                                                              \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->cmp_prio = cmp_prio;                                                 \
    self->frd_prio = frd_prio;                                                 \
    self->cmp_data = cmp_data;                                                 \
    self->frd_data = frd_data;                                                 \
                                                                               \
    self->capacity = init_capacity;                                            \
    self->size = 0;                                                            \
                                                                               \
    for (size_t iter = 0; iter < init_capacity; ++iter) {                      \
      self->nodes[iter] = NULL;                                                \
    }                                                                          \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  ID##_mpqueue_node_t ID##_internal_mpqueue_node(const K *const prio,          \
                                                 const V *const data) {        \
    ID##_mpqueue_node_t self_node = malloc(sizeof *self_node);                 \
                                                                               \
    if (self_node == NULL) {                                                   \
      return NULL;                                                             \
    }                                                                          \
    memset(&self_node->prio, 0, sizeof self_node->prio);                       \
    memset(&self_node->data, 0, sizeof self_node->data);                       \
                                                                               \
    if (prio != NULL) {                                                        \
      self_node->prio = *prio;                                                 \
    }                                                                          \
                                                                               \
    if (data != NULL) {                                                        \
      self_node->data = *data;                                                 \
    }                                                                          \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mpqueue_free(ID##_mpqueue_t *self) {                             \
    if ((self != NULL) && (*self != NULL)) {                                   \
      if ((*self)->nodes != NULL) {                                            \
        for (size_t iter = 0; iter < (*self)->capacity; ++iter) {              \
          if ((*self)->nodes[iter] != NULL) {                                  \
            if ((*self)->frd_prio != NULL) {                                   \
              (*self)->frd_prio(&(*self)->nodes[iter]->prio);                  \
            }                                                                  \
                                                                               \
            if ((*self)->frd_data != NULL) {                                   \
              (*self)->frd_data(&(*self)->nodes[iter]->data);                  \
            }                                                                  \
                                                                               \
            free((*self)->nodes[iter]);                                        \
          }                                                                    \
        }                                                                      \
                                                                               \
        free((*self)->nodes);                                                  \
      }                                                                        \
                                                                               \
      free(*self);                                                             \
      *self = NULL;                                                            \
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }                                                                            \
                                                                               \
  void ID##_internal_mpqueue_sift_up(const ID##_mpqueue_ptr_t *const self,     \
                                     size_t idx) {                             \
    while ((idx > 0) &&                                                        \
           (self->cmp_prio(                                                    \
                &self->nodes[idx]->prio,                                       \
                &self->nodes[MPQUEUE_INTERNAL_PR_IDX(idx)]->prio) >= 1)) {     \
      ID##_mpqueue_node_t temp = self->nodes[idx];                             \
      self->nodes[idx] = self->nodes[MPQUEUE_INTERNAL_PR_IDX(idx)];            \
      self->nodes[MPQUEUE_INTERNAL_PR_IDX(idx)] = temp;                        \
                                                                               \
      idx = MPQUEUE_INTERNAL_PR_IDX(idx);                                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  void ID##_internal_mpqueue_sift_down(const ID##_mpqueue_ptr_t *const self,   \
                                       size_t idx) {                           \
    size_t swap_idx = idx;                                                     \
                                                                               \
    size_t check_idx = MPQUEUE_INTERNAL_LT_IDX(idx);                           \
                                                                               \
    if ((check_idx < self->size) &&                                            \
        (self->cmp_prio(&self->nodes[check_idx]->prio,                         \
                        &self->nodes[swap_idx]->prio) >= 1)) {                 \
      swap_idx = check_idx;                                                    \
    }                                                                          \
                                                                               \
    check_idx = MPQUEUE_INTERNAL_RT_IDX(idx);                                  \
    if ((check_idx < self->size) &&                                            \
        (self->cmp_prio(&self->nodes[check_idx]->prio,                         \
                        &self->nodes[swap_idx]->prio) >= 1)) {                 \
      swap_idx = check_idx;                                                    \
    }                                                                          \
                                                                               \
    if (idx != swap_idx) {                                                     \
      ID##_mpqueue_node_t temp = self->nodes[idx];                             \
      self->nodes[idx] = self->nodes[swap_idx];                                \
      self->nodes[swap_idx] = temp;                                            \
                                                                               \
      ID##_internal_mpqueue_sift_down(self, swap_idx);                         \
    }                                                                          \
  }

/**
 * @brief Creates a priority queue based on heapify method which runs in O(n)
 * time complexity. If a big chunck if data is known is recommended to use this
 * method because is faster than inserting each element. The function has to get
 * an empty priority queue in order to make the heap correctly. This function
 * can be used once per priority queue and it is a must to be called when the
 * object is empty.
 */
#define MPQUEUE_HEAPIFY(ID, K, V)                                              \
  merr_t ID##_mpqueue_heapify(ID##_mpqueue_t self_empty, size_t heap_size,     \
                              const K *const prios, const V *const data) {     \
    if ((self_empty == NULL) || (prios == NULL)) {                             \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((self_empty->size != 0) || (self_empty->nodes == NULL) ||              \
        (heap_size > self_empty->capacity)) {                                  \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    for (size_t iter = 0; iter < heap_size; ++iter) {                          \
      ID##_mpqueue_node_t self_node = NULL;                                    \
                                                                               \
      if (data != NULL) {                                                      \
        self_node = ID##_internal_mpqueue_node(prios + iter, data + iter);     \
      } else {                                                                 \
        self_node = ID##_internal_mpqueue_node(prios + iter, NULL);            \
      }                                                                        \
                                                                               \
      if (self_node == NULL) {                                                 \
        return M_MALLOC_FAILED;                                                \
      }                                                                        \
                                                                               \
      self_empty->nodes[iter] = self_node;                                     \
      ++(self_empty->size);                                                    \
    }                                                                          \
                                                                               \
    for (int64_t iter = (int64_t)(self_empty->size / 2 - 1); iter >= 0;        \
         --iter) {                                                             \
      ID##_internal_mpqueue_sift_down(self_empty, (size_t)iter);               \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Checks if the object is empty or not.
 */
#define MPQUEUE_EMPTY(ID, K, V)                                                \
  mbool_t ID##_mpqueue_empty(const ID##_mpqueue_ptr_t *const self) {           \
    if ((self == NULL) || (self->nodes == NULL) || (self->size == 0)) {        \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

/**
 * @brief Gets the size of the object.
 */
#define MPQUEUE_SIZE(ID, K, V)                                                 \
  size_t ID##_mpqueue_size(const ID##_mpqueue_ptr_t *const self) {             \
    if ((self == NULL) || (self->nodes)) {                                     \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

/**
 * @brief Changes the priority of a node, should be called after you find the
 * index with `find_pri_idx` method. Sifts Up or Down the node regarding to its
 * new priority.
 */
#define MPQUEUE_CHANGE_PRI(ID, K, V)                                           \
  merr_t ID##_mpqueue_change_pri(const ID##_mpqueue_ptr_t *const self,         \
                                 size_t idx, K prio) {                         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (idx >= self->size) {                                                   \
      return M_IDX_OVERFLOW;                                                   \
    }                                                                          \
                                                                               \
    if (self->cmp_prio(&self->nodes[idx]->prio, &prio) >= 1) {                 \
      if (self->frd_prio != NULL) {                                            \
        self->frd_prio(&self->nodes[idx]->prio);                               \
      }                                                                        \
      self->nodes[idx]->prio = prio;                                           \
                                                                               \
      ID##_internal_mpqueue_sift_down(self, idx);                              \
    } else if (self->cmp_prio(&self->nodes[idx]->prio, &prio) <= -1) {         \
      if (self->frd_prio != NULL) {                                            \
        self->frd_prio(&self->nodes[idx]->prio);                               \
      }                                                                        \
      self->nodes[idx]->prio = prio;                                           \
                                                                               \
      ID##_internal_mpqueue_sift_up(self, idx);                                \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Changes the data of a node, should be called after you find the index
 * of the data node using the `find_idx` function.
 */
#define MPQUEUE_CHANGE(ID, K, V)                                               \
  merr_t ID##_mpqueue_change(const ID##_mpqueue_ptr_t *const self, size_t idx, \
                             V data) {                                         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (idx >= self->size) {                                                   \
      return M_IDX_OVERFLOW;                                                   \
    }                                                                          \
                                                                               \
    if (self->frd_data != NULL) {                                              \
      self->frd_data(&self->nodes[idx]->data);                                 \
    }                                                                          \
    self->nodes[idx]->data = data;                                             \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Find the index of a priority, the `acc` fild can be NULL if you do not
 * want to store the index and just to find out if the heap contains the
 * priority.
 */
#define MPQUEUE_FIND_IDX(ID, K, V)                                             \
  merr_t ID##_mpqueue_find_idx(const ID##_mpqueue_ptr_t *const self, V data,   \
                               size_t *const acc) {                            \
    if ((self == NULL) || (self->nodes == NULL) || (self->cmp_data == NULL)) { \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    for (size_t iter = 0; iter < self->size; ++iter) {                         \
      if (self->nodes[iter] != NULL) {                                         \
        if (self->cmp_data(&self->nodes[iter]->data, &data) == 0) {            \
          if (acc != NULL) {                                                   \
            *acc = iter;                                                       \
          }                                                                    \
                                                                               \
          return M_OK;                                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    return M_NOT_FOUND;                                                        \
  }

/**
 * @brief Find the index of a data node, the `acc` fild can be NULL if you do
 * not want to store the index and just to find out if the heap contains the
 * data.
 */
#define MPQUEUE_FIND_PRI_IDX(ID, K, V)                                         \
  merr_t ID##_mpqueue_find_pri_idx(const ID##_mpqueue_ptr_t *const self,       \
                                   K prio, size_t *const acc) {                \
    if ((self == NULL) || (self->nodes == NULL)) {                             \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    for (size_t iter = 0; iter < self->size; ++iter) {                         \
      if (self->nodes[iter] != NULL) {                                         \
        if (self->cmp_prio(&self->nodes[iter]->prio, &prio) == 0) {            \
          if (acc != NULL) {                                                   \
            *acc = iter;                                                       \
          }                                                                    \
                                                                               \
          return M_OK;                                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    return M_NOT_FOUND;                                                        \
  }

/**
 * @brief Peeks the top priority (min or max), the `acc` must be not NULL.
 */
#define MPQUEUE_TOP_PRI(ID, K, V)                                              \
  merr_t ID##_mpqueue_top_pri(const ID##_mpqueue_ptr_t *const self,            \
                              K *const acc) {                                  \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((self->nodes == NULL) || (self->nodes[0] == NULL)) {                   \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->nodes[0]->prio;                                               \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Peeks the top data, the `acc` must be not NULL.
 */
#define MPQUEUE_TOP(ID, K, V)                                                  \
  merr_t ID##_mpqueue_top(const ID##_mpqueue_ptr_t *const self,                \
                          V *const acc) {                                      \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((self->nodes == NULL) || (self->nodes[0] == NULL)) {                   \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->nodes[0]->data;                                               \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Inserts an new node in the priority queue, and gets sift up or down
 * based on its priority.
 */
#define MPQUEUE_PUSH(ID, K, V)                                                 \
  merr_t ID##_mpqueue_push(ID##_mpqueue_t const self, K prio, V data) {        \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->size >= self->capacity) {                                        \
      self->capacity *= MPQUEUE_DEFAULT_REALLOC_RATIO;                         \
                                                                               \
      ID##_mpqueue_node_t *try_real =                                          \
          realloc(self->nodes, sizeof *(self->nodes) * self->capacity);        \
                                                                               \
      if (try_real == NULL) {                                                  \
        self->capacity /= MPQUEUE_DEFAULT_REALLOC_RATIO;                       \
                                                                               \
        return M_REALLOC_FAILED;                                               \
      }                                                                        \
                                                                               \
      self->nodes = try_real;                                                  \
    }                                                                          \
                                                                               \
    ID##_mpqueue_node_t self_node = ID##_internal_mpqueue_node(&prio, &data);  \
                                                                               \
    if (self_node == NULL) {                                                   \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    self->nodes[self->size] = self_node;                                       \
                                                                               \
    ID##_internal_mpqueue_sift_up(self, self->size);                           \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Pops the min or max element from the priority queue.
 */
#define MPQUEUE_POP(ID, K, V)                                                  \
  merr_t ID##_mpqueue_pop(ID##_mpqueue_t const self) {                         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->size == 0) {                                                     \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mpqueue_node_t temp = self->nodes[0];                                 \
    self->nodes[0] = self->nodes[self->size - 1];                              \
    self->nodes[self->size - 1] = temp;                                        \
                                                                               \
    if (self->frd_prio != NULL) {                                              \
      self->frd_prio(&self->nodes[self->size - 1]->prio);                      \
    }                                                                          \
                                                                               \
    if (self->frd_data != NULL) {                                              \
      self->frd_data(&self->nodes[self->size - 1]->data);                      \
    }                                                                          \
                                                                               \
    free(self->nodes[self->size - 1]);                                         \
    self->nodes[self->size - 1] = NULL;                                        \
                                                                               \
    --(self->size);                                                            \
                                                                               \
    ID##_internal_mpqueue_sift_down(self, 0);                                  \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Traverses all the nodes of a priority queue and performs an action on
 * it. This function is used mostly for printing or other non-mutable actions.
 */
#define MPQUEUE_TRAVERSE(ID, K, V)                                             \
  ACTION_FUNC(ID, V)                                                           \
                                                                               \
  merr_t ID##_mpqueue_traverse(const ID##_mpqueue_ptr_t *const self,           \
                               ID##_action_func action) {                      \
    if ((self == NULL) || (self->nodes == NULL)) {                             \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->size == 0) {                                                     \
      printf("[]\n");                                                          \
    } else {                                                                   \
      printf("[");                                                             \
                                                                               \
      for (size_t iter = 0; iter < self->size; ++iter) {                       \
        if (self->nodes[iter] != NULL) {                                       \
          action(&self->nodes[iter]->data);                                    \
        }                                                                      \
      }                                                                        \
                                                                               \
      printf(" ]\n");                                                          \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Adds the all API for the `mpqueue_t` structure. You will not be always
 * need to use all the API, in this case you must be sure that you call
 * `MPQUEUE` for definition of the stack, any other macro definitions are to
 * bring new functionalities. The code length may be reduced a lot if you do not
 * call `MPQUEUE_ALL`, the code duplicated when calling `MPQUEUE_ALL` for
 * different ids or different types, it is encoureged to not to use
 * `MPQUEUE_ALL` or not to declare different ids for the same type. The ID
 * protocol is used when different files want to have the same two typed
 * structures in order to avoid name collisions.
 */
#define MPQUEUE_ALL(ID, K, V)                                                  \
  MPQUEUE(ID, K, V)                                                            \
  MPQUEUE_HEAPIFY(ID, K, V)                                                    \
  MPQUEUE_EMPTY(ID, K, V)                                                      \
  MPQUEUE_SIZE(ID, K, V)                                                       \
  MPQUEUE_CHANGE_PRI(ID, K, V)                                                 \
  MPQUEUE_CHANGE(ID, K, V)                                                     \
  MPQUEUE_FIND_PRI_IDX(ID, K, V)                                               \
  MPQUEUE_FIND_IDX(ID, K, V)                                                   \
  MPQUEUE_TOP_PRI(ID, K, V)                                                    \
  MPQUEUE_TOP(ID, K, V)                                                        \
  MPQUEUE_PUSH(ID, K, V)                                                       \
  MPQUEUE_POP(ID, K, V)                                                        \
  MPQUEUE_TRAVERSE(ID, K, V)

#endif /* MACROS_GENERICS_PRIORITY_QUEUE_UTILS_H_ */
