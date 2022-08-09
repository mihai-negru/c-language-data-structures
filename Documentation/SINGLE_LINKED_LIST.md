# Documentation for linked list object ([scl_list.h](../src/include/scl_list.h))

## How to create a linked list and how to destroy it?

In the scl_list.h file you have two functions that helps you to create and delete a linked list.

```C
    list_t*    create_list       (compare_func cmp, free_func frd, size_t data_size);
    void       free_list         (list_t * const __restrict__ list);
```

create_dlist function will allocate a list on heap and will return a pointer to its location. However you should provide 2 function that are neccessary for maintaing the linked list.

```C
    typedef int     (*compare_func) (const void * const elem1, const void * const elem2);
    typedef void    (*free_func)    (void *data);

    // Check scl_config.h for any function definitions
```

Function will take 2 generic pointers and according to user needs will return:

    1. 1 - if elem1 > elem2
    2. -1 - if elem1 < elem2
    3.  0 - if elem1 == elem2

Example of compare_data function for **int** data type:

```C
    int compare_data(const void * const elem1, const void * const elem2) {
        const int * const fa = elem1;
        const int * const fb = elem2;

        return *fa - *fb;
    }
```

If you want to print the output in a file, you will have to use freopen on **stdout** stream to redirect it to another file, and then print your data as usual.

```C
    void print_data(void * const elem) {
        printf("%d ", *(const int * const)elem);

        // You also shall consider calling freopen function of stdout and to use just printf
        // these is very good because you do not declare a FILE pointer globally
    }
```

free_data function is optional if you do not use dynamically data types. Every thing that you allocate on heap by yourself must be specified in **free_data** function.
If no free is needed that NULL should be passed in `create_list` function.

Exaple of type:
```C
    typedef struct {
        char *name;
        int *marks;
        int GPA;
        char first_letter;
        double average;
    } student;
```

An example of `free_data` function for this structure should be:

```C
    void free_data(void *elem1) {
        student *fa = elem1;

        if (NULL != fa->name) {
            free(fa->name);
        }
        
        fa->name = NULL;

        if (NULL != fa->marks) {
            free(fa->marks);
        }

        fa->marks = NULL;
    }
```

> **NOTE:** If any proccess of `create_list` fails than an error will be printed into the **stderr** will be thrown.

**If you want to print the entire list** than you should call `list_traverse` function.

> **NOTE:** If list is not allocated than nothing will be printed. If list is empty that a set of square paired brackets will be printed **[ ]** and if list is not empty than every element from the list will be printed according to **print_data** function provided by the user.

The last function is for freeing memory from heap allocated for the list.

**`free_dlist`** will take a list as input and will free every element according to **free_data** function provided by user. If free_data function is NULL than content of one data will be not removed (because is allocated on stack not on the heap -- No memory leaks will be generated). Every pointer that is freed is also set to NULL pointer.

Example of creating a linked list containing int data types:
```C
    list_t *list = create_list(&compare_int, NULL, sizeof(int));
    list_traverse(list, &print_int);
    free_list(list);
```

## How to insert and how to remove elements from double linked list ?

In this section we will cover 7 functions dedicated for insertion and deletion from linked list object.

Insertion in double linked list works just like insertion in **vectors** from C++.

There are 4 types of insertion:

* list_insert - inserts an element to the end of the list
* list_insert_front - inserts an element to the beginnign of the list
* list_insert_order - insers an element in order in list according to compare_data function rule
* list_insert_index - inserts an element to an index in the list

Let's assume that we works with integers and we would like to maintain a double linked list of integers,
insertion would show like:

```C
    list_t *list = create_list(...);
    int data = 0; // Dummy value not necessary

    for (int i = 0; i < 10; ++i) {
        scanf("%d", &data); // You could replace with fscanf
        if (list_insert(list, toptr(data)) != SCL_OK) {
            // Something went wrong
            scl_error_message(err); // To print verbose explanation of the current error
        }
    }
```

>**NOTE:** If you will want to use the `toptr` and `ltoptr` macros you will have to include [scl_func_types.h](../src/include/scl_func_types.h). You can use istead the toptr
and ltoptr their definition, but i find it more fancy to use them.

If you have dynamic elements in a structure and want to store the strucutre in the list you shouls do as follows:

```C
    typedef struct {
        char *name;
        int age;
    }   person;

    int compare_person(const void * const data1, const void * const data2) {
        const person * const f1 = data1;
        const person * const f2 = data2;

        return (f1->age - f2->age);
    }

    void free_person(void *data) {
        if (NULL == data) {
            return;
        }

        person *t_data = data;

        if (NULL != t_data->name) {
            free(t_data->name);
            t_data->name = NULL;
        }
    }

    ....

    list_t *list = create_list(&compare_person, &free_person, sizeof(person));

    for (int i = 0; i < 10; ++i) {
        char *name = malloc(SET_A_WORD_SIZE);
        int age = 0;
        scanf("%d", &age);
        scanf("%s", name);
        list_insert(list, ltoptr(person, { make_pair(.name = name, .age = age) })); // Returns a scl_error_t for you to check
    }

    // OR

    for (int i = 0; i < 10; ++i) {
        person pers;
        pers.name = malloc(SET_A_WORD_SIZE);
        pers.age = 0;
        scanf("%d", pers.age);
        scanf("%s", pers.name);
        list_insert(list, toptr(pers)); // Returns a scl_error_t for you to check
    }

    // It is very important not to free manually pers.name
    // This will be done in freeing the entire list

    free_list(list); // Returns a scl_error_t but it is not so important as in insertion or deletion
    // Will free every allocation made for list including data.name
```

