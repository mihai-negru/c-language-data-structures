/**
 * @file scl_hash_table.c
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

#include "./include/scl_hash_table.h"
#include "./include/scl_queue.h"

#define DEFAULT_HASH_CAPACITY 100
#define DEFAULT_HASH_LOAD_FACTOR 0.75
#define DEFAULT_HASH_CAPACITY_RATIO 2

hash_table_t* create_hash_table(size_t capacity, hash_func hash, compare_func cmp, free_func frd_key, free_func frd_dt) {
    if ((NULL == hash) || (NULL == cmp)) {
        errno = EINVAL;
        perror("Compare or hash function undefined in hash_table");
        return NULL;
    }

    if (0 == capacity) {
        capacity = DEFAULT_HASH_CAPACITY;
    }

    hash_table_t *new_hash_table = malloc(sizeof(*new_hash_table));

    if (NULL != new_hash_table) {
        new_hash_table->hash = hash;
        new_hash_table->cmp = cmp;
        new_hash_table->frd_key = frd_key;
        new_hash_table->frd_dt = frd_dt;

        new_hash_table->capacity = capacity;
        new_hash_table->size = 0;

        new_hash_table->nil = malloc(sizeof(*new_hash_table->nil));

        if (NULL != new_hash_table->nil) {
            new_hash_table->nil->data = NULL;
            new_hash_table->nil->color = HASH_BLACK;
            new_hash_table->nil->count = 1;
            new_hash_table->nil->left = new_hash_table->nil->right = new_hash_table->nil;
            new_hash_table->nil->parent = new_hash_table->nil;

            new_hash_table->roots = malloc(sizeof(*new_hash_table->roots) * capacity);

            if (NULL != new_hash_table->roots) {
                for (size_t iter = 0; iter < capacity; ++iter) {
                    new_hash_table->roots[iter] = new_hash_table->nil;
                }
            } else {
                free(new_hash_table->nil);
                free(new_hash_table);
                new_hash_table = NULL;

                errno = ENOMEM;
                perror("Not enough memory for buckets of hash table");
            }
        } else {
            free(new_hash_table);
            new_hash_table = NULL;

            errno = ENOMEM;
            perror("Not enough memory for nil hash table allocation");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory for hash table allocation");
    }

    return new_hash_table;
}

static void free_hash_table_node(const hash_table_t * const ht, hash_table_node_t ** const free_node) {
    if ((NULL != ht) && (NULL != free_node) && (ht->nil != *free_node)) {
        if ((NULL != ht->frd_dt) && (NULL != (*free_node)->data)) {
            ht->frd_dt((*free_node)->data);
        }

        if (NULL != (*free_node)->data) {
            free((*free_node)->data);
        }

        (*free_node)->data = NULL;

        if ((NULL != ht->frd_key) && (NULL != (*free_node)->key)) {
            ht->frd_key((*free_node)->key);
        }

        if (NULL != (*free_node)->key) {
            free((*free_node)->key);
        }

        (*free_node)->key = NULL;

        if (ht->nil != *free_node) {
            free(*free_node);
        }

        *free_node = ht->nil;
    }
}

static void free_hash_table_helper(const hash_table_t * const ht, hash_table_node_t ** const root) {
    if ((NULL == root) || (ht->nil == *root)) {
        return;
    }

    free_hash_table_helper(ht, &(*root)->left);
    free_hash_table_helper(ht, &(*root)->right);

    free_hash_table_node(ht, root);
}

scl_error_t free_hash_table(hash_table_t * const ht) {
    if (NULL != ht) {
        if (NULL != ht->roots) {
            for (size_t iter = 0; iter < ht->capacity; ++iter) {
                free_hash_table_helper(ht, &ht->roots[iter]);
            }

            free(ht->nil);
            ht->nil = NULL;

            free(ht->roots);
            ht->roots = NULL;
        }

        free(ht);

        return SCL_OK;
    }

    return SCL_NULL_HASH_TABLE;
}

static hash_table_node_t* create_hash_table_node(const hash_table_t * const ht, const void * const key, const void * const data, size_t key_size, size_t data_size) {
    if ((NULL == data) || (NULL == key)) {
        return ht->nil;
    }

    hash_table_node_t *new_node = malloc(sizeof(*new_node));

    if (NULL != new_node) {
        new_node->left = new_node->right = ht->nil;
        new_node->parent = ht->nil;
        new_node->count = 1;
        new_node->color = HASH_RED;

        new_node->data = malloc(data_size);

        if (NULL != new_node->data) {
            memcpy((uint8_t *)new_node->data, (const uint8_t * const)data, data_size);
        } else {
            free(new_node);
            new_node = ht->nil;

            errno = ENOMEM;
            perror("Not enough memory for node hash table data allocation");
        }

        new_node->key = malloc(key_size);

        if (NULL != new_node->key) {
            memcpy((uint8_t *)new_node->key, (const uint8_t * const)key, key_size);
        } else {
            free(new_node->data);
            new_node->data = NULL;

            free(new_node);
            new_node = ht->nil;
        }
    } else {
        new_node = ht->nil;

        errno = ENOMEM;
        perror("Not enough memory for node hash table allocation");
    }

    return new_node;
}

static uint8_t hash_table_need_to_rehash(const hash_table_t * const ht) {
    if (NULL == ht) {
        return 0;
    }

    return ((1.0 * ht->size) / ht->capacity > DEFAULT_HASH_LOAD_FACTOR);
}

static void hash_table_rotate_left(const hash_table_t * const ht, size_t root_index, hash_table_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->roots) || (ht->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (ht->nil == fix_node->right) {
        return;
    }

    /* Set new rotated sub-root */
    hash_table_node_t * const rotate_node = fix_node->right;

    /* Update child of fix_node */
    fix_node->right = rotate_node->left;

    /* Update child parent to fix_node */
    if (ht->nil != rotate_node->left) {
        rotate_node->left->parent = fix_node;
    }

    /* Rotation to left */
    rotate_node->left = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (ht->nil != rotate_node->parent) {
        if (ht->cmp(rotate_node->data, rotate_node->parent->data) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->roots[root_index] = rotate_node;
    }
}

