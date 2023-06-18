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
  ID##_mbst_node_t ID##_internal_mbst_node(const ID##_mbst_ptr_t *const self,  \
                                           T *const data) {                    \
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
  void ID##_internal_mbst_free_help(const ID##_mbst_ptr_t *const self,         \
                                    ID##_mbst_node_t *self_node) {             \
    if (*self_node == self->nil) {                                             \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_internal_mbst_free_help(self, &(*self_node)->left);                   \
    ID##_internal_mbst_free_help(self, &(*self_node)->right);                  \
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
      ID##_internal_mbst_free_help(*self, &(*self)->root);                     \
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
  ID##_mbst_node_t ID##_internal_mbst_find_node(                               \
      const ID##_mbst_ptr_t *const self, T *const data) {                      \
    if (self == NULL) {                                                        \
      return self->nil;                                                        \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t iterator = self->root;                                    \
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
    ID##_mbst_node_t self_node = ID##_internal_mbst_find_node(self, &data);    \
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
  ID##_mbst_node_t ID##_internal_mbst_min_node(                                \
      const ID##_mbst_ptr_t *const self, ID##_mbst_node_t self_node) {         \
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
    *acc = ID##_internal_mbst_min_node(                                        \
               self, ID##_internal_mbst_find_node(self, &data))                \
               ->data;                                                         \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_MAX(ID, T)                                                        \
  ID##_mbst_node_t ID##_internal_mbst_max_node(                                \
      const ID##_mbst_ptr_t *const self, ID##_mbst_node_t self_node) {         \
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
    *acc = ID##_internal_mbst_max_node(                                        \
               self, ID##_internal_mbst_find_node(self, &data))                \
               ->data;                                                         \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_PRED(ID, T)                                                       \
  merr_t ID##_mbst_pred(const ID##_mbst_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t iterator = ID##_internal_mbst_find_node(self, &data);     \
                                                                               \
    if (iterator == self->nil) {                                               \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      if (iterator->left != self->nil) {                                       \
        *acc = ID##_internal_mbst_max_node(self, iterator->left)->data;        \
      } else {                                                                 \
        ID##_mbst_node_t parent_iterator = iterator->parent;                   \
                                                                               \
        while ((parent_iterator != self->nil) &&                               \
               (parent_iterator->left == iterator)) {                          \
          iterator = parent_iterator;                                          \
          parent_iterator = parent_iterator->parent;                           \
        }                                                                      \
                                                                               \
        *acc = parent_iterator->data;                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_SUCC(ID, T)                                                       \
  merr_t ID##_mbst_succ(const ID##_mbst_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t iterator = ID##_internal_mbst_find_node(self, &data);     \
                                                                               \
    if (iterator == self->nil) {                                               \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      if (iterator->right != self->nil) {                                      \
        *acc = ID##_internal_mbst_min_node(self, iterator->left)->data;        \
      } else {                                                                 \
        ID##_mbst_node_t parent_iterator = iterator->parent;                   \
                                                                               \
        while ((parent_iterator != self->nil) &&                               \
               (parent_iterator->right == iterator)) {                         \
          iterator = parent_iterator;                                          \
          parent_iterator = parent_iterator->parent;                           \
        }                                                                      \
                                                                               \
        *acc = parent_iterator->data;                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_LCA(ID, T)                                                        \
  merr_t ID##_mbst_lca(const ID##_mbst_ptr_t *const self, T left, T right,     \
                       T *const acc) {                                         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((ID##_internal_mbst_find_node(self, &left) == self->nil) ||            \
        (ID##_internal_mbst_find_node(self, &right) == self->nil)) {           \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      ID##_mbst_node_t iterator = self->root;                                  \
                                                                               \
      while (iterator != self->nil) {                                          \
        if ((self->cmp(&iterator->data, &left) >= 1) &&                        \
            (self->cmp(&iterator->data, &right) >= 1)) {                       \
          iterator = iterator->left;                                           \
        } else if ((self->cmp(&iterator->data, &left) <= -1) &&                \
                   (self->cmp(&iterator->data, &right) <= -1)) {               \
          iterator = iterator->right;                                          \
        } else {                                                               \
          *acc = iterator->data;                                               \
                                                                               \
          return M_OK;                                                         \
        }                                                                      \
      }                                                                        \
                                                                               \
      return M_UNDEFINED_BEHAVIOUR;                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_PUSH(ID, T)                                                       \
  merr_t ID##_mbst_push(ID##_mbst_t const self, T data) {                      \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t iterator = self->root;                                    \
    ID##_mbst_node_t parent_iterator = self->nil;                              \
                                                                               \
    while (iterator != self->nil) {                                            \
      parent_iterator = iterator;                                              \
                                                                               \
      if (self->cmp(&iterator->data, &data) >= 1) {                            \
        iterator = iterator->left;                                             \
      } else if (self->cmp(&iterator->data, &data) <= -1) {                    \
        iterator = iterator->right;                                            \
      } else {                                                                 \
        ++(iterator->count);                                                   \
        return M_OK;                                                           \
      }                                                                        \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t self_node = ID##_internal_mbst_node(self, &data);         \
                                                                               \
    if (self_node == self->nil) {                                              \
      return M_MALLOC_FAILED;                                                  \
    }                                                                          \
                                                                               \
    if (parent_iterator != self->nil) {                                        \
      self_node->parent = parent_iterator;                                     \
                                                                               \
      if (self->cmp(&parent_iterator->data, &self_node->data) >= 1) {          \
        parent_iterator->left = self_node;                                     \
      } else {                                                                 \
        parent_iterator->right = self_node;                                    \
      }                                                                        \
    } else {                                                                   \
      self->root = self_node;                                                  \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_POP(ID, T)                                                        \
  void ID##_internal_mbst_swap(ID##_mbst_t const self,                         \
                               ID##_mbst_node_t const dest,                    \
                               ID##_mbst_node_t const src) {                   \
    if ((dest == self->nil) || (src == self->nil)) {                           \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t temp = dest->right;                                       \
    dest->right = src->right;                                                  \
                                                                               \
    if (dest->right != self->nil) {                                            \
      dest->right->parent = dest;                                              \
    }                                                                          \
                                                                               \
    src->right = temp;                                                         \
                                                                               \
    if (src->right != self->nil) {                                             \
      src->right->parent = src;                                                \
    }                                                                          \
                                                                               \
    temp = dest->left;                                                         \
    dest->left = src->left;                                                    \
                                                                               \
    if (dest->left != self->nil) {                                             \
      dest->left->parent = dest;                                               \
    }                                                                          \
                                                                               \
    src->left = temp;                                                          \
                                                                               \
    if (src->left != self->nil) {                                              \
      src->left->parent = src;                                                 \
    }                                                                          \
                                                                               \
    temp = dest->parent;                                                       \
    dest->parent = src->parent;                                                \
                                                                               \
    if (dest->parent != self->nil) {                                           \
      if (dest->parent->left == src) {                                         \
        dest->parent->left = dest;                                             \
      } else {                                                                 \
        dest->parent->right = dest;                                            \
      }                                                                        \
    } else {                                                                   \
      self->root = dest;                                                       \
    }                                                                          \
                                                                               \
    src->parent = temp;                                                        \
                                                                               \
    if (src->parent != self->nil) {                                            \
      if (src->parent->left == dest) {                                         \
        src->parent->left = src;                                               \
      } else {                                                                 \
        src->parent->right = src;                                              \
      }                                                                        \
    } else {                                                                   \
      self->root = src;                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_pop(ID##_mbst_t const self, T data) {                       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mbst_node_t self_node = ID##_internal_mbst_find_node(self, &data);    \
                                                                               \
    if (self_node == self->nil) {                                              \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if ((self_node->left != self->nil) && (self_node->right != self->nil)) {   \
      ID##_internal_mbst_swap(                                                 \
          self, self_node,                                                     \
          ID##_internal_mbst_min_node(self, self_node->right));                \
    }                                                                          \
                                                                               \
    if (self_node->left != self->nil) {                                        \
      self_node->left->parent = self_node->parent;                             \
                                                                               \
      if (self_node->parent != self->nil) {                                    \
        if (self_node->parent->right == self_node) {                           \
          self_node->parent->right = self_node->left;                          \
        } else {                                                               \
          self_node->parent->left = self_node->left;                           \
        }                                                                      \
      } else {                                                                 \
        self->root = self_node->left;                                          \
      }                                                                        \
    } else if (self_node->right != self->nil) {                                \
      self_node->right->parent = self_node->parent;                            \
                                                                               \
      if (self_node->parent != self->nil) {                                    \
        if (self_node->parent->right == self_node) {                           \
          self_node->parent->right = self_node->right;                         \
        } else {                                                               \
          self_node->parent->left = self_node->right;                          \
        }                                                                      \
      } else {                                                                 \
        self->root = self_node->right;                                         \
      }                                                                        \
    } else {                                                                   \
      if (self_node->parent != self->nil) {                                    \
        if (self_node->parent->right == self_node) {                           \
          self_node->parent->right = self->nil;                                \
        } else {                                                               \
          self_node->parent->left = self->nil;                                 \
        }                                                                      \
      } else {                                                                 \
        self->root = self->nil;                                                \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&self_node->data);                                             \
    }                                                                          \
                                                                               \
    free(self_node);                                                           \
                                                                               \
    --(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_TRAVERSE_INORDER(ID, T)                                           \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mbst_traverse_inorder_help(                               \
      const ID##_mbst_ptr_t *const self,                                       \
      const ID##_mbst_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_internal_mbst_traverse_inorder_help(self, self_node->left, action);   \
    action(&self_node->data);                                                  \
    ID##_internal_mbst_traverse_inorder_help(self, self_node->right, action);  \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_traverse_inorder(const ID##_mbst_ptr_t *const self,         \
                                    ID##_action_func action) {                 \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      printf("(Nil)\n");                                                       \
    } else {                                                                   \
      printf("[");                                                             \
      ID##_internal_mbst_traverse_inorder_help(self, self->root, action);      \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_TRAVERSE_PREORDER(ID, T)                                          \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mbst_traverse_preorder_help(                              \
      const ID##_mbst_ptr_t *const self,                                       \
      const ID##_mbst_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    action(&self_node->data);                                                  \
    ID##_internal_mbst_traverse_preorder_help(self, self_node->left, action);  \
    ID##_internal_mbst_traverse_preorder_help(self, self_node->right, action); \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_traverse_preorder(const ID##_mbst_ptr_t *const self,        \
                                     ID##_action_func action) {                \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      printf("(Nil)\n");                                                       \
    } else {                                                                   \
      printf("[");                                                             \
      ID##_internal_mbst_traverse_preorder_help(self, self->root, action);     \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

#define MBST_TRAVERSE_POSTORDER(ID, T)                                         \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mbst_traverse_postorder_help(                             \
      const ID##_mbst_ptr_t *const self,                                       \
      const ID##_mbst_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    action(&self_node->data);                                                  \
    ID##_internal_mbst_traverse_postorder_help(self, self_node->left, action); \
    ID##_internal_mbst_traverse_postorder_help(self, self_node->right,         \
                                               action);                        \
  }                                                                            \
                                                                               \
  merr_t ID##_mbst_traverse_postorder(const ID##_mbst_ptr_t *const self,       \
                                      ID##_action_func action) {               \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (action == NULL) {                                                      \
      return M_NULL_ACTION;                                                    \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      printf("(Nil)\n");                                                       \
    } else {                                                                   \
      printf("[");                                                             \
      ID##_internal_mbst_traverse_postorder_help(self, self->root, action);    \
      printf(" ]");                                                            \
    }                                                                          \
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
  MBST_TRAVERSE_POSTORDER(ID, T)

#endif /* MACROS_GENERIC_BINARY_SEARCH_TREE_UTILS_H_ */