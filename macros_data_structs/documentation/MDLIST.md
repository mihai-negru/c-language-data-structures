# Documentation for MDLIST

## Description

In this readme file we will walk through the MDLIST structure and its utilities. We will learn how to use it and what are the best practises for this structure.

### **Table of Contents**

1. [`include and define`](#include-and-define)
2. [`creating and freeing`](#creating-and-freeing)
3. [`fetching data with accumulators`](#fetching-data-with-accumulators)
4. [`finding`](#finding)
5. [`changing`](#changing)
6. [`map and filter`](map-and-filter)
7. [`traversing`](#traversing)

### `include and define`

In order to use the structures and the whole api, you need to clone the "**m_dlist.h**" file into your project. The for including you just need to simply include it.

```c
  #include "path_to_file/m_dlist.h"
```

For now you are ready to use the api.

In order to access the structures and the methods you have a lot options. Every function should be included separately or all at once.

For **defining** the structure (which is the core of the api):

```c
  MDLIST(id, type) // without ending ;
```

the *id* represents the name of the structure and the *type* represents the type of the data stored in the structure.

For example:

```c
  MDLIST(test, int) // will create the structure test_mdlist_t
```

>**NOTE:** It is allowed to specify NO id, however it is a god practice, because you may have name collisions with other structures defined in the libc or other libraries.

Other methods are included like following:

```c
  MDLIST_PUSH(test, int) // defines the test_mdlist_push method
  MDLIST_POP(test, int) // defines the test_mdlist_pop method
```

it is very important that the id and type be the same as the definition of the structure.

If you want to add the whole api in the file you just have to:

```c
  MDLIST_ALL(id, type) // now you have defined everything
```

### `creating and freeing`

The creating and freeing process is not very hard, however you should take care of some things:

* You assing the *freeing* job to the struture.
* You take care by yourself of *freeing* memory.

Some *types* do not require some freeing mechanism, like primitive types (int, double, etc..). Also structures that contain primitive types. However if the type is a pointer or a structure containg pointers, you have two choices, assign the job to the structure or do it yourself.

Let's dive into some code in order to undestand better:

```c
  // In order to create the structure let's define it
  MDLIST(doc, int)

  // the doc_mdlist_t struture is a pointer
  // typedef struct doc_mdlist_s *doc_mdlist_t;

  int compare_func(const int * const a, const int * const b) {
    return *a - *b;
  }
  ...

  int main(void) {
    doc_mdlist_t list = doc_mdlist(&compare_int, NULL);

    doc_mdlist_free(&list);
  }
```

So the type chosen is int, and I have assinged the name of *doc*. For the creating of an doubly linked list, we need two things:

* A compare function for the type.
* A free function if it is required.

In this case we did not need a free function because the type is a primitive one, so the *free* function will not free the data and will free just the nodes of the structure.

An example of freeing a structure:

```c
  typedef struct test_s {
      int *some;
  } test_t;

  MDLIST_ALL(doc, test_t)

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
    doc_mdlist_t list = doc_mdlist(&compare_test, &free_test);

    doc_mdlist_free(&list);
  }
```

Now let's see how can you by yourself take care of the memory:

```c
  int main(void) {
    doc_mdlist_t list = doc_mdlist(&comapre_test, NULL);

    ... // here we do some inserts and other operations

    // Now, how to free the memory
    test_t head_data;
    while (!doc_mdlist_empty(list)) {
      doc_mdlist_head(list, &head_data);
      free(head_data.some);
      doc_mdlist_pop(list);
    }

    // Basically the free function does the exact same thing, but faster.
  }
```

This method brings an overhead, because the freeing can be done just using the pop method, which has a bigger cost than the *free_method*.

>**NOTE:** It is good practice to specify the free function at the begining, for some other functions (liniar structures) the two methods have the same complexity, however for the tree structures the first method has O(n) time complexity, and the second method has O(nlogn) time complexity.

The second method is very good when you want to move the data from a structure to another and do not want to free the memory for it, just to move the pointers asside, for example a structure can have a `NULL` free function and the other one to have a valid one, for example:

```c
  int main(void) {
    doc_mdlist_t list = doc_mdlist(&compare_test, NULL);
    doc_mdlist_t list1 = doc_md_list(&compare_test, &free_test);

    .... // add some data in the list structure

    test_t head_data;
    while (!doc_mdlist_empty(list)) {
      doc_mdlist_head(list, &head_data);
      doc_mdlist_push(list1, head_data);
      doc_mdlist_pop(list);
    }

    doc_mdlist_free(&list); // will not free the head_data
    doc_mdlist_free(&list1); // Here all the memory will be freed
  }
```

### `fetching data with accumulators`

In order to use the data from the structure, to process it and to use for you needs, you have to have an accumulator, where to keep those values.

As you seen above, in order to fetch the head data, we defined a variable and sent it to the function to get filled.

Some functions like finding, they do not require a valid accumulator, and can put `NULL` inside it. However if the function's main role is to return something than the accumulator is needed, for example, accessing the head data, the single purpose is too return the head data.

for example:

```c
  int main(void) {
    int head = 0;

    if (doc_mdlist_head(list, &head) != M_OK) {
      // handing the error
    }    
  }
```

### `finding`

In order to check if some data exists, or if to modify the data from the structure, make sure you have defined the `MDLIST_FIND` macro.

```c
  int main(void) {
    if (doc_mdlist_find(list, 2, NULL /* The accumulator*/) != M_OK) {
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

    doc_mdlist_find(list, data, &find);

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

    doc_mdlist_find(list, data, &find);

    *find.some = 10; // Now we have modified the value, and everything looks alright, the order of nodes has not been changed.
  }
```

The **ID** functionality is only necessary if you want to use the **"_mdlist_push_order"** method, that keeps the data inside the list as ordered objects, so a modification on the actual data may break the order of the struture, however if you do not care about the order that it is alright not to specify an ID field.

### `changing`

In order to modify some values you have a fully api for it:

```c
  MDLIST_CHANGE(doc, int) // or/and MDLIST_CHANGE_IDX(doc, int)

  int main(void) {
    // from a list of list = [1, 2, 3, 4, 5]

    doc_mdlist_change(list, 1, 10);

    // this will result in a [10, 2, 3, 4, 5]

    doc_mdlist_change_idx(list, 0, 10);

    // Gives the same result as the upper method
  }
```

### `map and filter`

The list structures (and just the lists) come with builtiin functionality like *filter* and *map*,
by default when including the *MDLIST_ALL*, you define a mapping between the `type` to `type`, which means that you can map
a list (to transform it) in another list of the same type. A very important thing is that you can make mappings between two
different types, however you have to define a function that will do the following mapping, as follows:

```c
  MDLIST(doc1, int)
  MDLIST(doc2, double)
  MDLIST_MAP(doc1, int, doc2, double) // from doc1 to doc2

  // The mapping function
  double map(const int *const a) {
    return 1.0 * (*a) / 2;
  }

  int main(void) {
    doc1_mdlist_t l1 = doc1_mdlist(...);
    doc2_mdlist_t l2 = doc1_to_doc2_mdlist_map(l1, &map, &cmp_double, &free_double);
  }
```

For filter the process is the same, however you can filter the data and to create a list of the same type.
The filter function, just reduces the number of the nodes from the list:

```c
  MDLIST_FILTER(doc, int)

  // Check if the value is odd
  mbool_t filer(const int *const a) {
    return *a & 1;
  }

  int main(void) {
    // Take the list = [0,1,2,3,4,5,6,7,8,9]

    doc_mdlist_t list2 = doc_mdlist_filter(list, &filter);

    // Then the list2 = [1, 3, 5, 7, 9]
  }
```

### `traversing`

Working with data structures sometimes require a full printing for the structure, or an all-in check of the structure.

For this cases we have some traversing methods, that traverse all the structure in some differents manners.

for example we can view the tree by traversing it and print the values:

```c
  MDLIST_TRAVERSE(doc, int)

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
      doc_mdlist_push(list, i);
    }

    doc_mdlist_traverse(list, &print_int);
    // This wil generate [ 0 1 2 3 4 5 6 7 8 9 ]

    doc_mdlist_traverse_(list, &print_sq_int);
    // This wil generate [ 0 1 4 9 16 25 36 49 64 81 ]
  }
```

Usually you will use this functions mostly for printing, however you can run a whole check of the structure, for example, let's check if all the numbers are odd:

```c
  MDLIST_TRAVERSE(doc, int)

  int odds; // We need to use a global variable for this

  void check_odd(const int * const a) {
    if (a != NULL) {
      odds += (*a & 1); // Add 1 for odd add 0 for even
    }
  }

  int main(void) {
    for (int i = 1; i < 20; i += 2) {
      doc_mdlist_push(tree, i);
    }

    odds = 0;
    doc_mdlist_traverse(tree, &check_odd);

    if (odds == doc_mdlist_size(tree)) {
      // All number are odd
    } else {
      // Just `odds` numbers are odd
    }
  }
```

In order to se other examples on how to use the structures or to run some benchmark tests you can access the examples from the [mdlist](../examples/README.md) section.