static void hash_table_rotate_right(const hash_table_t * const ht, size_t root_index, hash_table_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->roots) || (ht->nil == fix_node)) {
        return;
    }

    /* Check if rotation may happen */
    if (ht->nil == fix_node->left) {
        return;
    }

    /* Set new rotated sub-root */
    hash_table_node_t * const rotate_node = fix_node->left;

    /* Update child of fix_node */
    fix_node->left = rotate_node->right;

    /* Update child parent to fix_node */
    if (ht->nil != rotate_node->right) {
        rotate_node->right->parent = fix_node;
    }

    /* Rotation to right */
    rotate_node->right = fix_node;

    /* Update new sub-root parent */
    rotate_node->parent = fix_node->parent;

    /* Update fix_node parent to new sub-root */
    fix_node->parent = rotate_node;

    /* Update new sub-root links to the rest of tree */
    if (ht->nil != rotate_node->parent) {
        if (ht->cmp(rotate_node->data, rotate_node->parent->data) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->roots[root_index] = rotate_node;
    }
}

static scl_error_t hash_table_insert_fix_node_up(const hash_table_t * const ht, size_t root_index, hash_table_node_t *fix_node) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (ht->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set parent node pointer as default value */
    hash_table_node_t *parent_fix_node = ht->nil;

    /* Fix up the red black tree */
    while ((ht->roots[root_index] != fix_node) && (HASH_BLACK != fix_node->color) && (HASH_BLACK != fix_node->parent->color)) {

        /* Selected node is not root so check brother color */
        
        /* Set initial data */
        parent_fix_node = fix_node->parent;
        hash_table_node_t *brother_node = ht->nil;

        /* Find brother node */
        if (parent_fix_node->parent->left == parent_fix_node) {
            brother_node = parent_fix_node->parent->right;
        } else {
            brother_node = parent_fix_node->parent->left;
        }

        /* Fix tree according to brother's color */
        if (HASH_BLACK == brother_node->color) {

            /* Brother's color is black check what rotations we should make */
            
            if (parent_fix_node->left == fix_node) {
                if (parent_fix_node->parent->left == parent_fix_node) {

                    /* Left-Left rotation case*/

                    /* Recolouring nodes */
                    parent_fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    /* Rotation */
                    hash_table_rotate_right(ht, root_index, parent_fix_node->parent);

                    /* Repoint selected node*/
                    fix_node = parent_fix_node;
                } else {

                    /* Right-Left Rotation */

                    /* Recolouring nodes */
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;

                    /* Rotation */
                    hash_table_rotate_right(ht, root_index, parent_fix_node);
                    hash_table_rotate_left(ht, root_index, fix_node->parent);
                }
            } else {
                if (parent_fix_node->parent->left == parent_fix_node) {

                    /* Left-Right Rotation */

                    /* Recolouring nodes */
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    /* Rotation */
                    hash_table_rotate_left(ht, root_index, parent_fix_node);
                    hash_table_rotate_right(ht, root_index, fix_node->parent);
                } else {

                    /* Right-Right Rotation */

                    /* Recolouring nodes */
                    parent_fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    /* Rotation */
                    hash_table_rotate_left(ht, root_index, parent_fix_node->parent);

                    /* Repoint selected node */
                    fix_node = parent_fix_node;
                }
            }
        } else if (HASH_RED == brother_node->color) {

            /* Brother's color is red so recolor the nodes */
            parent_fix_node->parent->color = HASH_RED;
            brother_node->color = HASH_BLACK;
            parent_fix_node->color = HASH_BLACK;

            /* Repoint selected node */
            fix_node = parent_fix_node->parent;
        } else {

            /* Color is not RED or BLACK something went wrong */
            return SCL_UNKNOWN_HASH_NODE_COLOR;
        }
    }

    /* Make sure root is black */
    ht->roots[root_index]->color = HASH_BLACK;

    return SCL_OK;
}

