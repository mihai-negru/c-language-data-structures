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

#define DEFAULT_HASH_CAPACITY 50
#define DEFAULT_HASH_LOAD_FACTOR 0.75
#define DEFAULT_HASH_CAPACITY_RATIO 2

/**
 * @brief Create a hash table object. Allocation may fail if there is not enough
 * memory on heap, compare or hash functions are not valid.
 * 
 * @param init_capacity initial capacity for the hash table (should be >= |All data| * 0.75)
 * @param hash pointer to a function to hash the key into a size_t type (should not apply modulo)
 * @param cmp_key pointer to a function to compare two sets of key
 * @param cmp_dt pointer to a function to compare two sets of data
 * @param frd_key pointer to a function to free memory allocated for the CONTENT of the key pointer
 * @param frd_dt pointer to a function to free memory allocated for the CONTENT of the data pointer
 * @param key_size length in bytes of the key data type
 * @param data_size length in bytes of the data data type
 * @return hash_table_t* a new allocated hash table object or `NULL` (if function fails)
 */
hash_table_t* create_hash_table(size_t init_capacity, hash_func hash, compare_func cmp_key, compare_func cmp_dt, free_func frd_key, free_func frd_dt, size_t key_size, size_t data_size) {
    /* Check if hash function and compare function are valid */
    if ((NULL == hash) || (NULL == cmp_key) || (NULL == cmp_dt)) {
        errno = EINVAL;
        perror("Compare or hash functions undefined in hash_table");
        return NULL;
    }

    /* Check if data and key sizes are valid */
    if ((0 == key_size) || (0 == data_size)) {
        errno = EINVAL;
        perror("Key or data size are zero");
        return NULL;
    }

    /* Check if initial capacity is valid if not set it as default value */
    if (10 >= init_capacity) {
        init_capacity = DEFAULT_HASH_CAPACITY;
    }

    /* Allocate a new hash table object on heap */
    hash_table_t *new_hash_table = malloc(sizeof(*new_hash_table));

    /* Check if hash table was allocated successfully */
    if (NULL != new_hash_table) {

        /* Set function pointers of the hash table */
        new_hash_table->hash = hash;
        new_hash_table->cmp_key = cmp_key;
        new_hash_table->cmp_dt = cmp_dt;
        new_hash_table->frd_key = frd_key;
        new_hash_table->frd_dt = frd_dt;

        /* Set capacity and default size of the hash table */
        new_hash_table->key_size = key_size;
        new_hash_table->data_size = data_size;
        new_hash_table->capacity = init_capacity;
        new_hash_table->size = 0;

        /* Create the black hole node */
        new_hash_table->nil = malloc(sizeof(*new_hash_table->nil));

        /* Check if black hole node was created */
        if (NULL != new_hash_table->nil) {

            /* Set functionality for black hole node */
            new_hash_table->nil->key = NULL;
            new_hash_table->nil->data = NULL;
            new_hash_table->nil->color = HASH_BLACK;
            new_hash_table->nil->count = 1;
            new_hash_table->nil->left = new_hash_table->nil->right = new_hash_table->nil;
            new_hash_table->nil->parent = new_hash_table->nil;

            /* Allocate all buckets from hash table */
            new_hash_table->buckets = malloc(sizeof(*new_hash_table->buckets) * init_capacity);

            /* Check if buckets were allocated successfully */
            if (NULL != new_hash_table->buckets) {

                /* Set every bucket to point to black hole node */
                for (size_t iter = 0; iter < init_capacity; ++iter) {
                    new_hash_table->buckets[iter] = new_hash_table->nil;
                }
            } else {

                /* Trees were not allocated wipe hash table's memory */
                free(new_hash_table->nil);
                free(new_hash_table);
                new_hash_table = NULL;

                errno = ENOMEM;
                perror("Not enough memory for buckets of hash table");
            }
        } else {

            /* Black hole node was not allocated wipe hash table's mempry */
            free(new_hash_table);
            new_hash_table = NULL;

            errno = ENOMEM;
            perror("Not enough memory for nil hash table allocation");
        }
    } else {

        /* Hash table was not allocated return `NULL` */
        errno = ENOMEM;
        perror("Not enough memory for hash table allocation");
    }

    /* Return an allocated hash table or `NULL` */
    return new_hash_table;
}

/**
 * @brief Function to delete memory of selected node of the
 * hash table from heap zone
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param free_node address of a pointer to a memory location of a hash table node to delete
 */
static void free_hash_table_node(const hash_table_t * const __restrict__ ht, hash_table_node_t ** const __restrict__ free_node) {
    /* Check if input data is valid */
    if ((NULL != ht) && (NULL != free_node) && (ht->nil != *free_node)) {

        /* Check if content of the data node was allocated dynamically */
        if ((NULL != ht->frd_dt) && (NULL != (*free_node)->data)) {
            ht->frd_dt((*free_node)->data);
        }

        /* Free data pointer of the node */
        if (NULL != (*free_node)->data) {
            free((*free_node)->data);
        }

        /* Point to default value */
        (*free_node)->data = NULL;

        /* Check if content of the data node was allocated dynamically */
        if ((NULL != ht->frd_key) && (NULL != (*free_node)->key)) {
            ht->frd_key((*free_node)->key);
        }

        /* Free key pointer of the node */
        if (NULL != (*free_node)->key) {
            free((*free_node)->key);
        }

        /* Point to default value */
        (*free_node)->key = NULL;

        /* Free hash table node from heap */
        if (ht->nil != *free_node) {
            free(*free_node);
        }

        /* Point to default value */
        *free_node = ht->nil;
    }
}

