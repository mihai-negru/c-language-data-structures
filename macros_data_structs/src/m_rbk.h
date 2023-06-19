/**
 * @file m_rbk.h
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

#ifndef MACROS_GENERICS_RBK_BINARY_SEARCH_TREE_UTILS_H_
#define MACROS_GENERICS_RBK_BINARY_SEARCH_TREE_UTILS_H_

#include "./m_config.h"

/**
 * @brief Utility file containing macros that generate functions for generic
 * single linked list.
 *
 * @param ID the id of the structure in order to reduce name collisions.
 * @param T the type of the data stored inside the structure.
 */

typedef enum notype_rbk_color_s { RED, BLACK } notype_rbk_color_t;

/**
 * @brief Generates the `mrbk_t` structure depending on the name and type.
 * Also generates basic function for creation and freeing memory for the
 * structure. This structure require a method for comparing data and for freeing
 * data memory, if data is not stored as a pointer (T <=> *M), then the free
 * function must be `NULL`. If data represents a structure which contains
 * pointers allocated, then the free function must free those fields.
 */
#define MRBK(ID, T)                                                            \
  CMP_FUNC(ID, T)                                                              \
  FREE_FUNC(ID, T)                                                             \
                                                                               \
  typedef struct ID##_mrbk_node_s {                                            \
    T data;                                                                    \
    struct ID##_mrbk_node_s *parent;                                           \
    struct ID##_mrbk_node_s *left;                                             \
    struct ID##_mrbk_node_s *right;                                            \
    uint32_t count;                                                            \
    notype_rbk_color_t color;                                                  \
  } ID##_mrbk_node_ptr_t, *ID##_mrbk_node_t;                                   \
                                                                               \
  typedef struct ID##_mrbk_s {                                                 \
    ID##_mrbk_node_t root;                                                     \
    ID##_mrbk_node_t nil;                                                      \
    ID##_compare_func cmp;                                                     \
    ID##_free_func frd;                                                        \
    size_t size;                                                               \
  } ID##_mrbk_ptr_t, *ID##_mrbk_t;                                             \
                                                                               \
  ID##_mrbk_t ID##_mrbk(ID##_compare_func cmp, ID##_free_func frd) {           \
    if (NULL == cmp) {                                                         \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    ID##_mrbk_t self = malloc(sizeof *self);                                   \
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
    self->nil->color = BLACK;                                                  \
    self->nil->count = 1;                                                      \
    self->nil->left = self->nil->right = self->nil->parent = self->nil;        \
                                                                               \
    self->root = self->nil;                                                    \
    self->size = 0;                                                            \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  ID##_mrbk_node_t ID##_internal_mrbk_node(const ID##_mrbk_ptr_t *const self,  \
                                           T *const data) {                    \
    ID##_mrbk_node_t self_node = malloc(sizeof *self_node);                    \
                                                                               \
    if (self_node == NULL) {                                                   \
      return self->nil;                                                        \
    }                                                                          \
                                                                               \
    self_node->left = self_node->right = self_node->parent = self->nil;        \
    self_node->count = 1;                                                      \
    self_node->color = RED;                                                    \
    self_node->data = *data;                                                   \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  void ID##_internal_mrbk_free_help(const ID##_mrbk_ptr_t *const self,         \
                                    ID##_mrbk_node_t *self_node) {             \
    if (*self_node == self->nil) {                                             \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_internal_mrbk_free_help(self, &(*self_node)->left);                   \
    ID##_internal_mrbk_free_help(self, &(*self_node)->right);                  \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&(*self_node)->data);                                          \
    }                                                                          \
                                                                               \
    free(*self_node);                                                          \
    *self_node = self->nil;                                                    \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_free(ID##_mrbk_t *self) {                                   \
    if ((self != NULL) && (*self != NULL)) {                                   \
      ID##_internal_mrbk_free_help(*self, &(*self)->root);                     \
      free((*self)->nil);                                                      \
      free(*self);                                                             \
      *self = NULL;                                                            \
                                                                               \
      return M_OK;                                                             \
    }                                                                          \
                                                                               \
    return M_FREE_NULL;                                                        \
  }                                                                            \
                                                                               \
  void ID##_internal_mrbk_rotl(ID##_mrbk_t const self,                         \
                               ID##_mrbk_node_t const self_node) {             \
    if ((self == NULL) || (self_node == self->nil) ||                          \
        (self_node->right == self->nil)) {                                     \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t temp = self_node->right;                                  \
    self_node->right = temp->left;                                             \
                                                                               \
    if (temp->left != self->nil) {                                             \
      temp->left->parent = self_node;                                          \
    }                                                                          \
                                                                               \
    temp->left = self_node;                                                    \
    temp->parent = self_node->parent;                                          \
    self_node->parent = temp;                                                  \
                                                                               \
    if (temp->parent != self->nil) {                                           \
      if (self->cmp(&temp->data, &temp->parent->data) >= 1) {                  \
        temp->parent->right = temp;                                            \
      } else {                                                                 \
        temp->parent->left = temp;                                             \
      }                                                                        \
    } else {                                                                   \
      self->root = temp;                                                       \
    }                                                                          \
  }                                                                            \
                                                                               \
  void ID##_internal_mrbk_rotr(ID##_mrbk_t const self,                         \
                               ID##_mrbk_node_t const self_node) {             \
    if ((self == NULL) || (self_node == self->nil) ||                          \
        (self_node->left == self->nil)) {                                      \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t temp = self_node->left;                                   \
    self_node->left = temp->right;                                             \
                                                                               \
    if (temp->right != self->nil) {                                            \
      temp->right->parent = self_node;                                         \
    }                                                                          \
                                                                               \
    temp->right = self_node;                                                   \
    temp->parent = self_node->parent;                                          \
    self_node->parent = temp;                                                  \
                                                                               \
    if (temp->parent != self->nil) {                                           \
      if (self->cmp(&temp->data, &temp->parent->data) >= 1) {                  \
        temp->parent->right = temp;                                            \
      } else {                                                                 \
        temp->parent->left = temp;                                             \
      }                                                                        \
    } else {                                                                   \
      self->root = temp;                                                       \
    }                                                                          \
  }

/**
 * @brief Checks whether a binary
 * searc tree object is empty or not.
 */
#define MRBK_EMPTY(ID, T)                                                      \
  mbool_t ID##_mrbk_empty(const ID##_mrbk_ptr_t *const self) {                 \
    if ((self == NULL) || (self->root == self->nil) || (self->size == 0)) {    \
      return mtrue;                                                            \
    }                                                                          \
                                                                               \
    return mfalse;                                                             \
  }

