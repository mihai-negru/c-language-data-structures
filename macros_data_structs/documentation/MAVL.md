# Documentation for MAVL

## Description

In this readme file we will walk through the MAVL structure and its utilities. We will learn how to use it and what are the best practises for this structure.

### **Table of Contents**

1. [`include and define`](#include-and-define)
2. [`creating and freeing`](#creating-and-freeing)
3. [`fetching data with accumulators`](#fetching-data-with-accumulators)
4. [`finding`](#finding)
5. [`traversing`](#traversing)

### `include and define`

In order to use the strcutures and the whole api, you need to clone the "**m_avl.h**" file into your project. The for including you just need to simply include it.

```c
  #include "path_to_file/m_avl.h"
```

For now you are ready to use the api.

In order to access the structures and the methods you have a lot options. Every function should be included separately or all at once.

For **defining** the structure (which is the core of the api):

```c
  MAVL(id, type) // without ending ;
```

the *id* represents the name of the structure and the *type* represents the type of the data stored in the structure.

For example:

```c
  MAVL(test, int) // will create the structure test_mavl_t
```

>**NOTE:** It is allowed to specify NO id, however it is a god practice, because you may have name collisions with other structures defined in the libc or other libraries.

Other methods are included like following:

```c
  MAVL_PUSH(test, int) // defines the test_mavl_push method
  MAVL_POP(test, int) // defines the test_mavl_pop method
```

it is very important that the id and type be the same as the definition of the structure.

If you want to add the whole api in the file you just have to:

```c
  MAVL_ALL(id, type) // now you have defined everything
```

### `creating and freeing`

The creating and freeing process is not very hard, however you should take care of some things:

* You assing the *freeing* job to the struture.
* You take care by yourself of *freeing* memory.

Some *types* do not require some freeing mechanism, like primitive types (int, double, etc..). Also structures that contain primitive types. However if the type is a pointer or a structure containg pointers, you have two choices, assign the job to the structure or do it yourself.

Let's dive into some code in order to undestand better:

```c
  // In order to create the structure let's define it
  MAVL(doc, int)

  // the doc_mavl_t struture is a pointer
  // typedef struct doc_mavl_s *doc_mavl_t;

  int compare_func(const int * const a, const int * const b) {
    return *a - *b;
  }
  ...

  int main(void) {
    doc_mavl_t tree = doc_mavl(&compare_int, NULL);

    doc_mavl_free(&tree);
  }
```

So the type chosen is int, and I have assinged the name of *doc*. For the creating of an avl tree, we need two things:

* A compare function for the type, because the avl tree is an ordered structure and we need to arrange the data in a specified order.
* A free function if it is required.

In this case we did not need a free function because the type is a primitive one, so the *free* function will not free the data and will free just the nodes of the structure.

An example of freeing a structure:

```c
  typedef struct test_s {
      int *some;
  } test_t;

  MAVL_ALL(doc, test_t)

  int compare_test(const test_t * const a, const test_t * const b) {
    return a->some - b->some;
  }

  void free_test(test_t * a) {
    if (a != NULL) {
      free(a->some);

      // Do not free the 'a', just free the content of it.
    }
  }

  int main(void) {
    doc_mavl_t tree = doc_mavl(&compare_test, &free_test);

    doc_mavl_free(&tree);
  }
```

Now let's see how can you by yourself take care of the memory:

```c
  int main(void) {
    doc_mavl_t tree = doc_mavl(&comapre_test, NULL);

    ... // here we do some inserts and other operations

    // Now, how to free the memory
    test_t root_data;
    while (!doc_mavl_empty(tree)) {
      doc_mavl_root(tree, &root_data);
      free(root_data.some);
      doc_mavl_pop(tree);
    }
  }
```

This method brings an overhead, because the freeing can be done just using the pop method, which has a bigger cost than the *free_method*.

>**NOTE:** It is good practice to specify the free function at the begining, for some other functions (liniar structures) the two methods have the same complexity, however for the tree structures the first method has O(n) time complexity, and the second method has O(nlogn) time complexity.

### `fetching data with accumulators`

In order to use the data from the structure, to process it and to use for you needs, you have to have an accumulator, where to keep those values.

As you seen above, in order to fetch the root data, we defined a variable and sent it to the function to get filled.

Some functions like finding, they do not require a valid accumulator, and can put `NULL` inside it. However if the function's main role is to return something than the accumulator is needed, for example, accessing the root data, the single purpose is too return the root data.

for example:

```c
  int main(void) {
    int root = 0;

    if (doc_mavl_root(tree, &root) != M_OK) {
      // handing the error
    }

    int min = 0, max = 0;

    doc_mavl_min(tree, root, &min); // finds the minimum data starting from the zroot` subtree

    doc_mavl_max(tree, root, &max); // same as minimum, but for maximum
  }
```

### `finding`

In order to check if some data exists, or if to modify the data from the structure, make sure you have defined the `MAVL_FIND` macro.

```c
  int main(void) {
    if (doc_mavl_find(tree, 2, NULL /* The accumulator*/) != M_OK) {
      // the number `2` does not exist in the structure
    } else {
      // the number `2` was found
    }
  }
```

Firstly it seams that we do not really need the accumulator, however for pointers it is very important, for example:

```c
  int main(void) {
    // we will use the test_t structure

    .... // Inserting data and processing it

    test_t find;

    test_t data;
    data.some = malloc(sizeof *data.some);
    data.some = 4; // We want to find the node with value 4

    doc_mavl_find(tree, data, &find);

    find.some = 10; // We modified the value to 10 in the struture
  }
```

As you have seen the structure permits to change the inner values from the structure, which is mainly wrong, however the tree structure has to have a field that acts like an **ID** and is never changed, in order not to mess the structure up.

As an example we can add the following field to the *test_t* structure and can change the compare function like this:

```c
  typedef struct test_s {
    int id;
    int *some;
  } test_t;

  int compare_test(const test_t *const a, const test_t *const b) {
    return a->id - b->id;
  }

  // Now we can find the id and to modify the rest of the fields that are not important for the order of the tree

  int main(void) {
    ...
    test_t data;
    data.id = 4; // We do not have to malloc the some

    test_t find;

    doc_mavl_find(tree, data, &find);

    *find.some = 10; // Now we have modified the value, and everything looks alright, the order of nodes has not been changed.
  }
```

### `traversing`

Working with data structures sometimes require a full printing for the structure, or an all-in check of the structure.

For this cases we have some traversing methods, that traverse all the structure in some differents manners.

for example we can view the tree by traversing it in *inorder* and printg the values:

```c
  MAVL_TRAVERSE_INORDER(doc, int)

  void print_int(const int* const a) {
    // The signature is important const ptr *const
    if (a != NULL) {
      printf(" %d", *a);
    }
  }

  void print_sq_int(const int* const a) {
    if (a != NULL) {
      printf(" %d", *a * *a);
    }
  }

  int main(void) {
    for (int i = 0; i < 9; ++i) {
      doc_mavl_push(tree, i);
    }

    doc_mavl_traverse_inorder(tree, &print_int);
    // This wil generate [ 0 1 2 3 4 5 6 7 8 9 ]

    doc_mavl_traverse_inorder(tree, &print_sq_int);
    // This wil generate [ 0 1 4 9 16 25 36 49 64 81 ]
  }
```

Usually you will use this functions mostly for printing, however you can run a whole check of the sttucture, for example, let's check if all the numbers are odd:

```c
  MAVL_TRAVERSE_INORDER(doc, int)

  int odds; // We need to use a global variable for this

  void check_odd(const int * const a) {
    if (a != NULL) {
      odds += (*a & 1); // Add 1 for odd add 0 for even
    }
  }

  int main(void) {
    for (int i = 1; i < 20; i += 2) {
      doc_mavl_push(tree, i);
    }

    odds = 0;
    doc_mavl_traverse_inorder(tree, &check_odd);

    if (odds == doc_mavl_size(tree)) {
      // All number are odd
    } else {
      // Just `odds` numbers are odd
    }
  }
```

In order to se other examples on how to use the structures or to run some benchmark tests you can access the examples from the [mavl](../examples/README.md) section.