/**
 * @brief Helper function to delete all nodes from a
 * hash table bucket.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket address of a pointer to a memory location of a hash table node to delete
 */
static void free_hash_table_helper(const hash_table_t * const __restrict__ ht, hash_table_node_t ** const __restrict__ bucket) {
    /* Check if node can be freed */
    if ((NULL == bucket) || (ht->nil == *bucket)) {
        return;
    }

    /* Free left node child */
    free_hash_table_helper(ht, &(*bucket)->left);

    /* Free right node child */
    free_hash_table_helper(ht, &(*bucket)->right);

    /* Free current working node */
    free_hash_table_node(ht, bucket);
}

/**
 * @brief Function to delete all memory allocated for one hash table.
 * Function will not automatically move hash table pointer to `NULL`.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return scl_error_t enum object for handling errors 
 */
scl_error_t free_hash_table(hash_table_t * const __restrict__ ht) {
    /* Check if hash table can be freed */
    if (NULL != ht) {

        /* Check if hash table roots are allocated */
        if (NULL != ht->buckets) {

            /* Free every tree from the hash table */
            for (size_t iter = 0; iter < ht->capacity; ++iter) {
                free_hash_table_helper(ht, &ht->buckets[iter]);
            }

            /* Free memory for the black hole node */
            free(ht->nil);
            ht->nil = NULL;

            /* Free memory for roots array */
            free(ht->buckets);
            ht->buckets = NULL;
        }

        /* Free memory of the hash table */
        free(ht);

        /* All good, go sleep */
        return SCL_OK;
    }

    /* `NULL` hash table sent to delete */
    return SCL_NULL_HASH_TABLE;
}

/**
 * @brief Create a hash table node object. Allocation may fail if address of data
 * or key pointers are `NULL` or if not enough memory is left on the heap zone.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @param data pointer to a location of a value representing data of a node
 * @return hash_table_node_t* a new allocated hash table node object or `nil`
 */
static hash_table_node_t* create_hash_table_node(const hash_table_t * const __restrict__ ht, const void *key, const void *data) {
    /* Check if data and key pointer are not `NULL` */
    if ((NULL == data) || (NULL == key)) {
        return ht->nil;
    }

    /* Allocate a new hash table node */
    hash_table_node_t *new_node = malloc(sizeof(*new_node));

    /* Check if node was allocated successfully */
    if (NULL != new_node) {

        /* Set default values of one node */
        new_node->left = new_node->right = ht->nil;
        new_node->parent = ht->nil;
        new_node->count = 1;
        new_node->color = HASH_RED;

        /* Allocate memory for data value */
        new_node->data = malloc(ht->data_size);

        /* Check if data memory was allocated */
        if (NULL != new_node->data) {

            /* 
             * Copy all bytes from data pointer
             * to memory allocated on heap
             */
            memcpy(new_node->data, data, ht->data_size);
        } else {

            /* Data memory was not allocated wipe node's memory */
            free(new_node);
            new_node = ht->nil;

            errno = ENOMEM;
            perror("Not enough memory for node hash table data allocation");
        }

        /* Allocate memory for key value */
        new_node->key = malloc(ht->key_size);

        /* Check if key memory was allocated */
        if (NULL != new_node->key) {

            /* 
             * Copy all bytes from key pointer
             * to memory allocated on heap
             */
            memcpy(new_node->key, key, ht->key_size);
        } else {

            /* Key memory was not allocated wipe node's memory */
            free(new_node->data);
            new_node->data = NULL;

            free(new_node);
            new_node = ht->nil;
        }
    } else {

        /* Node memory was not allocated, set default value */
        new_node = ht->nil;

        errno = ENOMEM;
        perror("Not enough memory for node hash table allocation");
    }

    /* Return an allocated hash table node object or `nil` */
    return new_node;
}

/**
 * @brief Function to check if hash table needs to be rehashed.
 * Function will check if hash table's load factor is greater than
 * 0.75.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return uint8_t 0 if hash tables does not need to be rehashed or
 * 1 otherwise
 */
static uint8_t hash_table_need_to_rehash(const hash_table_t * const __restrict__ ht) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return 0;
    }

    /* Check if current load factor is less or greater than 0.75 */
    return ((1.0 * ht->size) / ht->capacity > DEFAULT_HASH_LOAD_FACTOR);
}

/**
 * @brief Function to rotate to left a bucket node starting 
 * from fix_node hash table node object. Function may fail
 * if hash table object is not allocated or hash table node
 * object is nil.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working bucket from hash table
 * @param fix_node pointer to a hash table node to rotate
 */