static scl_error_t hash_table_rehash(hash_table_t * const ht, size_t key_size, size_t data_size);

scl_error_t hash_table_insert(hash_table_t * const ht, const void * const key, const void * const data, size_t key_size, size_t data_size) {
    /* Check if tree and data are valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    size_t root_index = ht->hash(key) % ht->capacity;

    /* Set iterator pointers */
    hash_table_node_t *iterator = ht->roots[root_index];
    hash_table_node_t *parent_iterator = ht->nil;

    /* Find a valid position for insertion */
    while (ht->nil != iterator) {
        parent_iterator = iterator;

        if (ht->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else if (ht->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else {

            /*
             * Node already exists in current red-black tree
             * increment count value of node
             */
            ++(iterator->count);
            return 0;
        }
    }

    /* Create a new red-black node object */
    hash_table_node_t *new_node = create_hash_table_node(ht, key, data, key_size, data_size);

    /* Check if new red-black node was created */
    if (ht->nil == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
    
    scl_error_t err = SCL_OK;

    if (ht->nil != parent_iterator) {

        /* Update parent links */
        new_node->parent = parent_iterator;

        /* Update children links */
        if (ht->cmp(parent_iterator->data, new_node->data) >= 1) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        /* Fix the red black tree*/
        err = hash_table_insert_fix_node_up(ht, root_index, new_node);
    } else {

        /* Created node is root node */
        ht->roots[root_index] = new_node;
        new_node->color = HASH_BLACK;
    }

    /* Increase red-black tree size */
    ++(ht->size);

    if (1 == hash_table_need_to_rehash(ht)) {
        return hash_table_rehash(ht, key_size, data_size);
    }

    /* Insertion in red-black went successfully */
    return err;
}

static void hash_table_rehash_helper(hash_table_t * const ht, const hash_table_node_t * const root, size_t key_size, size_t data_size) {
    if (ht->nil == root) {
        return;
    }

    hash_table_insert(ht, root->key, root->data, key_size, data_size);

    hash_table_rehash_helper(ht, root->left, key_size, data_size);
    hash_table_rehash_helper(ht, root->right, key_size, data_size);
}

static scl_error_t hash_table_rehash(hash_table_t * const ht, size_t key_size, size_t data_size) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == ht->hash) {
        return SCL_NULL_HASH_FUNCTION;
    }

    size_t old_capacity = ht->capacity;
    ht->capacity *= DEFAULT_HASH_CAPACITY_RATIO;

    hash_table_node_t **new_roots = malloc(sizeof(*new_roots) * ht->capacity);

    if (NULL == new_roots) {
        errno = ENOMEM;
        perror("Not enough memory for buckets of hash table");

        return SCL_REHASHING_FAILED;
    }

    hash_table_node_t **old_roots = ht->roots;
    ht->roots = new_roots;

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        new_roots[iter] = ht->nil;
    }

    ht->size = 0;

    for (size_t iter = 0; iter < old_capacity; ++iter) {
        if (ht->nil != old_roots[iter]) {
            hash_table_rehash_helper(ht, old_roots[iter], key_size, data_size);
            free_hash_table_helper(ht, &old_roots[iter]);
        }
    }

    free(old_roots);
    old_roots = NULL;

    return SCL_OK;
}

