/**
 * @file scl_hash_table.h
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2022-06-21
 * 
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of C-language-Data-Structures.
 *
 * C-language-Data-Structures is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C-language-Data-Structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with C-language-Data-Structures.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef HASH_TABLE_UTILS_H_
#define HASH_TABLE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

/**
 * @brief Color of one hash table node
 * 
 */
typedef enum {
    HASH_RED,                                                   /* Red color of the node */
    HASH_BLACK                                                  /* Black color of the node */
} hash_table_node_color;

/**
 * @brief Hash Table Node object definition
 * 
 */
typedef struct hash_table_node_s {
    void *key;                                                  /* Pointer to a location of a value representing key of the hash */
    void *data;                                                 /* Pointer to a location of a value representing data of a node */
    struct hash_table_node_s *parent;                           /* Pointer to the parent of the current node */
    struct hash_table_node_s *left;                             /* Pointer to the left child of the current node */
    struct hash_table_node_s *right;                            /* Pointer to the right child of the current node */
    uint32_t count;                                             /* Number of nodes with the same data and key value */
    hash_table_node_color color;                                /* Color of the current node */
} hash_table_node_t;

/**
 * @brief Hash Table object definition
 * 
 */
typedef struct {
    hash_table_node_t **roots;                                  /* Array of pointers of red black trees */
    hash_table_node_t *nil;                                     /* Black hole pointer once in never out */
    hash_func hash;                                             /* Pointer to a hash function */
    compare_func cmp;                                           /* Pointer to a compare function to compare data values */
    free_func frd_key;                                          /* Pointer to a function to delete content of the key */
    free_func frd_dt;                                           /* Pointer to a function to delete content of the data */
    size_t capacity;                                            /* Number of red black trees within the hash table */
    size_t size;                                                /* Number of total nodes from  hash table object*/
} hash_table_t;

hash_table_t*           create_hash_table                       (size_t init_capacity, hash_func hash, compare_func cmp, free_func frd_ket, free_func frd_dt);
scl_error_t             free_hash_table                         (hash_table_t * const ht);

scl_error_t             hash_table_insert                       (hash_table_t * const ht, const void * const key, const void * const data, size_t key_size, size_t data_size);
const void*             hash_table_find_key_data                (const hash_table_t * const ht, const void * const key, const void * const data);
const void*             hash_table_find_key                     (const hash_table_t * const ht, const void * const data);
const void*             hash_table_find_data                    (const hash_table_t * const ht, const void * const data);
uint8_t                 hash_table_contains_key_data            (const hash_table_t * const ht, const void * const key, const void * const data);
uint8_t                 hash_table_contains_data                (const hash_table_t * const ht, const void * const data);

uint8_t                 is_hash_table_empty                     (const hash_table_t * const ht);
uint8_t                 is_hash_table_bucket_key_empty          (const hash_table_t * const ht, const void * const key);
size_t                  get_hash_table_size                     (const hash_table_t * const ht);
size_t                  get_hash_table_capacity                 (const hash_table_t * const ht);
size_t                  hash_table_count_bucket_elements        (const hash_table_t * const ht, const void * const key);

scl_error_t             hash_table_delete_key_data              (hash_table_t * const ht, const void * const key, const void * const data, size_t data_size);
scl_error_t             hash_table_delete_hash                  (hash_table_t * const ht, const void * const key);
scl_error_t             hash_table_delete_data                  (hash_table_t * const ht, const void * const data, size_t data_size);

scl_error_t             hash_table_bucket_traverse_inorder      (const hash_table_t * const ht, size_t bucket_index, action_func action);
scl_error_t             hash_table_traverse_inorder             (const hash_table_t * const ht, action_func action);
scl_error_t             hash_table_bucket_traverse_preorder     (const hash_table_t * const ht, size_t bucket_index, action_func action);
scl_error_t             hash_table_traverse_preorder            (const hash_table_t * const ht, action_func action);
scl_error_t             hash_table_bucket_traverse_postorder    (const hash_table_t * const ht, size_t bucket_index, action_func action);
scl_error_t             hash_table_traverse_postorder           (const hash_table_t * const ht, action_func action);
scl_error_t             hash_table_bucket_traverse_level        (const hash_table_t * const ht, size_t bucket_index, action_func action);
scl_error_t             hash_table_traverse_level               (const hash_table_t * const ht, action_func action);

#endif /* HASH_TABLE_UTILS_H_ */