static void hash_table_rotate_left(const hash_table_t * const __restrict__ ht, size_t bucket_index, hash_table_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets) || (ht->nil == fix_node)) {
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

    /* Update new sub-root links to the rest of the tree */
    if (ht->nil != rotate_node->parent) {
        if (ht->cmp_key(rotate_node->key, rotate_node->parent->key) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->buckets[bucket_index] = rotate_node;
    }
}

/**
 * @brief Function to rotate to right a bucket node starting 
 * from fix_node hash table node object. Function may fail
 * if hash table object is not allocated or hash table node
 * object is nil.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working bucket from hash table
 * @param fix_node pointer to a hash table node to rotate
 */
static void hash_table_rotate_right(const hash_table_t * const __restrict__ ht, size_t bucket_index, hash_table_node_t * const fix_node) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets) || (ht->nil == fix_node)) {
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
        if (ht->cmp_key(rotate_node->key, rotate_node->parent->key) >= 1) {
            rotate_node->parent->right = rotate_node;
        } else {
            rotate_node->parent->left = rotate_node;
        }
    } else {
        ht->buckets[bucket_index] = rotate_node;
    }
}

/**
 * @brief Helper function to fix up the balance of a bucket represented as
 * a red black binary tree from a hash table after insertion of one node.
 * Function may fail if current working bucket and node are `nil`.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree from hash table
 * @param fix_node pointer to a hash table node to start fixing the balance
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t hash_table_insert_fix_node_up(const hash_table_t * const __restrict__ ht, size_t bucket_index, hash_table_node_t *fix_node) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if fixing node is not `nil` */
    if (ht->nil == fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set parent node pointer as default value */
    hash_table_node_t *parent_fix_node = ht->nil;

    /* Fix up the bucket from hash table */
    while ((ht->buckets[bucket_index] != fix_node) && (HASH_BLACK != fix_node->color) && (HASH_BLACK != fix_node->parent->color)) {

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
                    hash_table_rotate_right(ht, bucket_index, parent_fix_node->parent);

                    /* Repoint selected node*/
                    fix_node = parent_fix_node;
                } else {

                    /* Right-Left Rotation */

                    /* Recolouring nodes */
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;

                    /* Rotation */
                    hash_table_rotate_right(ht, bucket_index, parent_fix_node);
                    hash_table_rotate_left(ht, bucket_index, fix_node->parent);
                }
            } else {
                if (parent_fix_node->parent->left == parent_fix_node) {

                    /* Left-Right Rotation */

                    /* Recolouring nodes */
                    fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    /* Rotation */
                    hash_table_rotate_left(ht, bucket_index, parent_fix_node);
                    hash_table_rotate_right(ht, bucket_index, fix_node->parent);
                } else {

                    /* Right-Right Rotation */

                    /* Recolouring nodes */
                    parent_fix_node->color = HASH_BLACK;
                    parent_fix_node->parent->color = HASH_RED;
                    
                    /* Rotation */
                    hash_table_rotate_left(ht, bucket_index, parent_fix_node->parent);

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
    ht->buckets[bucket_index]->color = HASH_BLACK;

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to rehash a hash table and to double the capacity
 * of the table (double number of bucket). Function may fail
 * if hash table is not allocated or has `NULL` buckets or not enough
 * memory is left on heap to allocate new red black trees.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t hash_table_rehash(hash_table_t * const __restrict__ ht);

/**
 * @brief 
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @param data pointer to a location of a value representing data of a node
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_insert(hash_table_t * const __restrict__ ht, const void *key, const void *data) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if key pointer is not `NULL` */
    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    /* Check if data pointer is not `NULL` */
    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Compute index of the current working tree */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Set iterator pointers */
    hash_table_node_t *iterator = ht->buckets[bucket_index];
    hash_table_node_t *parent_iterator = ht->nil;

    /* Find a valid position for insertion */
    while (ht->nil != iterator) {
        parent_iterator = iterator;

        if (ht->cmp_key(iterator->key, key) >= 1) {
            iterator = iterator->left;
        } else if (ht->cmp_key(iterator->key, key) <= -1) {
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

    /* Create a new bucket node object */
    hash_table_node_t *new_node = create_hash_table_node(ht, key, data);

    /* Check if new bucket(hash table) node was created */
    if (ht->nil == new_node) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }
    
    scl_error_t err = SCL_OK;

    if (ht->nil != parent_iterator) {

        /* Update parent links */
        new_node->parent = parent_iterator;

        /* Update children links */
        if (ht->cmp_key(parent_iterator->key, new_node->key) >= 1) {
            parent_iterator->left = new_node;
        } else {
            parent_iterator->right = new_node;
        }

        /* Fix the bucket */
        err = hash_table_insert_fix_node_up(ht, bucket_index, new_node);
    } else {

        /* Created node is root node */
        ht->buckets[bucket_index] = new_node;
        new_node->color = HASH_BLACK;
    }

    /* Increase hash table size */
    ++(ht->size);

    /* Check if has table needs to be rehashed */
    if (1 == hash_table_need_to_rehash(ht)) {
        return hash_table_rehash(ht);
    }

    /* Insertion in hash table went successfully, or not */
    return err;
}

/**
 * @brief Subroutine function of hash_table rehash, to traverse all
 * nodes from a bucket as in a red black tree and to insert all nodes
 * into the new buckets.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket pointer to current working hash table(red black tree) node to re-insert
 */
static void hash_table_rehash_helper(hash_table_t * const __restrict__ ht, const hash_table_node_t * const __restrict__ bucket) {
    /* Check if current root can be inserted */
    if (ht->nil == bucket) {
        return;
    }

    /* Reinsert current node into the hash table */
    hash_table_insert(ht, bucket->key, bucket->data);

    /* Reinsert left node child */
    hash_table_rehash_helper(ht, bucket->left);
    
    /* Reinsert right node child */
    hash_table_rehash_helper(ht, bucket->right);
}

/**
 * @brief Function to rehash a hash table and to double the capacity
 * of the table (double number of red black trees). Function may fail
 * if hash table is not allocated or has NULL trees or not enough
 * memory is left on heap to allocate new red black trees.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t hash_table_rehash(hash_table_t * const __restrict__ ht) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if hash function is valid */
    if (NULL == ht->hash) {
        return SCL_NULL_HASH_FUNCTION;
    }

    /* Remember old capacity and compute new capacity */
    size_t old_capacity = ht->capacity;
    ht->capacity *= DEFAULT_HASH_CAPACITY_RATIO;

    /* Allocate new array of tree roots */
    hash_table_node_t **new_buckets = malloc(sizeof(*new_buckets) * ht->capacity);

    /* Check if roots were allocated */
    if (NULL == new_buckets) {
        ht->capacity = old_capacity;

        errno = ENOMEM;
        perror("Not enough memory for buckets of hash table");

        return SCL_REHASHING_FAILED;
    }

    /* Change old roots pointer to new roots pointer */
    hash_table_node_t **old_buckets = ht->buckets;
    ht->buckets = new_buckets;

    /* Set all buckets to black hole node */
    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        new_buckets[iter] = ht->nil;
    }

    /* Set size of the hash table as default */
    ht->size = 0;

    /* 
     * Insert one bucket at a time in the new allocated trees
     * and delete one bucket at a time from old roots pointer
     */
    for (size_t iter = 0; iter < old_capacity; ++iter) {
        if (ht->nil != old_buckets[iter]) {
            hash_table_rehash_helper(ht, old_buckets[iter]);
            free_hash_table_helper(ht, &old_buckets[iter]);
        }
    }

    /* Free pointer of the old roots pointer */
    free(old_buckets);
    old_buckets = NULL;

    /* All good */
    return SCL_OK;
}