static hash_table_node_t* hash_table_find_node(const hash_table_t * const ht, size_t root_index, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->roots) || (NULL == data)) {
        return ht->nil;
    }

    root_index = root_index % ht->capacity;

    /* Set iterator pointer */
    hash_table_node_t *iterator = ht->roots[root_index];

    /* Search for imput data (void *data) in all tree */
    while (ht->nil != iterator) {
        if (ht->cmp(iterator->data, data) <= -1) {
            iterator = iterator->right;
        } else if (ht->cmp(iterator->data, data) >= 1) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    /* Data was not found */
    return ht->nil;
}

const void* hash_table_find_key_data(const hash_table_t * const ht, const void * const key, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->roots) || (NULL == key) || (NULL == data)) {
        return NULL;
    }

    size_t root_index = ht->hash(key) % ht->capacity;

    /* Get the nide data or NULL if node is nil */
    return hash_table_find_node(ht, root_index, data)->data;
}

const void* hash_table_find_key(const hash_table_t * const ht, const void * const data) {
    if ((NULL == ht) || (NULL == ht->roots) || (NULL == data)) {
        return NULL;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        const hash_table_node_t * const search_node = hash_table_find_node(ht, iter, data);
        if (NULL != search_node->data) {
            return search_node->key;
        }
    }

    return NULL;
}

const void* hash_table_find_data(const hash_table_t * const ht, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->roots) || (NULL == data)) {
        return NULL;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        const void *search_data = hash_table_find_node(ht, iter, data)->data;

        if (NULL != search_data) {
            return search_data;
        }
    }

    return NULL;
}

uint8_t hash_table_contains_key_data(const hash_table_t * const ht, const void * const key, const void * const data) {
    if (NULL == hash_table_find_key_data(ht, key, data)) {
        return 0;
    }

    return 1;
}

uint8_t hash_table_contains_data(const hash_table_t * const ht, const void * const data) {
    if (NULL == hash_table_find_data(ht, data)) {
        return 0;
    }

    return 1;
}

uint8_t is_hash_table_empty(const hash_table_t * const ht) {
    if ((NULL == ht) || (NULL == ht->roots) || (0 == ht->capacity) || (0 == ht->size)) {
        return 1;
    }

    return 0;
}

uint8_t is_hash_table_bucket_key_empty(const hash_table_t * const ht, const void * const key) {
    size_t root_index = ht->hash(key) % ht->capacity;

    if ((NULL == ht) || (NULL == ht->roots) || (0 == ht->capacity) || 
        (0 == ht->size) || (ht->nil == ht->roots[root_index])) {
        return 1;
    }

    return 0;
}

size_t get_hash_table_size(const hash_table_t * const ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->size;
}

size_t get_hash_table_capacity(const hash_table_t * const ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->capacity;
}

static size_t hash_table_count_bucket_elements_helper(const hash_table_t * const ht, const hash_table_node_t * const root) {
    if (ht->nil == root) {
        return 0;
    }

    size_t total_nodes = 1;

    total_nodes += hash_table_count_bucket_elements_helper(ht, root->left);
    total_nodes += hash_table_count_bucket_elements_helper(ht, root->right);

    return total_nodes;
}

