# Documentation for MPQUEUE

## Description

In this readme file we will walk through the MPQUEUEU structure and its utilities. We will learn how to use it and what are the best practises for this structure.

### **Table of Contents**

1. [`include and define`](#include-and-define)
2. [`creating and freeing`](#creating-and-freeing)
3. [`fetching data with accumulators`](#fetching-data-with-accumulators)
4. [`finding`](#finding)
5. [`changing`](#changing)
6. [`traversing`](#traversing)

### `include and define`

In order to use the strcutures and the whole api, you need to clone the "**m_pqueue.h**" file into your project. The for including you just need to simply include it.

```c
  #include "path_to_file/m_pqueue.h"
```

For now you are ready to use the api.

In order to access the structures and the methods you have a lot options. Every function should be included separately or all at once.

For **defining** the structure (which is the core of the api):

```c
  MPQUEUE(id, key_type, value_type) // without ending ;
```

the *id* represents the name of the structure, the *key_type* is the type of the priority, the data will be arranged in the structure based on the priority and the *value_type* is the type of the data that will be stored in a priority node.

For example:

```c
  MPQUEUE(test, int, int) // will create the structure test_mpqueue_t
```

>**NOTE:** It is allowed to specify NO id, however it is a god practice, because you may have name collisions with other structures defined in the libc or other libraries.

Other methods are included like following:

```c
  MPQUEUE_PUSH(test, int, int) // defines the test_mpqueue_push method
  MPQUEUE_POP(test, int, int) // defines the test_mpqueue_pop method
```

it is very important that the id and type be the same as the definition of the structure.

If you want to add the whole api in the file you just have to:

```c
  MPQUEUE_ALL(id, key_type, value_type) // now you have defined everything
```

### `creating and freeing`

The creating and freeing process is not very hard, however you should take care of some things:

* You assing the *freeing* job to the struture.
* You take care by yourself of *freeing* memory.

Some *types* do not require some freeing mechanism, like primitive types (int, double, etc..). Also structures that contain primitive types. However if the type is a pointer or a structure containg pointers, you have two choices, assign the job to the structure or do it yourself.

Let's dive into some code in order to undestand better:

```c
  typedef char *string;
  // In order to create the structure let's define it
  MPQUEUE(doc, int, string)

  // the doc_mpqueue_t struture is a pointer
  // typedef struct doc_mpqueue_s *doc_mpqueue_t;

  // This function wil generate a max heap priority queue
  int compare_priority_max(const int * const a, const int * const b) {
    return *a - *b;
  }

  int compare_priority_min(const int * const a, const int * const b) {
    return *b - *a;
  }
  ...

  int main(void) {
    doc_mpqueue_t pq = doc_mpqueue(
      10, // Initial capacity for the priority queue can be zero for the default capacity
      &compare_priority_max, // REQUIRED, function to compare priority
      NULL, // Function to free the memory allocated by priority if needed
      &compare_string, // It is not required, however if you will want to find some data it is needed
      &free_string // Function to free memory allocated by the data, if needed
    );

    doc_mpqueue_free(&pq);
  }
```

So the type chosen is int, and I have assinged the name of *doc*. For the creating of a priority queue, we need some things:

* An initial capacity, for example if you know that you will have at most 100 elements specify the initial capacity as 100, plus a little addition space, so the pushes do not bring an overhead when needed to reallocate a bigger chunk of memory.
* A compare function to compare the priorities, based on priorities you can define a min or a max priority queue.
* A free function to deallocate the priority field if neccessary.
* A compare function for the value.
* A free function for values if it is required.

In this case we did not need a free function because the type is a primitive one, so the *free* function will not free the data and will free just the nodes of the structure.

An example of freeing a structure:

```c
  typedef struct test_s {
      int *some;
  } test_t;

  MPQUEUE_ALL(doc, int, test_t)

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
    doc_mpqueue_t pq = doc_mpqueue(10, &compare_int, NULL, &compare_test, &free_test);

    doc_mpqueue_free(&pq);
  }
```

Now let's see how can you by yourself take care of the memory:

```c
  int main(void) {
    doc_mpueue_t pq = doc_mpqueue(..., &compare_test, NULL);

    ... // here we do some inserts and other operations

    // Now, how to free the memory
    test_t top_data;
    while (!doc_mpueue_empty(pq)) {
      doc_mpqueue_top(pq, &top_data);
      free(top_data.some);
      doc_mpqueue_pop(pq);
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
    int top = 0;

    if (doc_mpqueue_top(pq, &top) != M_OK) {
      // handing the error
    }

    int top_pri = 0;

    if (doc_mpqueue_top_pri(pq, &top_pri) != M_OK) {
      // handling the error
    }
  }
```

### `finding`

In order to check if some data exists, or if to modify the data from the structure, make sure you have defined the `MPQUEUE_FIND_IDX` or `MPQUEUE_FIND_PRI_IDX` macro.

When using the priority queues, you have a limit of interacting witht he structure, the finding methods, will give
you a `size_t` token (index), that can be used in any other functions, this is usually used with changing functions.

```c
  int main(void) {
    size_t token = 0;
    if (doc_mpqueue_find_idx(pq, 2, &token) != M_OK) {
      // the number `2` does not exist in the structure
    } else {
      // the number `2` was found
    }

    size_t pri_token = 0;
    if (doc_mpqueue_find_pri_idx(pq, 2, &pri_token) != M_OK) {
      // the number `2` does not exist in the structure as a priority
    } else {
      // the number `2` was found not found as a priority
    }
  }
```

### `changing`

We have two types of changing something in the structure:

* Firstly, we can change the priority of one node
* Secondly, we can modify the value of one node

>**NOTE:** For both actions we will need the token of the data or priority, which is basically the
index where the node is stored.

for example let's change the top value of an priority queue and also to change it's priority;

```c
  int main(void) {

    // It is important to have the pair inside the pqueue, or the changing will fail.

    // For now the top pair is (prio, data) as (4, 10)

    doc_mpqueue_change(pq, 0 /* Token */, /* New value */ 20); // New pair (4, 20)

    // Or let's change the priority
    doc_mpqueue_change_pri(pq, 0, 1); // New pair (1, 20)

    // The changing priority function may modify the heap structure in order to preserver
    // the property of a heap, which means that the pair could be sifted up or down.
  }
```

### `traversing`

Working with data structures sometimes require a full printing for the structure, or an all-in check of the structure.

For this cases we have some traversing methods, that traverse all the structure in some differents manners.
However for the priority queue we will traverse just the *data*, the priority is left alone.

for example we can view the tree by traversing it and printg the values:

```c
  MPQUEUE_TRAVERSE(doc, int, int)

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
      doc_mpqueue_push(pq, i, i);
    }

    doc_mpqueue_traverse(pq, &print_int);
    doc_mpqueue_traverse(pq, &print_sq_int);
  }
```

Usually you will use this functions mostly for printing, however you can run a whole check of the sttucture, for example, let's check if all the numbers are odd:

```c
  MPQUEUE_TRAVERSE(doc, int, int)

  int odds; // We need to use a global variable for this

  void check_odd(const int * const a) {
    if (a != NULL) {
      odds += (*a & 1); // Add 1 for odd add 0 for even
    }
  }

  int main(void) {
    for (int i = 1; i < 20; i += 2) {
      doc_mpqueue_push(pq, i);
    }

    odds = 0;
    doc_mpqueue_traverse(pq, &check_odd);

    if (odds == doc_mpqueue_size(pq)) {
      // All number are odd
    } else {
      // Just `odds` numbers are odd
    }
  }
```

In order to se other examples on how to use the structures or to run some benchmark tests you can access the examples from the [mpqueue](../examples/README.md) section.