/**
 * @brief Subroutine function to search one node having as node value
 * current key specified in input. If no key is found function will
 * return a pointer to the black hole node.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree from hash table
 * @param data pointer to a location of a value representing data of a node
 * @return hash_table_node_t* an allocated hash table node object containing
 * desired data or `nil` id such data does not exists in the current tree
 */
static hash_table_node_t* hash_table_find_node(const hash_table_t * const __restrict__ ht, const void * const __restrict__ key) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets) || (NULL == key)) {
        return ht->nil;
    }

    /* Make sure that current working tree exists */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Set iterator pointer */
    hash_table_node_t *iterator = ht->buckets[bucket_index];

    /* Search for input data (void *data) in all tree */
    while (ht->nil != iterator) {
        if (ht->cmp_key(iterator->key, key) <= -1) {
            iterator = iterator->right;
        } else if (ht->cmp_key(iterator->key, key) >= 1) {
            iterator = iterator->left;
        } else {
            return iterator;
        }
    }

    /* Data was not found */
    return ht->nil;
}

/**
 * @brief Function to find the pair {key, data} from hash table.
 * However function will return a pointer to memory location just
 * for data type not for key type.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @param data pointer to a location of a value representing data of a node
 * @return const void* pointer to memory location of the data pointer from the input
 * or `NULL` is no such data exists in the hash tree
 */
const void* hash_table_find_key_data(const hash_table_t * const __restrict__ ht, const void * const key, const void * const data) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets) || (NULL == key) || (NULL == data)) {
        return NULL;
    }

    /* Get the node data or `NULL` if node is `nil` */
    const void * const search_data = hash_table_find_node(ht, key)->data;

    if (ht->cmp_dt(search_data, data) == 0) {
        return search_data;
    }

    return NULL;
}

/**
 * @brief Function to find first occurence of the key node 
 * from hash table. However function will return a pointer to memory
 * location just for data type.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of a node
 * @return const void* pointer to memory location of the data pointer
 * or `NULL` is no such key exists in the hash tree
 */
const void* hash_table_find_data(const hash_table_t * const __restrict__ ht, const void * const __restrict__ key) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets) || (NULL == key)) {
        return NULL;
    }

    /* Key was not found it means no data */
    return hash_table_find_node(ht, key)->data;
}

/**
 * @brief Function to check if hash table contains the {key, data} pair. 
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @param data pointer to a location of a value representing data of a node
 * @return uint8_t 0 if pair {key, data} is not found in hash table or 1 otherwise
 */
uint8_t hash_table_contains_key_data(const hash_table_t * const __restrict__ ht, const void * const key, const void * const data) {
    /* Pair is not in the current hash table */
    if (NULL == hash_table_find_key_data(ht, key, data)) {
        return 0;
    }

    /* Pair is in the current hash table */
    return 1;
}

/**
 * @brief Function to check if hash table is empty or not.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return uint8_t 0 if hash table is not empty or 1 otherwise
 */
uint8_t is_hash_table_empty(const hash_table_t * const __restrict__ ht) {
    /* Hash table is empty */
    if ((NULL == ht) || (NULL == ht->buckets) || (0 == ht->capacity) || (0 == ht->size)) {
        return 1;
    }

    /* Hash table is not empty */
    return 0;
}

/**
 * @brief Function to check if hash table bucket according to key
 * type value is empty or not.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @return uint8_t 0 if hash table bucket is not empty or 1 otherwise
 */