> **NOTE:** In this case if no free_data function is provided the program will result in memory leaks. If we change from `char *name`, into `char name[SIZE_OF_A_WORD]` then no free_data should be provided.

> **NOTE:** list_insert_front, list_insert_order and list_insert_index works just like list_insert, but have different proprieties.

> **NOTE:** in list_insert_index if provided index is greter or equal to list size than element will be inserted at the end of the list.

**If you want** to delete some elements from a list you have 3 choices:

* Delete element by data - provide a data for function to find and delete (list_delete_data)
* Delete element by index - provide an index for function to find and delete (list_delete_index)
* Delete a range of elements - provide a left and a right index for function to erase (list_erase)

Let's take the above example of insertion with integers and let remove some elements:

```C

    scl_error_t err = SCL_OK;

    // By data
    data = 4;
    if ((err = list_delete_data(list, toptr(data))) != SCL_ERR)
        scl_error_message(err);

    // By index
    if ((err = list_delete_index(list, 4)) != SCL_ERR)
        scl_error_message(err);

    // By range
    if ((err = list_erase(list, 1, 3)) != SCL_ERR)
        scl_error_message(err);;
```

> **NOTE:** It is a good practise to embrace an if statement around deletion elements but you can do same pattern when inserting an element in the list.

> **NOTE:** for list_erase, you can provide different left and right index even in a random form. For example if you will pass left and right index out of bound then the last element will be removed, if left index is greater than right one, they both will be swapped.

## How to access items within the list ?

I also provided two functions that can find and return pointers to nodes from list

* list_find_data - returns a pointer to first occurence of data node that contains **data** value
* list_find_index - returns a pointer to data node positioned at provided index in the list

> **NOTE:** If no data node is found than NULL pointer will be returned

Example of using list finds:

```C
    // list = 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> NULL
    int data = 3;

    // By data
    const int *node1 = list_find_data(list, toptr(data));

    // When changing the value of one data from list you shall not find it
    // the function will by itself find data pointer and will modify it's value

    // By index
    const int *node2 = list_find_index(list, 4);

    // Now you cand do stuff with nodes for example swap them 
```

## Can I modify value of a node data, can I modify type of a node data ?

You easily can modify a value of a node data, but you **CANNOT** change it's data type.

If you try to change data between typed that have different size you can encounter a memory leak even a segmentation fault. If types have the same size that program may result into an **unknown behavior**.

Two functions that change data of nodes are:

* list_change_data - changes data of a node with another value
* list_swap_data - interchanges data from two valid nodes

Example of changing and swapping two data:

```C
    // list = 1 -> 2 -> 3 -> 4 -> 5
    const int * node1 = list_find_index(list, 0);
    const int * node2 = list_find_index(list, list->size - 1);

    list_swap_data(list, node1, node2); // Also returns a scl_error_t
    
    list_change_data(list, node1, ltoptr(int, 20)); // Also returns a scl_error_t

    // In this case we didn't know the values in the list but if we know
    // that list[0] = 4 and list[list->size - 1] = 7; we could do
    // list_swap_data(list, ltoptr(int, 4), ltoptr(int, 7));
    // Same applies to change data function

    // list = 20 -> 2 -> 3 -> 4 -> 5
```

## Some special functions

Two important functions provided in **scl_list.h** are:

* Filter function - creates a new list and selects some elements based of filter rule
* Map function / Action function - modifies in-place the current list by mapping every element into a new element of the same type

> **NOTE:** Filter function must return 1 for true and 0 for false. When true, element is added in the new list.

Examples of list filter and mapping

```C
    int filter(const void * const elem) {
        const int * const fa = elem;

        if (1 == *fa) return 1;

        return 0;
    }

    void map(void * const elem) {
        int * const fa = elem;

        *fa = (*fa) % 2;
    }

    ...

    // list = 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10

    list_traverse(list, &map); // Check here for SCL_OK 

    // list = 1 -> 0 -> 1 -> 0 -> 1 -> 0 -> 1 -> 0 -> 1 -> 0

    list_t *new_list = list_filter(list, &filter);

    // newList = 1 -> 1 -> 1 -> 1 -> 1 -> NULL

    printf("We have %lu odd numbers\n", get_list_size(new_list));

    // Do not forget to free space
    free_list(new_list);
    free_list(list);
```

> **NOTE:** If filter function return **0** for every element then NULL pointer will be returned and no new_list will be created, however you can pass a NULL linked list pointer to **free_list**, but it will have no effect.

## For some other examples of using double linked lists you can look up at [examples](../examples/list/)