size_t hash_table_count_bucket_elements(const hash_table_t * const ht, const void * const key) {
    if ((NULL == ht) || (NULL == ht->roots)) {
        return SIZE_MAX;
    }

    size_t root_index = ht->hash(key) % ht->capacity;

    return hash_table_count_bucket_elements_helper(ht, ht->roots[root_index]);
}

static hash_table_node_t* hash_table_root_min_node(const hash_table_t * const ht, hash_table_node_t *root) {
    if (ht->nil != root) {
        while (ht->nil != root->left) {
            root = root->left;
        }
    }

    return root;
}

static scl_error_t hash_table_change_data(hash_table_node_t * const dest_node, const hash_table_node_t * const src_node, size_t data_size) {
    /* Check if data pointers are allocated */
    if ((NULL == dest_node->data) || (NULL == src_node->data)) {
        return SCL_CANNOT_CHANGE_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    /* Rewrite bytes into dest_node from src_node */
    memcpy((uint8_t *)dest_node->data, (const uint8_t * const)src_node->data, data_size);

    /* Update count parameter */
    dest_node->count = src_node->count;

    return SCL_OK;
}

static scl_error_t hash_table_delete_fix_node_up(const hash_table_t * const ht, size_t root_index, hash_table_node_t *fix_node, hash_table_node_t *parent_fix_node) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (ht->nil == parent_fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set the brother of the double black node */
    hash_table_node_t *brother_node = ht->nil;

    /* Fix the red-black tree */
    while ((ht->roots[root_index] != fix_node) && (HASH_BLACK == fix_node->color)) {
        if (parent_fix_node->left == fix_node) {

            /* Double black node is a left child */

            /* Find the brother node */
            brother_node = parent_fix_node->right;

            if (HASH_RED == brother_node->color) {

                /* Case 1: brother is a red node */

                /* Recolor the nodes */
                brother_node->color = HASH_BLACK;
                parent_fix_node->color = HASH_RED;

                /* Rotate the parent to the left */
                hash_table_rotate_left(ht, root_index, parent_fix_node);

                /* Update the brother node */
                brother_node = parent_fix_node->right;
            }

            if ((HASH_BLACK == brother_node->left->color) && (HASH_BLACK == brother_node->right->color)) {

                /* Case 2: brother is a black node and its children are black */

                /* Recolor the brother node */
                brother_node->color = HASH_RED;

                /* Propagate the double black problem in higher hierarchy */
                fix_node = parent_fix_node;
            } else {
                if (HASH_BLACK == brother_node->right->color) {

                    /* Case 3: brother node is black, left child is red and right is black */

                    /* Recolor nodes */
                    brother_node->left->color = HASH_BLACK;
                    brother_node->color = HASH_RED;

                    /* Rotate brother to the right */
                    hash_table_rotate_right(ht, root_index, brother_node);
                    
                    /* Update the new brother */
                    brother_node = parent_fix_node->right;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->right->color = HASH_BLACK;

                /* Rotate parent node to left */
                hash_table_rotate_left(ht, root_index, parent_fix_node);

                /* Tree is fixed */
                fix_node = ht->roots[root_index];
            }
        } else {

            /* Double black node is a right child */

            /* Find the brother node */
            brother_node = parent_fix_node->left;

            if (HASH_RED == brother_node->color) {

                /* Case 1: brother is a red node */

                /* Recolor the nodes */
                brother_node->color = HASH_BLACK;
                parent_fix_node->color = HASH_RED;

                /* Rotate the parent to the right */
                hash_table_rotate_right(ht, root_index, parent_fix_node);

                /* Update the brother node */
                brother_node = parent_fix_node->left;
            }

            if ((HASH_BLACK == brother_node->right->color) && (HASH_BLACK == brother_node->left->color)) {

                /* Case 2: brother is a black node and its children are black */

                /* Recolor the brother node */
                brother_node->color = HASH_RED;

                /* Propagate the double black problem in higher hierarchy */
                fix_node = parent_fix_node;
            } else {
                if (HASH_BLACK == brother_node->left->color) {

                    /* Case 3: brother node is black, right child is red and left is black */

                    /* Recolor nodes */
                    brother_node->right->color = HASH_BLACK;
                    brother_node->color = HASH_RED;

                    /* Rotate brother to the left */
                    hash_table_rotate_left(ht, root_index, brother_node);

                    /* Update the new brother */
                    brother_node = parent_fix_node->left;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->left->color = HASH_BLACK;

                /* Rotate parent node to right */
                hash_table_rotate_right(ht, root_index, parent_fix_node);

                /* Tree is fixed */
                fix_node = ht->roots[root_index];
            }
        }
    }

    /* Recolor the root as BLACK */
    fix_node->color = HASH_BLACK;

    return SCL_OK;
}

scl_error_t hash_table_delete_key_data(hash_table_t * const ht, const void * const key, const void * const data, size_t data_size) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    size_t root_index = ht->hash(key) % ht->capacity;

    if (ht->nil == ht->roots[root_index]) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    /* Find node to delete */
    hash_table_node_t *delete_node = hash_table_find_node(ht, root_index, data);

    /* Delete node is not in the current working tree */
    if (ht->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Node has two children swap with it's inorder successor and delete successor */
    if ((ht->nil != delete_node->left) && (ht->nil != delete_node->right)) {

        /* Find a replacement for selected node */
        hash_table_node_t *delete_succecessor = hash_table_root_min_node(ht, delete_node->right);
                
        /* Replace the selected red-black node and remove the dublicate */
        scl_error_t err = hash_table_change_data(delete_node, delete_succecessor, data_size);

        if (SCL_OK != err) {
            return err;
        }

        /* Delete node now is inorder successor */
        delete_node = delete_succecessor;
    }

    /* Variable to check if fixing is needed */
    uint8_t need_fixing_tree = 1;

    /* Set the child of the deleted node */
    hash_table_node_t *delete_node_child = ht->nil;

    /* delete node has only one child */
    if (ht->nil != delete_node->left) {
        delete_node_child = delete_node->left;

        /* Check if deletion will not make a double black exception */
        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = HASH_BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */            
            ht->roots[root_index] = delete_node_child;
        }
    } else if (ht->nil != delete_node->right) {
        delete_node_child = delete_node->right;

        /* Check if deletion will not make a double black exception */
        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = HASH_BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            ht->roots[root_index] = delete_node_child;
        }
    } else {

        /* Check if deletion will not make a double black exception */ 
        if (HASH_RED == delete_node->color) {
            need_fixing_tree = 0;
        }

        // Update parent's links to nil node
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = ht->nil;
            } else {
                delete_node->parent->left = ht->nil;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            ht->roots[root_index] = ht->nil;
        }
    }

    hash_table_node_t *parent_delete_node = delete_node->parent;

    free_hash_table_node(ht, &delete_node);

    /* Deacrease tree size  */
    --(ht->size);

    /* Check if fixing is needed */
    if (0 != need_fixing_tree) {
        return hash_table_delete_fix_node_up(ht, root_index, delete_node_child, parent_delete_node);
    }

    /* Deletion went successfully */
    return SCL_OK;
}