uint8_t is_hash_table_bucket_key_empty(const hash_table_t * const __restrict__ ht, const void * const __restrict__ key) {
    /* Compute the curent bucket index */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Hast table bucket is empty */
    if ((NULL == ht) || (NULL == ht->buckets) || (0 == ht->capacity) || 
        (0 == ht->size) || (ht->nil == ht->buckets[bucket_index])) {
        return 1;
    }

    /* Hash table bucket is not empty */
    return 0;
}

/**
 * @brief Get the current hash table size.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return size_t SIZE_MAX if hash table is not allocated or
 * hash table's size.
 */
size_t get_hash_table_size(const hash_table_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->size;
}

/**
 * @brief Get the current hash table capacity.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @return size_t SIZE_MAX if hash table is not allocated or
 * hash table's capacity.
 */
size_t get_hash_table_capacity(const hash_table_t * const __restrict__ ht) {
    if (NULL == ht) {
        return SIZE_MAX;
    }

    return ht->capacity;
}

/**
 * @brief Helper function to compute the size of one bucket
 * according to key type value.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket pointer to current hash table node to start counting size
 * @return size_t size of one bucket(red black tree) from hash table
 */
static size_t hash_table_count_bucket_elements_helper(const hash_table_t * const __restrict__ ht, const hash_table_node_t * const __restrict__ bucket) {
    /* Check if node can be counted */
    if (ht->nil == bucket) {
        return 0;
    }

    /* Count current node */
    size_t total_nodes = 1;

    /* Count nodes from left subtree */
    total_nodes += hash_table_count_bucket_elements_helper(ht, bucket->left);
    
    /* Count nodes from right subtree */
    total_nodes += hash_table_count_bucket_elements_helper(ht, bucket->right);

    /* Return total count of the all tree(bucket) */
    return total_nodes;
}

/**
 * @brief Function to compute the size of one bucket according to key type
 * value. Bucket index is computed with hashing function of the current
 * hast table.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @return size_t SIZE_MAX if hash table is not valid or size of desired bucket
 */
size_t hash_table_count_bucket_elements(const hash_table_t * const __restrict__ ht, const void * const __restrict__ key) {
    /* Check if input data is valid */
    if ((NULL == ht) || (NULL == ht->buckets)) {
        return SIZE_MAX;
    }

    /* Compute the bucket index */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Compute the size of the bucket */
    return hash_table_count_bucket_elements_helper(ht, ht->buckets[bucket_index]);
}

/**
 * @brief Function to find minimum node from a bucket(red black tree) starting
 * from root(subtree), this function is a subroutine to delete data from a hash
 * table bucket.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket pointer to current hash table node to start finding minimum node
 * @return hash_table_node_t* pointer to minimum node starting from root subtree
 */
static hash_table_node_t* hash_table_root_min_node(const hash_table_t * const __restrict__ ht, hash_table_node_t * __restrict__ bucket) {
    /* Check if root node has minimum */
    if (ht->nil != bucket) {

        /* Find minimum node starting from root */
        while (ht->nil != bucket->left) {
            bucket = bucket->left;
        }
    }

    /* Return minimum node */
    return bucket;
}