/**
 * @brief Fetches the size of the binary search tree of unique nodes,
 * which means that the count of a single node is not taken in account.
 */
#define MRBK_SIZE(ID, T)                                                       \
  size_t ID##_mrbk_size(const ID##_mrbk_ptr_t *const self) {                   \
    if (self == NULL) {                                                        \
      return SIZE_MAX;                                                         \
    }                                                                          \
                                                                               \
    return self->size;                                                         \
  }

/**
 * @brief Gets the data under the root node of the binary search tree.
 */
#define MRBK_ROOT(ID, T)                                                       \
  merr_t ID##_mrbk_root(const ID##_mrbk_ptr_t *const self, T *const acc) {     \
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

#define MRBK_FIND_NODE(ID, T)                                                  \
  ID##_mrbk_node_t ID##_internal_mrbk_find_node(                               \
      const ID##_mrbk_ptr_t *const self, T *const data) {                      \
    if (self == NULL) {                                                        \
      return self->nil;                                                        \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t iterator = self->root;                                    \
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

/**
 * @brief Finds a data inside the binary search tree. The function works mostly
 * like a `contains` method, with extra accumulator field, in order to fetch the
 * data from the node, it is needed if you have pointers fields which can be
 * modified, thus the cmp function MUST not depend on the pointer data,
 * otherwise it may break the structure of the binary search tree.
 *
 */
#define MRBK_FIND(ID, T)                                                       \
  merr_t ID##_mrbk_find(const ID##_mrbk_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if ((self == NULL)) {                                                      \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t self_node = ID##_internal_mrbk_find_node(self, &data);    \
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

/**
 * @brief Finds the minimum data of the binary search tree, works just like
 * mrbk_find, with fetching the minimum node.
 */
#define MRBK_MIN(ID, T)                                                        \
  ID##_mrbk_node_t ID##_internal_mrbk_min_node(                                \
      const ID##_mrbk_ptr_t *const self, ID##_mrbk_node_t self_node) {         \
    if (self_node != self->nil) {                                              \
      while (self_node->left != self->nil) {                                   \
        self_node = self_node->left;                                           \
      }                                                                        \
    }                                                                          \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_min(const ID##_mrbk_ptr_t *const self, T data,              \
                       T *const acc) {                                         \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = ID##_internal_mrbk_min_node(                                        \
               self, ID##_internal_mrbk_find_node(self, &data))                \
               ->data;                                                         \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Finds the maximumdata of the binary search tree, works just like
 * mrbk_find, with fetching the maximum node.
 */
#define MRBK_MAX(ID, T)                                                        \
  ID##_mrbk_node_t ID##_internal_mrbk_max_node(                                \
      const ID##_mrbk_ptr_t *const self, ID##_mrbk_node_t self_node) {         \
    if (self_node != self->nil) {                                              \
      while (self_node->right != self->nil) {                                  \
        self_node = self_node->right;                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    return self_node;                                                          \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_max(const ID##_mrbk_ptr_t *const self, T data,              \
                       T *const acc) {                                         \
    if ((self == NULL) || (acc == NULL)) {                                     \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    *acc = ID##_internal_mrbk_max_node(                                        \
               self, ID##_internal_mrbk_find_node(self, &data))                \
               ->data;                                                         \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Finds the predecessor data of the binary search tree, works just like
 * mrbk_find, with fetching the predecessor node.
 */
#define MRBK_PRED(ID, T)                                                       \
  merr_t ID##_mrbk_pred(const ID##_mrbk_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t iterator = ID##_internal_mrbk_find_node(self, &data);     \
                                                                               \
    if (iterator == self->nil) {                                               \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      if (iterator->left != self->nil) {                                       \
        *acc = ID##_internal_mrbk_max_node(self, iterator->left)->data;        \
      } else {                                                                 \
        ID##_mrbk_node_t parent_iterator = iterator->parent;                   \
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

/**
 * @brief Finds the successor data of the binary search tree, works just like
 * mrbk_find, with fetching the successor node.
 */
#define MRBK_SUCC(ID, T)                                                       \
  merr_t ID##_mrbk_succ(const ID##_mrbk_ptr_t *const self, T data,             \
                        T *const acc) {                                        \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t iterator = ID##_internal_mrbk_find_node(self, &data);     \
                                                                               \
    if (iterator == self->nil) {                                               \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      if (iterator->right != self->nil) {                                      \
        *acc = ID##_internal_mrbk_min_node(self, iterator->left)->data;        \
      } else {                                                                 \
        ID##_mrbk_node_t parent_iterator = iterator->parent;                   \
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

/**
 * @brief Finds the lowest common ancestor data of the binary search tree, works
 * just like mrbk_find.
 */
#define MRBK_LCA(ID, T)                                                        \
  merr_t ID##_mrbk_lca(const ID##_mrbk_ptr_t *const self, T left, T right,     \
                       T *const acc) {                                         \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if ((ID##_internal_mrbk_find_node(self, &left) == self->nil) ||            \
        (ID##_internal_mrbk_find_node(self, &right) == self->nil)) {           \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if (acc != NULL) {                                                         \
      ID##_mrbk_node_t iterator = self->root;                                  \
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

/**
 * @brief Inserts a new data inside the binary search tree, it is important to
 * specify a valid cmp function in order to arange data inside the binary search
 * tree.
 */
#define MRBK_PUSH(ID, T)                                                       \
  void ID##_internal_mrbk_push_fix(ID##_mrbk_t const self,                     \
                                   ID##_mrbk_node_t self_node) {               \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t parent_self_node = self->nil;                             \
                                                                               \
    while ((self_node != self->root) && (self_node->color != BLACK) &&         \
           (self_node->parent->color != BLACK)) {                              \
      parent_self_node = self_node->parent;                                    \
      ID##_mrbk_node_t brother_node = self->nil;                               \
                                                                               \
      if (parent_self_node->parent->left == parent_self_node) {                \
        brother_node = parent_self_node->parent->right;                        \
      } else {                                                                 \
        brother_node = parent_self_node->parent->left;                         \
      }                                                                        \
                                                                               \
      if (brother_node->color == BLACK) {                                      \
        if (parent_self_node->left == self_node) {                             \
          if (parent_self_node->parent->left == parent_self_node) {            \
            parent_self_node->color = BLACK;                                   \
            parent_self_node->parent->color = RED;                             \
                                                                               \
            ID##_internal_mrbk_rotr(self, parent_self_node->parent);           \
                                                                               \
            self_node = parent_self_node;                                      \
          } else {                                                             \
            self_node->color = BLACK;                                          \
            parent_self_node->parent->color = RED;                             \
                                                                               \
            ID##_internal_mrbk_rotr(self, parent_self_node);                   \
            ID##_internal_mrbk_rotl(self, self_node->parent);                  \
          }                                                                    \
        } else {                                                               \
          if (parent_self_node->parent->left == parent_self_node) {            \
            self_node->color = BLACK;                                          \
            parent_self_node->parent->color = RED;                             \
                                                                               \
            ID##_internal_mrbk_rotl(self, parent_self_node);                   \
            ID##_internal_mrbk_rotr(self, self_node->parent);                  \
          } else {                                                             \
            parent_self_node->color = BLACK;                                   \
            parent_self_node->parent->color = RED;                             \
                                                                               \
            ID##_internal_mrbk_rotl(self, parent_self_node->parent);           \
                                                                               \
            self_node = parent_self_node;                                      \
          }                                                                    \
        }                                                                      \
      } else {                                                                 \
        parent_self_node->parent->color = RED;                                 \
        brother_node->color = BLACK;                                           \
        parent_self_node->color = BLACK;                                       \
                                                                               \
        self_node = parent_self_node->parent;                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
    self->root->color = BLACK;                                                 \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_push(ID##_mrbk_t const self, T data) {                      \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t iterator = self->root;                                    \
    ID##_mrbk_node_t parent_iterator = self->nil;                              \
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
    ID##_mrbk_node_t self_node = ID##_internal_mrbk_node(self, &data);         \
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
                                                                               \
      ID##_internal_mrbk_push_fix(self, self_node);                            \
    } else {                                                                   \
      self->root = self_node;                                                  \
      self_node->color = BLACK;                                                \
    }                                                                          \
                                                                               \
    ++(self->size);                                                            \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Removes a single data node from the binary search tree and frees its
 * content using the frd function specified at the creation of the object.
 *
 */
#define MRBK_POP(ID, T)                                                        \
  void ID##_internal_mrbk_swap(ID##_mrbk_t const self,                         \
                               ID##_mrbk_node_t const dest,                    \
                               ID##_mrbk_node_t const src) {                   \
    if ((dest == self->nil) || (src == self->nil)) {                           \
      return;                                                                  \
    }                                                                          \
                                                                               \
    notype_rbk_color_t temp_color = dest->color;                               \
    dest->color = src->color;                                                  \
    src->color = temp_color;                                                   \
                                                                               \
    ID##_mrbk_node_t temp = dest->right;                                       \
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
  void ID##_internal_mrbk_pop_fix(ID##_mrbk_t const self,                      \
                                  ID##_mrbk_node_t self_node,                  \
                                  ID##_mrbk_node_t parent_self_node) {         \
    if (parent_self_node == self->nil) {                                       \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t brother_node = self->nil;                                 \
                                                                               \
    while ((self_node != self->root) && (self_node->color != BLACK)) {         \
      if (parent_self_node->left == self_node) {                               \
        brother_node = parent_self_node->right;                                \
                                                                               \
        if (brother_node->color == RED) {                                      \
          brother_node->color = BLACK;                                         \
          parent_self_node->color = RED;                                       \
                                                                               \
          ID##_internal_mrbk_rotl(self, parent_self_node);                     \
                                                                               \
          brother_node = parent_self_node->right;                              \
        }                                                                      \
                                                                               \
        if ((brother_node->left->color == BLACK) &&                            \
            (brother_node->right->color == BLACK)) {                           \
          brother_node->color = RED;                                           \
          self_node = parent_self_node;                                        \
        } else {                                                               \
          if (brother_node->right->color == BLACK) {                           \
            brother_node->left->color = BLACK;                                 \
            brother_node->color = RED;                                         \
                                                                               \
            ID##_internal_mrbk_rotr(self, brother_node);                       \
                                                                               \
            brother_node = parent_self_node->right;                            \
          }                                                                    \
                                                                               \
          brother_node->color = parent_self_node->color;                       \
          parent_self_node->color = BLACK;                                     \
          brother_node->right->color = BLACK;                                  \
                                                                               \
          ID##_internal_mrbk_rotl(self, parent_self_node);                     \
                                                                               \
          self_node = self->root;                                              \
        }                                                                      \
      } else {                                                                 \
        brother_node = parent_self_node->left;                                 \
                                                                               \
        if (brother_node->color == RED) {                                      \
          brother_node->color = BLACK;                                         \
          parent_self_node->color = RED;                                       \
                                                                               \
          ID##_internal_mrbk_rotr(self, parent_self_node);                     \
                                                                               \
          brother_node = parent_self_node->left;                               \
        }                                                                      \
                                                                               \
        if ((brother_node->right->color == BLACK) &&                           \
            (brother_node->left->color == BLACK)) {                            \
          brother_node->color = RED;                                           \
          self_node = parent_self_node;                                        \
        } else {                                                               \
          if (brother_node->left->color == BLACK) {                            \
            brother_node->right->color = BLACK;                                \
            brother_node->color = RED;                                         \
                                                                               \
            ID##_internal_mrbk_rotl(self, brother_node);                       \
                                                                               \
            brother_node = parent_self_node->left;                             \
          }                                                                    \
                                                                               \
          brother_node->color = parent_self_node->color;                       \
          parent_self_node->color = BLACK;                                     \
          brother_node->left->color = BLACK;                                   \
                                                                               \
          ID##_internal_mrbk_rotr(self, parent_self_node);                     \
                                                                               \
          self_node = self->root;                                              \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    self_node->color = BLACK;                                                  \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_pop(ID##_mrbk_t const self, T data) {                       \
    if (self == NULL) {                                                        \
      return M_NULL_INPUT;                                                     \
    }                                                                          \
                                                                               \
    if (self->root == self->nil) {                                             \
      return M_POP_FROM_EMPTY;                                                 \
    }                                                                          \
                                                                               \
    ID##_mrbk_node_t self_node = ID##_internal_mrbk_find_node(self, &data);    \
                                                                               \
    if (self_node == self->nil) {                                              \
      return M_INVALID_INPUT;                                                  \
    }                                                                          \
                                                                               \
    if ((self_node->left != self->nil) && (self_node->right != self->nil)) {   \
      ID##_internal_mrbk_swap(                                                 \
          self, self_node,                                                     \
          ID##_internal_mrbk_min_node(self, self_node->right));                \
    }                                                                          \
                                                                               \
    mbool_t need_fixing = mtrue;                                               \
    ID##_mrbk_node_t self_node_child = self->nil;                              \
                                                                               \
    if (self_node->left != self->nil) {                                        \
      self_node_child = self_node->left;                                       \
                                                                               \
      if ((self_node_child->color == RED) && (self_node->color == BLACK)) {    \
        need_fixing = mfalse;                                                  \
        self_node_child->color = BLACK;                                        \
      }                                                                        \
                                                                               \
      self_node_child->parent = self_node->parent;                             \
                                                                               \
      if (self_node->parent != self->nil) {                                    \
        if (self_node->parent->right == self_node) {                           \
          self_node->parent->right = self_node_child;                          \
        } else {                                                               \
          self_node->parent->left = self_node_child;                           \
        }                                                                      \
      } else {                                                                 \
        self->root = self_node_child;                                          \
      }                                                                        \
    } else if (self_node->right != self->nil) {                                \
      self_node_child = self_node->right;                                      \
                                                                               \
      if ((self_node_child->color == RED) && (self_node->color == BLACK)) {    \
        need_fixing = mfalse;                                                  \
        self_node_child->color = BLACK;                                        \
      }                                                                        \
                                                                               \
      self_node_child->parent = self_node->parent;                             \
                                                                               \
      if (self_node->parent != self->nil) {                                    \
        if (self_node->parent->right == self_node) {                           \
          self_node->parent->right = self_node_child;                          \
        } else {                                                               \
          self_node->parent->left = self_node_child;                           \
        }                                                                      \
      } else {                                                                 \
        self->root = self_node_child;                                          \
      }                                                                        \
    } else {                                                                   \
      if (self_node->color == RED) {                                           \
        need_fixing = mfalse;                                                  \
      }                                                                        \
                                                                               \
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
    ID##_mrbk_node_t parent_self_node = self_node->parent;                     \
                                                                               \
    if (self->frd != NULL) {                                                   \
      self->frd(&self_node->data);                                             \
    }                                                                          \
                                                                               \
    free(self_node);                                                           \
                                                                               \
    --(self->size);                                                            \
                                                                               \
    if ((need_fixing == mtrue) && (parent_self_node != self->nil)) {           \
      ID##_internal_mrbk_pop_fix(self, self_node_child, parent_self_node);     \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Traversing the tree inorder method.
 */
#define MRBK_TRAVERSE_INORDER(ID, T)                                           \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mrbk_traverse_inorder_help(                               \
      const ID##_mrbk_ptr_t *const self,                                       \
      const ID##_mrbk_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ID##_internal_mrbk_traverse_inorder_help(self, self_node->left, action);   \
    action(&self_node->data);                                                  \
    ID##_internal_mrbk_traverse_inorder_help(self, self_node->right, action);  \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_traverse_inorder(const ID##_mrbk_ptr_t *const self,         \
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
      ID##_internal_mrbk_traverse_inorder_help(self, self->root, action);      \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Traversing the tree preorder method.
 */
#define MRBK_TRAVERSE_PREORDER(ID, T)                                          \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mrbk_traverse_preorder_help(                              \
      const ID##_mrbk_ptr_t *const self,                                       \
      const ID##_mrbk_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    action(&self_node->data);                                                  \
    ID##_internal_mrbk_traverse_preorder_help(self, self_node->left, action);  \
    ID##_internal_mrbk_traverse_preorder_help(self, self_node->right, action); \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_traverse_preorder(const ID##_mrbk_ptr_t *const self,        \
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
      ID##_internal_mrbk_traverse_preorder_help(self, self->root, action);     \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Traversing the tree postorder method.
 */
#define MRBK_TRAVERSE_POSTORDER(ID, T)                                         \
  ACTION_FUNC(ID, T)                                                           \
                                                                               \
  void ID##_internal_mrbk_traverse_postorder_help(                             \
      const ID##_mrbk_ptr_t *const self,                                       \
      const ID##_mrbk_node_ptr_t *const self_node, ID##_action_func action) {  \
    if (self_node == self->nil) {                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    action(&self_node->data);                                                  \
    ID##_internal_mrbk_traverse_postorder_help(self, self_node->left, action); \
    ID##_internal_mrbk_traverse_postorder_help(self, self_node->right,         \
                                               action);                        \
  }                                                                            \
                                                                               \
  merr_t ID##_mrbk_traverse_postorder(const ID##_mrbk_ptr_t *const self,       \
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
      ID##_internal_mrbk_traverse_postorder_help(self, self->root, action);    \
      printf(" ]");                                                            \
    }                                                                          \
                                                                               \
    return M_OK;                                                               \
  }

/**
 * @brief Adds the all API for the `mrbk_t` structure (binary search tree). You
 * will not be always need to use all the API, in this case you must be sure
 * that you call `MRBK` for definition, any other macro definitions are to bring
 * new functionalities. The code length may be reduced a lot if you do not call
 * `MRBK_ALL`, the code duplicated when calling `MRBK_ALL` for different ids or
 * different types, it is encoureged to not to use `MRBK_ALL` or not to declare
 * different ids for the same type. The ID protocol is used when different files
 * want to have the same two typed structures in order to avoid name collisions.
 */
#define MRBK_ALL(ID, T)                                                        \
  MRBK(ID, T)                                                                  \
  MRBK_EMPTY(ID, T)                                                            \
  MRBK_SIZE(ID, T)                                                             \
  MRBK_ROOT(ID, T)                                                             \
  MRBK_FIND_NODE(ID, T)                                                        \
  MRBK_FIND(ID, T)                                                             \
  MRBK_MIN(ID, T)                                                              \
  MRBK_MAX(ID, T)                                                              \
  MRBK_PRED(ID, T)                                                             \
  MRBK_SUCC(ID, T)                                                             \
  MRBK_LCA(ID, T)                                                              \
  MRBK_PUSH(ID, T)                                                             \
  MRBK_POP(ID, T)                                                              \
  MRBK_TRAVERSE_INORDER(ID, T)                                                 \
  MRBK_TRAVERSE_PREORDER(ID, T)                                                \
  MRBK_TRAVERSE_POSTORDER(ID, T)

#endif /* MACROS_GENERIC_RBK_BINARY_SEARCH_TREE_UTILS_H_ */