scl_error_t hash_table_delete_key(hash_table_t * const ht, const void * const key) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    size_t root_index = ht->hash(key) % ht->capacity;

    if (ht->nil == ht->roots[root_index]) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    size_t delete_size = hash_table_count_bucket_elements(ht, key);

    if (SIZE_MAX == delete_size) {
        return SCL_INVALID_INPUT;
    }

    ht->size -= delete_size;

    free_hash_table_helper(ht, &ht->roots[root_index]);

    return SCL_OK;
}

scl_error_t hash_table_delete_data(hash_table_t * const ht, const void * const data, size_t data_size) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    if (0 == data_size) {
        return SCL_DATA_SIZE_ZERO;
    }

    size_t root_index = 0;
    hash_table_node_t *delete_node = ht->nil;

    for (; root_index < ht->capacity; ++root_index) {
        delete_node = hash_table_find_node(ht, root_index, data);

        if (ht->nil != delete_node) {
            break;
        }
    }

    /* Delete node is not in the current working tree */
    if (ht->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Node has two children swap with it's inorder successor and delete successor */
    if ((ht->nil != delete_node->left) && (ht->nil != delete_node->right)) {

        /* Find a replacement for selected node */
        hash_table_node_t *delete_succecessor = hash_table_root_min_node(ht, delete_node->right);
                
        /* Replace the selected red-black node and remove the dublicate */
        scl_error_t err = hash_table_change_data(delete_node, delete_succecessor, data_size);

        if (SCL_OK != err) {
            return err;
        }

        /* Delete node now is inorder successor */
        delete_node = delete_succecessor;
    }

    /* Variable to check if fixing is needed */
    uint8_t need_fixing_tree = 1;

    /* Set the child of the deleted node */
    hash_table_node_t *delete_node_child = ht->nil;

    /* delete node has only one child */
    if (ht->nil != delete_node->left) {
        delete_node_child = delete_node->left;

        /* Check if deletion will not make a double black exception */
        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = HASH_BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */            
            ht->roots[root_index] = delete_node_child;
        }
    } else if (ht->nil != delete_node->right) {
        delete_node_child = delete_node->right;

        /* Check if deletion will not make a double black exception */
        if ((HASH_RED == delete_node_child->color) && (HASH_BLACK == delete_node->color)) {
            need_fixing_tree = 0;

            /* Recolor red node into a red one */
            delete_node_child->color = HASH_BLACK;
        }

        /* Update child with it's grandparent */
        delete_node_child->parent = delete_node->parent;

        /* Update grandparent with it's grandchild */
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = delete_node_child;
            } else {
                delete_node->parent->left = delete_node_child;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            ht->roots[root_index] = delete_node_child;
        }
    } else {

        /* Check if deletion will not make a double black exception */ 
        if (HASH_RED == delete_node->color) {
            need_fixing_tree = 0;
        }

        // Update parent's links to nil node
        if (ht->nil != delete_node->parent) {
            if (delete_node->parent->right == delete_node) {
                delete_node->parent->right = ht->nil;
            } else {
                delete_node->parent->left = ht->nil;
            }
        } else {

            /*
             * Selected node was root
             * Update a new root
             */
            ht->roots[root_index] = ht->nil;
        }
    }

    hash_table_node_t *parent_delete_node = delete_node->parent;

    free_hash_table_node(ht, &delete_node);

    /* Deacrease tree size  */
    --(ht->size);

    /* Check if fixing is needed */
    if (0 != need_fixing_tree) {
        return hash_table_delete_fix_node_up(ht, root_index, delete_node_child, parent_delete_node);
    }

    /* Deletion went successfully */
    return SCL_OK;
}