/**
 * @brief Function to swap two nodes from a hash table bucket.
 * This function MUST NOT be used by users, because it will
 * break the proprety of red-black tree, it is a subroutine
 * program of delete function to change data of the current node
 * with its inorder successor.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree from hash table
 * @param dest_node hash table node object to rewrite data bytes from src_node
 * @param src_node hash table node object to copy data bytes
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t hash_table_swap_nodes(const hash_table_t * const __restrict__ ht, size_t bucket_index, hash_table_node_t * __restrict__ const dest_node, hash_table_node_t * __restrict__ const src_node) {
    /* Check if swap is posible */
    if ((ht->nil == dest_node) || (ht->nil == src_node)) {
        return SCL_CANNOT_SWAP_DATA;
    }

    /* Change color of nodes */
    hash_table_node_color_t temp_color = dest_node->color;
    dest_node->color = src_node->color;
    src_node->color = temp_color;

    /* Interchange the right child */

    hash_table_node_t *temp = dest_node->right;

    dest_node->right = src_node->right;

    if (ht->nil != dest_node->right) {
        dest_node->right->parent = dest_node;
    }

    src_node->right = temp;

    if (ht->nil != src_node->right) {
        src_node->right->parent = src_node;
    }

    /* Interchange the left child */

    temp = dest_node->left;

    dest_node->left = src_node->left;

    if (ht->nil != dest_node->left) {
        dest_node->left->parent = dest_node;
    }

    src_node->left = temp;

    if (ht->nil != src_node->left) {
        src_node->left->parent = src_node;
    }

    /* Interchange parents of the two nodes */

    temp = dest_node->parent;

    dest_node->parent = src_node->parent;

    if (ht->nil != dest_node->parent) {
        if (dest_node->parent->left == src_node) {
            dest_node->parent->left = dest_node;
        } else {
            dest_node->parent->right = dest_node;
        }
    } else {
        ht->buckets[bucket_index] = dest_node;
    }

    src_node->parent = temp;

    if (ht->nil != src_node->parent) {
        if (src_node->parent->left == dest_node) {
            src_node->parent->left = src_node;
        } else {
            src_node->parent->right = src_node;
        }
    } else {
        ht->buckets[bucket_index] = src_node;
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Subroutine function to fix one bucket after one node was deleted from
 * the same bucket, the bucket will be fixed as a red black tree.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree(bucket) from hash table
 * to fix after deletion of one hash table node
 * @param fix_node pointer to a hash table node (red black node) to start fixing
 * @param parent_fix_node parent pointer to a hash table node (red black node) to start fixing
 * @return scl_error_t enum object for handling errors
 */
static scl_error_t hash_table_delete_fix_node_up(const hash_table_t * const __restrict__ ht, size_t bucket_index, hash_table_node_t *fix_node, hash_table_node_t *parent_fix_node) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if fixing is needed */
    if (ht->nil == parent_fix_node) {
        return SCL_FIXING_NULL_TREE_NODE;
    }

    /* Set the brother of the double black node */
    hash_table_node_t *brother_node = ht->nil;

    /* Fix the red-black tree(bucket) */
    while ((ht->buckets[bucket_index] != fix_node) && (HASH_BLACK == fix_node->color)) {
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
                hash_table_rotate_left(ht, bucket_index, parent_fix_node);

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
                    hash_table_rotate_right(ht, bucket_index, brother_node);
                    
                    /* Update the new brother */
                    brother_node = parent_fix_node->right;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->right->color = HASH_BLACK;

                /* Rotate parent node to left */
                hash_table_rotate_left(ht, bucket_index, parent_fix_node);

                /* Tree(bucket) is fixed */
                fix_node = ht->buckets[bucket_index];
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
                hash_table_rotate_right(ht, bucket_index, parent_fix_node);

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
                    hash_table_rotate_left(ht, bucket_index, brother_node);

                    /* Update the new brother */
                    brother_node = parent_fix_node->left;
                }

                /* Case 4: brother node is black and right child is red */

                /* Recolor the nodes */
                brother_node->color = parent_fix_node->color;
                parent_fix_node->color = HASH_BLACK;
                brother_node->left->color = HASH_BLACK;

                /* Rotate parent node to right */
                hash_table_rotate_right(ht, bucket_index, parent_fix_node);

                /* Tree(Bucket) is fixed */
                fix_node = ht->buckets[bucket_index];
            }
        }
    }

    /* Recolor the root(first node from bucket) as BLACK */
    fix_node->color = HASH_BLACK;

    /* All good, go to sleep */
    return SCL_OK;
}

/**
 * @brief Function to delete one pair {key, data} from current working
 * hash table if it exists, function will throw an error if pair does not
 * exists in the hash table.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @param data pointer to a location of a value representing data of a node
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_delete_key_data(hash_table_t * __restrict__ const ht, const void * const key, const void * const data) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table buckets are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if key type pointer is valid */
    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    /* Check if data type pointer is valid */
    if (NULL == data) {
        return SCL_INVALID_DATA;
    }

    /* Compute the bucket index to insert pair */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Check if there exists at least data to search */
    if (ht->nil == ht->buckets[bucket_index]) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    /* Find node to delete */
    hash_table_node_t *delete_node = hash_table_find_node(ht, key);

    /* Delete node is not in the current working bucket */
    if ((ht->nil == delete_node) || (0 != ht->cmp_dt(delete_node->data, data))) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Node has two children swap with it's inorder successor and delete successor */
    if ((ht->nil != delete_node->left) && (ht->nil != delete_node->right)) {

        /* Find a replacement for selected node */
        hash_table_node_t *delete_succecessor = hash_table_root_min_node(ht, delete_node->right);
                
        /* Replace the selected red-black(hash table) node and remove the dublicate */
        scl_error_t err = hash_table_swap_nodes(ht, bucket_index, delete_node, delete_succecessor);

        if (SCL_OK != err) {
            return err;
        }
    }

    /* Variable to check if fixing is needed */
    uint8_t need_fixing_tree = 1;

    /* Set the child of the deleted node */
    hash_table_node_t *delete_node_child = ht->nil;

    /* Delete node has only one child */
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
            ht->buckets[bucket_index] = delete_node_child;
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
            ht->buckets[bucket_index] = delete_node_child;
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
            ht->buckets[bucket_index] = ht->nil;
        }
    }

    hash_table_node_t *parent_delete_node = delete_node->parent;

    /* Free node memory from hash table */
    free_hash_table_node(ht, &delete_node);

    /* Deacrease tree size  */
    --(ht->size);

    /* Check if fixing is needed */
    if (0 != need_fixing_tree) {
        return hash_table_delete_fix_node_up(ht, bucket_index, delete_node_child, parent_delete_node);
    }

    /* Deletion went successfully */
    return SCL_OK;
}

