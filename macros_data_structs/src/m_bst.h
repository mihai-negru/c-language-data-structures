/**
 * @file m_bst.h
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

#ifndef MACROS_GENERICS_BINARY_SEARCH_TREE_UTILS_H_
#define MACROS_GENERICS_BINARY_SEARCH_TREE_UTILS_H_

#include "./m_config.h"

#define MBST(ID, T)                                                            \
  CMP_FUNC(ID, T)                                                              \
  FREE_FUNC(ID, T)                                                             \
                                                                               \
  typedef struct ID##_mbst_node_s {                                            \
    T data;                                                                    \
    struct ID##_mbst_node_s *parent;                                           \
    struct ID##_mbst_node_s *left;                                             \
    struct ID##_mbst_node_s *right;                                            \
    uint32_t count;                                                            \
  } ID##_mbst_node_ptr_t, *ID##_mbst_node_t;                                   \
                                                                               \
  typedef struct ID##_mbst_s {                                                 \
    ID##_mbst_node_t root;                                                     \
    ID##_mbst_node_t nil;                                                      \
    ID##_compare_func cmp;                                                     \
    ID##_free_func frd;                                                        \
    size_t size;                                                               \
  } ID##_mbst_ptr_t, *ID##_mbst_t;                                             \
                                                                               \
  ID##_mbst_t ID##_mbst(ID##_compare_func cmp, ID##_free_func frd) {           \
    if (NULL == cmp) {                                                         \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mbst_t self = malloc(sizeof *self);                                   \
                                                                               \
    if (self == NULL) {                                                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->cmp = cmp;                                                           \
    self->frd = frd;                                                           \
                                                                               \
    self->nil = malloc(sizeof *self->nil);                                     \
                                                                               \
    if (self->nil == NULL) {                                                   \
      free(self);                                                              \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    self->nil->data = (T)0;                                                    \
    self->nil->count = 1;                                                      \
    self->nil->left = self->nil->right = self->nil->parent = self->nil;        \
                                                                               \
    self->root = self->nil;                                                    \
    self->size = 0;                                                            \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  ID##_mbst_node_t ID##_mbst_node(const ID##_mbst_ptr_t *const self,           \
                                  T *const data) {                             \
    ID##_mbst_node_t self_node = malloc(sizeof *self_node);                    \
                                                                               \
    if (self_node == NULL) {                                                   \
      return self->nil;                                                        \
    }                                                                          \
                                                                               \
    self_node->left = self_node->right = self_node->parent = self->nil;        \
    self_node->count = 1;                                                      \
    self_node->data = *data;                                                   \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  void ID##_mbst_free_help(const ID##_mbst_ptr_t *const self,                  \
                           ID##_mbst_node_t *self_node) {                      \
    if (*self_node == self->nil) {                                             \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mbst_free_help(self, &(*self_node)->left);                            \
    ID##_mbst_free_help(self, &(*self_node)->right);                           \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&(*self_node)->data);                                          \
    }                                                                          \
                                                                               \
    free(*self_node);                                                          \
    *self_node = self->nil;                                                    \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_free(ID##_mbst_t *self) {                                   \
    if ((self != NULL) && (*self != NULL)) {                                   \
      ID##_mbst_free_help(*self, &(*self)->root);                              \
      free((*self)->nil);                                                      \
      free(*self);                                                             \
      *self = NULL;                                                            \
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }

#define MBST_EMPTY(ID, T)                                                      \
  mbool_t ID##_mbst_empty(const ID##_mbst_ptr_t *const self) {                 \
    if ((self == NULL) || (self->root == self->nil) || (self->size == 0)) {    \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

#define MBST_SIZE(ID, T)                                                       \
  size_t ID##_mbst_size(const ID##_mbst_ptr_t *const self) {                   \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

#define MBST_ROOT(ID, T)                                                       \
  merr_t ID##_mbst_root(const ID##_mbst_ptr_t *const self, T *const acc) {     \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      return M_EMPTY_STRUCTURE;                                                \
    }                                                                          \
                                                                               \
    *acc = self->root->data;                                                   \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_FIND_NODE(ID, T)                                                  \
  ID##_mbst_node_t ID##_mbst_find_node(const ID##_mbst_ptr_t *const self,      \
                                       T *const data) {                        \
    if ((self == NULL) || (self->root == self->nil)) {                         \
      return self->nil;                                                        \
    }                                                                          \
                                                                               \
    ID_mbst_node_t iterator = self->root;                                      \
                                                                               \
    while (iterator != self->nil) {                                            \
      if (self->cmp(&iterator->data, data) <= -1) {                            \
        iterator = iterator->right;                                            \
      } else if (self->cmp(&iterator->data, data) >= 1) {                      \
        iterator = iterator->left;                                             \
      } else {                                                                 \
        return iterator;                                                       \
      }                                                                        \
    }                                                                          \
                                                                               \
    return self->nil;                                                          \
  }

#define MBST_FIND(ID, T)                                                       \
  merr_t ID##_mbst_find(const ID##_mbst_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if ((self == NULL)) {                                                      \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t self_node = ID##_mbst_find_node(self, &data);             \
                                                                               \
    if (self_node == self->nil) {                                              \
      return M_NOT_FOUND;                                                      \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      *acc = self_node->data;                                                  \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_MIN(ID, T)                                                        \
  ID##_mbst_node_t ID##_mbst_min_node(const ID##_mbst_ptr_t *const self,       \
                                      ID##_mbst_node_t self_node) {            \
    if (self_node != self->nil) {                                              \
      while (self_node->left != self->nil) {                                   \
        self_node = self_node->left;                                           \
      }                                                                        \
    }                                                                          \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_min(const ID##_mbst_ptr_t *const self, T data,              \
                       T *const acc) {                                         \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = ID##_mbst_min_node(self, ID##_mbst_find_node(self, &data))->data;   \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_MAX(ID, T)                                                        \
  ID##_mbst_node_t ID##_mbst_max_node(const ID##_mbst_ptr_t *const self,       \
                                      ID##_mbst_node_t self_node) {            \
    if (self_node != self->nil) {                                              \
      while (self_node->right != self->nil) {                                  \
        self_node = self_node->right;                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_max(const ID##_mbst_ptr_t *const self, T data,              \
                       T *const acc) {                                         \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = ID##_mbst_max_node(self, ID##_mbst_find_node(self, &data))->data;   \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_ALL(ID, T)                                                        \
  MBST(ID, T)                                                                  \
  MBST_EMPTY(ID, T)                                                            \
  MBST_SIZE(ID, T)                                                             \
  MBST_ROOT(ID, T)                                                             \
  MBST_FIND_NODE(ID, T)                                                        \
  MBST_FIND(ID, T)                                                             \
  MBST_MIN(ID, T)                                                              \
  MBST_MAX(ID, T)                                                              \
  MBST_PRED(ID, T)                                                             \
  MBST_SUCC(ID, T)                                                             \
  MBST_LCA(ID, T)                                                              \
  MBST_PUSH(ID, T)                                                             \
  MBST_POP(ID, T)                                                              \
  MBST_TRAVERSE_INORDER(ID, T)                                                 \
  MBST_TRAVERSE_PREORDER(ID, T)                                                \
  MBST_TRAVERSE_POSTORDER(ID, T)                                               \
  MBST_TRAVERSE_LEVEL(ID, T)

#endif /* MACROS_GENERIC_BINARY_SEARCH_TREE_UTILS_H_ */