static void hash_table_root_traverse_inorder_helper(const hash_table_t * const ht, const hash_table_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (ht->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    hash_table_root_traverse_inorder_helper(ht, root->left, action);
    
    /* Call action function */
    action(root->data);

    /* Traverse in the right sub-tree */
    hash_table_root_traverse_inorder_helper(ht, root->right, action);
}

scl_error_t hash_table_root_traverse_inorder(const hash_table_t * const ht, size_t root_index, action_func action) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    root_index = root_index % ht->capacity;

    printf("(%ld): ", root_index);

    if (ht->nil == ht->roots[root_index]) {

        /* Tree is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_root_traverse_inorder_helper(ht, ht->roots[root_index], action);
    }

    return SCL_OK;
}

scl_error_t hash_table_traverse_inorder(const hash_table_t * const ht, action_func action) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        scl_error_t err = hash_table_root_traverse_inorder(ht, iter, action);
        
        printf("\n");

        if (SCL_OK != err) {
            return err;
        }
    }

    return SCL_OK;
}

static void hash_table_root_traverse_preorder_helper(const hash_table_t * const ht, const hash_table_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (ht->nil == root) {
        return;
    }

    /* Call action function */
    action(root->data);

    /* Traverse in the left sub-tree */
    hash_table_root_traverse_preorder_helper(ht, root->left, action);

    /* Traverse in the right sub-tree */
    hash_table_root_traverse_preorder_helper(ht, root->right, action);
}

scl_error_t hash_table_root_traverse_preorder(const hash_table_t * const ht, size_t root_index, action_func action) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    root_index = root_index % ht->capacity;

    printf("(%ld): ", root_index);

    if (ht->nil == ht->roots[root_index]) {

        /* Tree is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_root_traverse_preorder_helper(ht, ht->roots[root_index], action);
    }

    return SCL_OK;
}

scl_error_t hash_table_traverse_preorder(const hash_table_t * const ht, action_func action) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        scl_error_t err = hash_table_root_traverse_preorder(ht, iter, action);

        printf("\n");

        if (SCL_OK != err) {
            return err;
        }
    }

    return SCL_OK;
}

static void hash_table_root_traverse_postorder_helper(const hash_table_t * const ht, const hash_table_node_t * const root, action_func action) {
    /* Check if current working red-black node is not NULL */
    if (ht->nil == root) {
        return;
    }

    /* Traverse in the left sub-tree */
    hash_table_root_traverse_postorder_helper(ht, root->left, action);

    /* Traverse in the right sub-tree */
    hash_table_root_traverse_postorder_helper(ht, root->right, action);

    /* Call action function */
    action(root->data);
}

scl_error_t hash_table_root_traverse_postorder(const hash_table_t * const ht, size_t root_index, action_func action) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    root_index = root_index % ht->capacity;

    printf("(%ld): ", root_index);

    if (ht->nil == ht->roots[root_index]) {

        /* Tree is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_root_traverse_postorder_helper(ht, ht->roots[root_index], action);
    }

    return SCL_OK;
}

scl_error_t hash_table_traverse_postorder(const hash_table_t * const ht, action_func action) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        scl_error_t err = hash_table_root_traverse_postorder(ht, iter, action);

        printf("\n");

        if (SCL_OK != err) {
            return err;
        }
    }

    return SCL_OK;
}

scl_error_t hash_table_root_traverse_level(const hash_table_t * const ht, size_t root_index, action_func action) {
    /* Check if input data is valid */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    root_index = root_index % ht->capacity;

    printf("(%ld): ", root_index);

    if (ht->nil == ht->roots[root_index]) {

        /* Tree is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Create a queue for bfs tree traversal */
        queue_t * const level_queue = create_queue(NULL);

        /* Check if queue was created successfully */
        if (NULL != level_queue) {

            scl_error_t err = SCL_OK;

            /* Push pointer to root node into qeuue */
            err = queue_push(level_queue, &ht->roots[root_index], sizeof(ht->roots[root_index]));

            if (SCL_OK != err) {
                return err;
            }

            /* Traverse all nodes */
            while (!is_queue_empty(level_queue)) {

                /* Get front node from queue */
                const hash_table_node_t * const front_node = *(const hash_table_node_t ** const)queue_front(level_queue);

                /* Remove front node from queue */
                err = queue_pop(level_queue);

                if (SCL_OK != err) {
                    return err;
                }

                /* Call action function on front node */
                action(front_node->data);

                /* Push on queue front left child if it exists */
                if (ht->nil != front_node->left) {
                    err = queue_push(level_queue, &front_node->left, sizeof(front_node->left));

                    if (SCL_OK != err) {
                        return err;
                    }
                }
                
                /* Push on queue front right child if it exists */
                if (ht->nil != front_node->right) {
                    err = queue_push(level_queue, &front_node->right, sizeof(front_node->right));

                    if (SCL_OK != err) {
                        return err;
                    }
                }
            }

            /* Free queue object from heap */
            return free_queue(level_queue);
        }
    }

    return SCL_OK;
}

scl_error_t hash_table_traverse_level(const hash_table_t * const ht, action_func action) {
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    if (NULL == ht->roots) {
        return SCL_NULL_HASH_ROOTS;
    }

    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        scl_error_t err = hash_table_root_traverse_level(ht, iter, action);

        printf("\n");

        if (SCL_OK != err) {
            return err;
        }
    }

    return SCL_OK;
}