/**
 * @brief Function to delete one bucket from hash table
 * correspondent to hash function of the key data type value.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of the hash
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_delete_hash(hash_table_t * __restrict__ const ht, const void * const __restrict__ key) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if key type pointer is valid */
    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    /* Compute bucket index of the key data value */
    size_t bucket_index = ht->hash(key) % ht->capacity;

    /* Check if we can delete at least one data from bucket */
    if (ht->nil == ht->buckets[bucket_index]) {
        return SCL_DELETE_FROM_EMPTY_OBJECT;
    }

    /* Compute the size of the selected bucket */
    size_t delete_size = hash_table_count_bucket_elements(ht, key);

    /* The size of the selected bucket was not computed well */
    if (SIZE_MAX == delete_size) {
        return SCL_INVALID_INPUT;
    }

    /* Update the size of the hash */
    ht->size -= delete_size;

    /* Free entire bucket from memory */
    free_hash_table_helper(ht, &ht->buckets[bucket_index]);

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to delete the first occurence of the key type value
 * from the hash table, if data was not found in the hash table an error will
 * be thrown.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param key pointer to a location of a value representing key of a node
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_delete_key(hash_table_t * __restrict__ const ht, const void * const __restrict__ key) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if has table buckets are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Check if data type value pointer is valid */
    if (NULL == key) {
        return SCL_INVALID_KEY;
    }

    /* Set delete node and bucket index ad default values */
    size_t bucket_index = ht->hash(key) % ht->capacity;
    hash_table_node_t *delete_node = hash_table_find_node(ht, key);

    /* Delete node is not in the current working bucket */
    if (ht->nil == delete_node) {
        return SCL_DATA_NOT_FOUND_FOR_DELETE;
    }

    /* Node has two children swap with it's inorder successor and delete successor */
    if ((ht->nil != delete_node->left) && (ht->nil != delete_node->right)) {

        /* Find a replacement for selected node */
        hash_table_node_t *delete_succecessor = hash_table_root_min_node(ht, delete_node->right);
                
        /* Replace the selected red-black node and remove the dublicate */
        scl_error_t err = hash_table_swap_nodes(ht, bucket_index, delete_node, delete_succecessor);

        if (SCL_OK != err) {
            return err;
        }
    }

    /* Variable to check if fixing is needed */
    uint8_t need_fixing_tree = 1;

    /* Set the child of the deleted node */
    hash_table_node_t *delete_node_child = ht->nil;

    /* Delete node has only one child */
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
            ht->buckets[bucket_index] = delete_node_child;
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
            ht->buckets[bucket_index] = delete_node_child;
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
            ht->buckets[bucket_index] = ht->nil;
        }
    }

    hash_table_node_t *parent_delete_node = delete_node->parent;

    /* Free node from hash table */
    free_hash_table_node(ht, &delete_node);

    /* Deacrease tree size  */
    --(ht->size);

    /* Check if fixing is needed */
    if (0 != need_fixing_tree) {
        return hash_table_delete_fix_node_up(ht, bucket_index, delete_node_child, parent_delete_node);
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Subroutine function for hash_table_bucket_traverse_inorder.
 * This method will recursively iterate through all nodes by
 * Left-Root-Right principle.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket starting point of the red-black tree(bucket) traversal
 * @param action a pointer function to perform an action on one hash table node object
 */
static void hash_table_bucket_traverse_inorder_helper(const hash_table_t * const __restrict__ ht, const hash_table_node_t * const __restrict__ bucket, action_func action) {
    /* Check if current working hash table node is not nil */
    if (ht->nil == bucket) {
        return;
    }

    /* Traverse in the left sub-tree */
    hash_table_bucket_traverse_inorder_helper(ht, bucket->left, action);
    
    /* Call action function */
    action(bucket->data);

    /* Traverse in the right sub-tree */
    hash_table_bucket_traverse_inorder_helper(ht, bucket->right, action);
}

/**
 * @brief Function to perform one action on one bucket, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree(bucket) from hash table
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_bucket_traverse_inorder(const hash_table_t * const __restrict__ ht, size_t bucket_index, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Make sure that bucket index does not exceed hash capacity */
    bucket_index = bucket_index % ht->capacity;

    printf("(%ld): ", bucket_index);

    if (ht->nil == ht->buckets[bucket_index]) {

        /* Tree(Bucket) is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_bucket_traverse_inorder_helper(ht, ht->buckets[bucket_index], action);
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to perform one action on entire hash table, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_traverse_inorder(const hash_table_t * const __restrict__ ht, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Traverse every bucket and perform actions */
    for (size_t iter = 0; iter < ht->capacity; ++iter) {
        
        /* Perform action on one bucket */
        scl_error_t err = hash_table_bucket_traverse_inorder(ht, iter, action);
        
        printf("\n");

        /* Something went wrong exit the function */
        if (SCL_OK != err) {
            return err;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Subroutine function for hash_table_bucket_traverse_inorder.
 * This method will recursively iterate through all nodes by
 * Root-Left-Right principle.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket starting point of the red-black tree(bucket) traversal
 * @param action a pointer function to perform an action on one hash table node object
 */
static void hash_table_bucket_traverse_preorder_helper(const hash_table_t * const __restrict__ ht, const hash_table_node_t * const __restrict__ bucket, action_func action) {
    /* Check if current working hash table node is not nil */
    if (ht->nil == bucket) {
        return;
    }

    /* Call action function */
    action(bucket->data);

    /* Traverse in the left sub-tree */
    hash_table_bucket_traverse_preorder_helper(ht, bucket->left, action);

    /* Traverse in the right sub-tree */
    hash_table_bucket_traverse_preorder_helper(ht, bucket->right, action);
}

/**
 * @brief Function to perform one action on one bucket, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree(bucket) from hash table
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_bucket_traverse_preorder(const hash_table_t * const __restrict__ ht, size_t bucket_index, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Make sure that bucket index does not exceed hash capacity */
    bucket_index = bucket_index % ht->capacity;

    printf("(%ld): ", bucket_index);

    if (ht->nil == ht->buckets[bucket_index]) {

        /* Tree(Bucket) is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_bucket_traverse_preorder_helper(ht, ht->buckets[bucket_index], action);
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to perform one action on entire hash table, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_traverse_preorder(const hash_table_t * const __restrict__ ht, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Traverse every bucket and perform actions */
    for (size_t iter = 0; iter < ht->capacity; ++iter) {

        /* Perform action on one bucket */
        scl_error_t err = hash_table_bucket_traverse_preorder(ht, iter, action);

        printf("\n");

        /* Something went wrong exit the function */
        if (SCL_OK != err) {
            return err;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Subroutine function for hash_table_bucket_traverse_inorder.
 * This method will recursively iterate through all nodes by
 * Left-Right-Root principle.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket starting point of the red-black tree(bucket) traversal
 * @param action a pointer function to perform an action on one hash table node object
 */
static void hash_table_bucket_traverse_postorder_helper(const hash_table_t * const __restrict__ ht, const hash_table_node_t * const __restrict__ bucket, action_func action) {
    /* Check if current working hash table node is not nil */
    if (ht->nil == bucket) {
        return;
    }

    /* Traverse in the left sub-tree */
    hash_table_bucket_traverse_postorder_helper(ht, bucket->left, action);

    /* Traverse in the right sub-tree */
    hash_table_bucket_traverse_postorder_helper(ht, bucket->right, action);

    /* Call action function */
    action(bucket->data);
}

/**
 * @brief Function to perform one action on one bucket, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree(bucket) from hash table
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_bucket_traverse_postorder(const hash_table_t * const __restrict__ ht, size_t bucket_index, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Make sure that bucket index does not exceed hash capacity */
    bucket_index = bucket_index % ht->capacity;

    printf("(%ld): ", bucket_index);

    if (ht->nil == ht->buckets[bucket_index]) {

        /* Tree(Bucket) is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Call helper function and traverse all nodes */
        hash_table_bucket_traverse_postorder_helper(ht, ht->buckets[bucket_index], action);
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to perform one action on entire hash table, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety).
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_traverse_postorder(const hash_table_t * const __restrict__ ht, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Traverse every bucket and perform actions */
    for (size_t iter = 0; iter < ht->capacity; ++iter) {

        /* Perform action on one bucket */
        scl_error_t err = hash_table_bucket_traverse_postorder(ht, iter, action);

        printf("\n");

        /* Something went wrong exit the function */
        if (SCL_OK != err) {
            return err;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to perform one action on one bucket, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety). Bucket will be traversed as a red black tree
 * by level.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param bucket_index index of the working red black tree(bucket) from hash table
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_bucket_traverse_level(const hash_table_t * const __restrict__ ht, size_t bucket_index, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Make sure that bucket index does not exceed hash capacity */
    bucket_index = bucket_index % ht->capacity;

    printf("(%ld): ", bucket_index);

    if (ht->nil == ht->buckets[bucket_index]) {

        /* Tree(Bucket) is empty no node to traverse */
        printf("(Null)");
    }
    else {

        /* Create a queue for bfs tree(bucket) traversal */
        queue_t * const level_queue = create_queue(NULL, sizeof(ht->buckets[bucket_index]));

        /* Check if queue was created successfully */
        if (NULL != level_queue) {

            scl_error_t err = SCL_OK;

            /* Push pointer to root node into qeuue */
            err = queue_push(level_queue, &ht->buckets[bucket_index]);

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
                    err = queue_push(level_queue, &front_node->left);

                    if (SCL_OK != err) {
                        return err;
                    }
                }
                
                /* Push on queue front right child if it exists */
                if (ht->nil != front_node->right) {
                    err = queue_push(level_queue, &front_node->right);

                    if (SCL_OK != err) {
                        return err;
                    }
                }
            }

            /* Free queue object from heap */
            return free_queue(level_queue);
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to perform one action on entire hash table, according
 * to bucket_index. Usually action will be a printing function, however
 * you can define a map function to map every node data to another
 * node data (the mapping proccess has to be injective to preserve the
 * red black tree proprety). Hash table will be traversed bucket by bucket
 * from the first bucket to last bucket, and every bucket will be traversed as
 * a red black tree by level.
 * 
 * @param ht pointer to an allocated hash table memory location
 * @param action a pointer function to perform an action on one hash table node object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t hash_table_traverse_level(const hash_table_t * const __restrict__ ht, action_func action) {
    /* Check if hash table is allocated */
    if (NULL == ht) {
        return SCL_NULL_HASH_TABLE;
    }

    /* Check if action function is valid */
    if (NULL == action) {
        return SCL_NULL_ACTION_FUNC;
    }

    /* Check if hash table roots are allocated */
    if (NULL == ht->buckets) {
        return SCL_NULL_HASH_ROOTS;
    }

    /* Traverse every bucket and perform actions */
    for (size_t iter = 0; iter < ht->capacity; ++iter) {

        /* Perform action on one bucket */
        scl_error_t err = hash_table_bucket_traverse_level(ht, iter, action);

        printf("\n");

        /* Something went wrong exit the function */
        if (SCL_OK != err) {
            return err;
        }
    }

    /* All good */
    return SCL_OK;
}