# Documentation for hash table object ([scl_hash_table.h](../src/include/scl_hash_table.h))

## How to create a hash table and how to destroy it?

Two main function that will help you to initialize a hash table and to free it from heap memory are:

1. **create_hash_table** -> function will take some input data and will allocate a hash table  strucutre on heap memory and will return a pointer to its location.

2. **free_hash_table** -> function will take as input a pointer to an allocated hash table (but also hash table pointer can be NULL), if hash table pointer is valid then all the memory allocated for selected hash table will be wipped out from RAM memory.

Exemple of basic functions for hash tables: (Suppose keys are strings and data is their length)
First of all we will need to thing about a hash function to map the key value to a **size_t** type,
next I will propose a function for hashing strings:

```C
    #include <scl_datastruc.h>

    #define MAX_STRING_SIZE 10

    size_t hash_string(const void * const key) {
        if (key == NULL) {
            return SIZE_MAX;
        }

        size_t hash = 0;

        for (size_t iter = 1; key[iter] != 0; ++iter) {
            hash = (hash + key[iter] * (iter - 1)) % SIZE_MAX;
        }

        return hash;
    }

    void free_string(void *key) {
        if (NULL != key) {
            free(key);
        }
    }

    int main() {

        size_t number_of_supposed_keys = 100;

        // The init capacity has to be >= number_of_supposed_keys * 0.75

        hash_table_t *ht = create_hash_table((size_t)(0.75 * number_of_supposed_keys) + 1,
             /* Function to hash the key type */ &hash_string,
             /* Function to compare keys */ &compare_string,
             /* Function to comapre data */ &compare_ulong_int,
             /* Function to free keys from memory */ &free_string,
             /* Function to free data from memory */ NULL,
             /* Size of the key time for strings has to be the same */ MAX_STRING_SIZE,
             /* Size of the data time */ sizeof(size_t)
        );

        // Yey we created the hash table

        if (NULL == ht) {
            /* The allocation failed */
            exit(EXIT_FAILURE);
        }

        free_hash_table(ht); // Every memory linked to hash table is removed

        return 0;
    }
```

>**NOTE:** Depending on your hash function the hash table operations may be faster or slower. It is important as the modulo operator not to use a size that is less than your possible number of data, suppose that you want to insert arround 1000 data, than your modulo operator has to be grater than 1000. For the best performance use `SIZE_MAX`, it will take care that no value overflow happens and will preserve the best and faster proprieties of the hash table.

>**NOTE:** As you observed I calculated the initial capacity for hash table as `number_of_supposed_key * 0.75`, the load factor is **0.75** which means that then the load factor is less than that value the hash table has a good performace, so then you will insert many keys the load factor will grow, and the hash table will lack performace, but when you specify this initial capacity, the performance will stay for more long time as one random calculated, plus then the **hash load factor** will be greater then **0.75** the rehash method will be called which takes a lot of time to execute.

## How to insert and how to remove elements from hash table?

There are 4 functions that will insert and delete node key-data from hash table:

1. **hash_table_insert** -> this function will insert a pair **key-data** in the hash table

2. **hash_table_delete_key_data** -> function will remove the pair **key-data** from the hash table, there exists just one pair **key-data**.

3. **hash_table_delete_hash** -> function will compute first the hash of the selected key and will remove the whole bucket of the corresponding hash result of the key.

4. **hash_table_delete_key** -> function to remove the **key-data** node but data value doesn't matter, in the second function the node will be removed if key and data are the same but in the second the data value does not count in the deletetion operation.

Examples of using these 4 functions:

```C
    // Let's suppose we took the **ht** from above code snippet

    ......

    // Lets insert some values

    for (size_t i = 0; i < number_of_supposed_keys; ++i) {
        char *str = malloc(MAX_STRING_SIZE);

        scanf("%s", str);

        hash_table_insert(ht, make_pair(str, ltoptr(size_t, strlen(str))));

        // OR

        size_t str_size = strlen(str);
        hash_table_insert(ht, str, &str_size);
    }

    // Let's remove a key-data value pair

    scl_error_t err = hash_table_delete_key_data(ht, make_pair("WOW", ltoptr(size_t, 3)));

    // If the data was found and the deletion went successfully than SCL_OK will be return
    // otherwise an error will be return, you shall call scl_error_message function to get
    // a verbose explanation of the error

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    // The same rule applies to hash_table_delete_hash and to hash_table_delete_key
```

>**NOTE:** It is very important to choose a wise hash function and an initial capacity, because the performance of your program may depend of your choice.

## How to find information into the hash table?

You will have 3 functions to help you in fetching information from hash table:

1. **hash_table_find_key_data** -> the function will search for the **key-data** pair value and if the pair is found in hash table, a pointer to the data location will be returned. It is very important to understand that key value cannot be modified because it will break the proprety of the red black tree from hash tables.

2. **hash_table_find_data** -> function will search for the key value in the entire hash table and if the key was found it will return a pointer to the data location in the heap memory. Aswell the key pointer doesn't have to be accessed by users or to be modified, however you can delete a key or a key-data pair if u do not need it any more.

3. **hash_table_contains_key_data**

An example of using it can be:

```C
    // Let suppose that this code is the continuation of the above code

    size_t str_size = *(const size_t *)hash_table_find_key(ht, "WOW");

    // If the "WOW" string exists in the following hash table than str_size should be 3
    // however if "WOW" does not exist in the hash table the above statement will create
    // a segmentation fault because we try to cast a NULL pointer.

    if (1 == hash_table_contains_key_data(ht, "WOW", ltoptr(size_t, 3))) {
        printf("Pair is in the hash table\n");
    } else {
        printf("Pair is not in the hash table\n");
    }
```

## Functions to traverse a hash table or one bucket individually ?

Because the hash table is an array of red-black trees the traversal is the exact as a binary search tree traversal, so in the main code we have inorder, preorder, postorder and level traversal and you have one function to traverse just one bucket and another function to traverse all the hash table in desired method.

An example of using traversal functions:

```C
    // First let's creeate an action function that in this case will be a printing function

    void print_data(void * const data) {
        if (NULL != data) {
            const size_t * const t_data = data;

            printf("%lu", *t_data);
        }
    }

    // This will print the bucket indexed at 100 % size(ht.
    hash_table_bucket_traverse_inorder(ht, 100, &print_data);

    // This will print the whole hash table
    hash_table_traverse_inorder(ht, &print_data);
```

>**NOTE:** The function will print the whole hash table from bucket indexed as 0 to bucket indexed as hash_table_size - 1. You have 4 choices, to travers the hash table in inorder, preorder, postoreder and in level order as a binary search tree.

>**NOTE:** You can use a map function instead of a printing function for example to increase the data value with 10 or to multiply it with (-3). You can do whatever you want in map function because it will effect just the data value, where the key data value stays untouched so the proprety of the red black tree is not destroied and the has table stays performant.

>**NOTE:** There are also other functions that are not so important as above functions and have an easy API, i encourage you to read the header file and to look up on [scl_hash_table.c](../src/scl_hash_table.c).

## For some other examples of using hash tables you can look up at [examples](../examples/hash_table/)