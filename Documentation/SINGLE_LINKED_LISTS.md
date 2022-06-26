# Documentation for linked list object (listUtils.h)

## How to create a linked list and how to destroy it?

In the listUtils.h file you have three functions that helps you to create and delete a linked list.

```C
    linkedList* createLinkedList(
        int (*compareData)(void *, void *),
        void (*printData)(void *),
        void (*freeData)(void *))
```

createLinkedList function will allocate a list on heap and will return a pointer to its location. However you should provide 3 function that are neccessary for maintaing the linked list.

```C
    int compareData(void *elem1, void *elem2)
```

Function will take 2 generic pointers and according to user needs will return:

    1. 1 - if elem1 > elem2
    2. -1 - if elem1 < elem2
    3.  0 - if elem1 == elem2

Example of compareData function for **int** data type:

```C
    int compareData(void *elem1, void *elem2) {
        int *fa = (int *)elem1;
        int *fb = (int *)elem2;

        return *fa - *fb;
    }
```

printData function will display output on **stdout**, hovewer you cannot print a data by using a filestream. The single solution is to declare a globale scope **FILE** variable and use *fprintf*s or *fwrite* instead of *printf*.

```C
    FILE *fin = NULL; // Allocate in main function
    void printData(void *elem) {
        fprintf(fin, "%d ", *(int *)elem);
        // Instead of printf("%d", *(int *)elem);
        // For int data type
    }
```

freeData function is optional if you do not use basic data types such as int, float, double, or static arrays you should provide a function to remove a parameter. Evrery thing that you allocate on heap by yourslef must be specified in **freeData** function.
If no free is needed that NULL should be passed in `createLinkedList` function.

Exaple of type:
```C
    typedef struct {
        char *name;
        int *marks;
        int GPA;
        char firstLetter;
        double average;
    } student;
```

An example of `freeData` function for this structure should be:

```C
    void freeData(void *elem1) {
        student *fa = (student *)elem1;

        if (fa->name)
            free(fa->name);

        if (fa->marks)
            free(fa->marks);
    }
```

> **NOTE:** If any proccess of `createLinkedList` fails than an exception will be thrown.

**If you want to print the entire list** than you should call `list_print` function.

> **NOTE:** If list is not allocated than nothing will be printed. If list is empty that a set of square paired brackets will be printed **[ ]** and if list is not empty than every element from the list will be printed according to **printData** function provided by the user.

The last function is for freeing memory from heap allocated for the list.

**`list_free_all`** will take a list as input and will free every element according to **freeData** function provided by user. If freeData function is NULL than content of one data will be not removed (because is allocated on stack not on the heap -- No memory leaks will be generated). Evry pointer that is freed is also set to NULL pointer.

Example of creating a linked list containing int data types:
```C
    linkedList *list = createLinkedList(&compareInt, &printInt, NULL);
    list_print(list);
    list_free_all(list);
```

## How to insert and how to remove elements from linked list ?

In this section we will cover 7 functions dedicated for insertion and deletion from linked list object.

Insertion in linked list works just like insertion in **vectors** from C++.

There are 4 types of insertion:

* list_insert - inserts an element to the end of the list
* list_insert_front - inserts an element to the beginnign of the list
* list_insert_order - insers an element in order in list according to compareData function rule
* list_insert_index - inserts an element to an index in the list

Let's assume that we works with integers and we would like to mantain a linked list of integers
insertion would show like:

```C
    linkedList *list = createLinkedList(...);
    int data = 0; // Dummy value not necessary

    for (int i = 0; i < 10; ++i) {
        scanf("%d", &data); // You could replace with fscanf
        list_insert(list, &data, sizeof(int));
    }
```

If you have dynamic elements in a structure and want to store the strucutre in the list you shouls do as follows:

```C
    typedef struct {
        char *name;
        int age;
    }   person;

    ....

    linkedList *list = createLinkedList(...);
    person data;

    for (int i = 0; i < 10; ++i) {
        data.name = malloc(SET_A_WORD_SIZE);
        scanf("%d", &data.age);
        scanf("%s", data.name);
        insert(list, &data, sizeof(person));
    }

    // It is very important not to free manually data.name
    // This will be done in freeing the entire list

    free_list_all(list);
    // Will free every allocation made for list including data.name
```

