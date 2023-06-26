# Documentation for MSTACK

## Description

In this readme file we will walk through the MAVL structure and its utilities. We will learn how to use it and what are the best practises for this structure.

### **Table of Contents**

1. [`include and define`](#include-and-define)
2. [`creating and freeing`](#creating-and-freeing)
3. [`fetching data with accumulators`](#fetching-data-with-accumulators)
4. [`traversing`](#traversing)

### `include and define`

In order to use the strcutures and the whole api, you need to clone the "**m_stack.h**" file into your project. The for including you just need to simply include it.

```c
  #include "path_to_file/m_stack.h"
```

For now you are ready to use the api.

In order to access the structures and the methods you have a lot options. Every function should be included separately or all at once.

For **defining** the structure (which is the core of the api):

```c
  MSTACK(id, type) // without ending ;
```

the *id* represents the name of the structure and the *type* represents the type of the data stored in the structure.

For example:

```c
  MSTACK(test, int) // will create the structure test_mstack_t
```

>**NOTE:** It is allowed to specify NO id, however it is a god practice, because you may have name collisions with other structures defined in the libc or other libraries.

Other methods are included like following:

```c
  MSTACK_PUSH(test, int) // defines the test_mstack_push method
  MSTACK_POP(test, int) // defines the test_mstack_pop method
```

it is very important that the id and type be the same as the definition of the structure.

If you want to add the whole api in the file you just have to:

```c
  MSTACK_ALL(id, type) // now you have defined everything
```

### `creating and freeing`

The creating and freeing process is not very hard, however you should take care of some things:

* You assing the *freeing* job to the struture.
* You take care by yourself of *freeing* memory.

Some *types* do not require some freeing mechanism, like primitive types (int, double, etc..). Also structures that contain primitive types. However if the type is a pointer or a structure containg pointers, you have two choices, assign the job to the structure or do it yourself.

Let's dive into some code in order to undestand better:

```c
  // In order to create the structure let's define it
  MSTACK(doc, int)

  // the doc_mavl_t struture is a pointer
  // typedef struct doc_mstack_s *doc_mstack_t;
  ...

  int main(void) {
    doc_mstack_t q = doc_mstack(NULL);

    doc_mstack_free(&q);
  }
```

So the type chosen is int, and I have assinged the name of *doc*. For the creating of a queue, we need one thing:

* A free function if it is required.

In this case we did not need a free function because the type is a primitive one, so the *free* function will not free the data and will free just the nodes of the structure.

An example of freeing a structure:

```c
  typedef struct test_s {
      int *some;
  } test_t;

  MSTACK_ALL(doc, test_t)

  void free_test(test_t * a) {
    if (a != NULL) {
      free(a->some);

      // Do not free the 'a', just free the content of it.
    }
  }

  int main(void) {
    doc_mstack_t q = doc_mstack(&free_test);

    doc_mstack_free(&q);
  }
```

Now let's see how can you by yourself take care of the memory:

```c
  int main(void) {
    doc_mstack_t q = doc_mstack(NULL);

    ... // here we do some inserts and other operations

    // Now, how to free the memory
    test_t top_data;
    while (!doc_mstack_empty(q)) {
      doc_mstack_top(q, &top_data);
      free(front_data.some);
      doc_mstack_pop(q);
    }
  }
```

This method brings an overhead, because the freeing can be done just using the pop method, which has a bigger cost than the *free_method*.

>**NOTE:** It is good practice to specify the free function at the begining, for some other functions (liniar structures) the two methods have the same complexity, however for the tree structures the first method has O(n) time complexity, and the second method has O(nlogn) time complexity.

### `fetching data with accumulators`

In order to use the data from the structure, to process it and to use for you needs, you have to have an accumulator, where to keep those values.

As you seen above, in order to fetch the top data, we defined a variable and sent it to the function to get filled.

Some functions like finding, they do not require a valid accumulator, and can put `NULL` inside it. However if the function's main role is to return something than the accumulator is needed, for example, accessing the top data, the single purpose is too return the top data.

for example:

```c
  int main(void) {
    int acc = 0;

    if (doc_mstack_top(q, &acc) != M_OK) {
      // handing the error
    }
  }
```

### `traversing`

Working with data structures sometimes require a full printing for the structure, or an all-in check of the structure.

For this cases we have some traversing methods, that traverse all the structure in some differents manners.

for example we can view the tree by traversing it in *inorder* and printg the values:

```c
  MSTACK_TRAVERSE(doc, int)

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
      doc_mstack_push(q, i);
    }

    doc_mstack_traverse(q, &print_int);
    // This wil generate [ 0 1 2 3 4 5 6 7 8 9 ]

    doc_mstack_traverse(q, &print_sq_int);
    // This wil generate [ 0 1 4 9 16 25 36 49 64 81 ]
  }
```

Usually you will use this functions mostly for printing, however you can run a whole check of the structure, for example, let's check if all the numbers are odd:

```c
  MSTACK_TRAVERSE(doc, int)

  int odds; // We need to use a global variable for this

  void check_odd(const int * const a) {
    if (a != NULL) {
      odds += (*a & 1); // Add 1 for odd add 0 for even
    }
  }

  int main(void) {
    for (int i = 1; i < 20; i += 2) {
      doc_mstack_push(q, i);
    }

    odds = 0;
    doc_mstack_traverse(q, &check_odd);

    if (odds == doc_mstack_size(q)) {
      // All number are odd
    } else {
      // Just `odds` numbers are odd
    }
  }
```

In order to se other examples on how to use the structures or to run some benchmark tests you can access the examples from the [mstack](../examples/README.md) section.
