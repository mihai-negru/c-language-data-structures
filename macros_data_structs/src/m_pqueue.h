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

#define DEFAULT_CAPACITY 10
#define DEFAULT_REALLOC_RATIO 2

#define MPQUEUE(ID, K, V)                                                      \
  CMP_FUNC(ID##K, K)                                                           \
  FREE_FUNC(ID##K, K)                                                          \
                                                                               \
  CMP_FUNC(ID##V, V)                                                           \
  FREE_FUNC(ID##V, V)                                                          \
                                                                               \
  typedef struct ID##_pqueue_node_s {                                          \
    K pri;                                                                     \
    V data;                                                                    \
  } ID##_pqueue_node_ptr_t, *ID##_pqueue_node_t;                               \
                                                                               \
  typedef struct ID##_pqueue_s {                                               \
    ID##_pqueue_node_t *nodes;                                                 \
    ID##K##_compare_func cmp_pri;                                              \
    ID##K##_free_func frd_pri;                                                 \
    ID##V##_compare_func cmp_data;                                             \
    ID##V##_free_func frd_data;                                                \
    size_t capacity;                                                           \
    size_t size;                                                               \
  } ID##_pqueue_ptr_t, *ID##_pqueue_t;

#define MPQUEUE_ALL(ID, K, V)                                                  \
  MPQUEUE(ID, K, V)                                                            \
  // MPQUEUE_HEAPIFY(ID, K, V)                                                    \
  // MPQUEUE_EMPTY(ID, K, V)                                                      \
  // MPQUEUE_SIZE(ID, K, V)                                                       \
  // MPQUEUE_CHANGE_PRI(ID, K, V)                                                 \
  // MPQUEUE_CHANGE(ID, K, V)                                                     \
  // MPQUEUE_FIND_IDX(ID, K, V)                                                   \
  // MPQUEUE_FIND_PRI_IDX(ID, K, V)                                               \
  // MPQUEUE_TOP(ID, K, V)                                                        \
  // MPQUEUE_TOP_PRI(ID, K, V)                                                    \
  // MPQUEUE_PUSH(ID, K, V)                                                       \
  // MPQUEUE_POP(ID, K, V)                                                        \
  // MPQUEUE_TRAVERSE(ID, K, V)                                                   \
  // MPQUEUE_HEAP_SORT(ID, K, V)

#endif /* MACROS_GENERICS_PRIORITY_QUEUE_UTILS_H_ */