> **NOTE:** In this case if no freeData function is provided the program will result in memory leaks. If we change from `char *name`, into `char name[SIZE_OF_A_WORD]` then no freeData should be provided.

> **NOTE:** list_insert_front, list_insert_order and list_insert_index works just like list_insert, but have different proprieties.

> **NOTE:** in list_insert_index if provided index is greter or equal to list size than element will be inserted at the end of the list.

**If you want** to delete some elements from a list you have 3 choices:

* Delete element by data - provide a data for function to find and delete (list_delete_data)
* Delete element by index - provide an index for function to find and delete (list_delete_index)
* Delete a range of elements - provide a left and a right index for function to erase (list_erase)

Let's take the above example of insertion with integers and let remove some elements:

```C
    // By data
    data = 4;
    if (list_delete_data(list, &data))
        printf("Failed to remove %d\n", data);

    // By index
    if (list_delete_index(list, 4))
        printf("Failed to remove index 4\n");

    // By range
    if (list_erase(list, 1, 3))
        printf("Failed to remove range [1,3]\n");
```

These 3 functions can return either 0 if deletion was successfully or 1 if something went wrong.
Some cases where functions may fail are:

1. pointer to data is NULL or data value is not in the linked list
2. provided index is to large and gets out from list range
3. list is empty and no element can be removed

> **NOTE:** It is a good practise to embrace a if around deletion elements but you can do same pattern when inserting an element in the list.

> **NOTE:** for list_erase, you can provide different left and right index even in a random form. For example if you will pass left and right index out of bound then the last element will be removed, if left index is greater than right one, they both will be swapped.

## How to access items within the list ?

I also provided two functions that can find and return pointers to nodes from list

* list_find_data - returns a pointer to first occurence of node that contains **data** value
* list_find_index - returns a pointer to node positioned at provided index in the list

> **NOTE:** If no node is found than NULL pointer will be returned

Example of using list finds:

```C
    // list = 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> NULL
    int data = 3;

    // By data
    TListNode *node1 = list_find_data(list, &data);

    // By index
    TListNode *node2 = list_find_index(list, 4);

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
    TListNode *node1 = list_find_index(list, 0);
    TListNode *node2 = list_find_index(list, list->size - 1);

    list_swap_data(list, node1, node2);
    
    data = 20;
    list_change_data(list, node1, &data, sizeof(int));

    // list = 20 -> 2 -> 3 -> 4 -> 5
```

> **NOTE:** A very important asspect is than list_swap_data will swap data pointers not the entire node pointer so node1 will point for the rest of his life to the beginnign of the list and node2 to the end of the list. list_change_data will not allocate a new node and delete the current one but will copy bytes from data pointer provided as input and will move them into the current data pointer location.

## Some special functions

Two important functions provided in **listUtils.h** are:

* Filter function - creates a new list and selects some elements based of filterFunction rule
* Map function - modifies in-place the current list by mapping evry element into a new element of the same type

> **NOTE:** Filter function must return 1 for true and 0 for false. When true, element is added in the new list.

Examples of list filter and mapping

```C
    int filterFunction(void *elem) {
        int *fa = (int *)elem;

        if (*fa == 1) return 1;

        return 0;
    }

    void* mapFunction(void *elem) {
        int *fa = (int *)elem;

        *fa = (*fa) % 2;

        return fa;
    }

    ...

    // list = 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10

    list_map(list, &mapFunction, sizeof(int));

    // list = 1 -> 0 -> 1 -> 0 -> 1 -> 0 -> 1 -> 0 -> 1 -> 0

    linkedList *newList = list_filter(list, &filterFunction, sizeof(int));

    // newList = 1 -> 1 -> 1 -> 1 -> 1 -> NULL

    printf("We have %d odd numbers\n", newList->size);

    // Do not forget to free space
    list_free_all(newList);
    list_free_all(list);
```

> **NOTE:** If filter function return **0** for every element then NULL pointer will be returned and no newList will be created, however you can pass a NULL linked list pointer to **list_free_all**, but it will have no effect.

## For some other examples of using linked lists you can look up at /examples/singleLists

