# Documentation for RED BLACK tree object ([scl_rbk_tree.h](../src/include/scl_rbk_tree.h))

## How to create a RED BLACK tree and how to destroy it?

If you read all previous documentations on lists, stacks and queue that the process of creating and removing are quite similar.

For creating an Red Black tree object you just need to:

```C
    // Somewhere in your program

    rbk_tree_t *my_tree = create_rbk(&compare_data, &free_data, sizeof(your_data));
```

>**NOTE:** As you observe you should provide a compare and a free data function, however if the compare function is a must, because data is inserted in the Red Black according to compare function the free function is optional. If you insert data that is not dynamically allocated by yourself and it is maintained on the stack that you must not provide a free function.

Example of free function:

```C
    // Suppose your desire data is like
    typedef struct {
        char *name;
        int age;
    } person;

    // Than your free function mush be like

    void free_person(void *data) {
        if (data == NULL) return;

        preson *t_data = data;

        free(t_data->name);
        t_data->name = NULL;
    }
```

>**NOTE:** In any other case that you do not allocate memory for your single data element that no free function should be provided.

If you want to delete an Red Black tree object and to free every byte of memory ocupied by it you should call the following function:

```C
    // Somewhere in your code

    free_rbk(my_tree);
    my_tree = NULL; // It is a good practice to set tree pointer after
                    // deletion as NULL, because free function will not
                    // do that for you

    // As simple as that, you also can pass a NULL tree
```

## How to insert and how to remove elements from Red Black tree ?

According to following functions:

```C
    slc_error_t rbk_insert(rbk_tree_t * const __restrict__ tree, const void * __restrict__ data);
    scl_error_t rbk_delete(rbk_tree_t * const __restrict__ tree, const void * __restrict__ data);
```

Let's assume that we work with integers in our program, so for now no need for a free function, that's good:

```C
    int main(void) {
        scl_error_t err_msg = SCL_OK;

        rbk_tree_t *my_tree = create_rbk(&compare_int, 0, sizeof(int));
        // You can find compare_int into scl_func_types.h

        for (int i = 0; i < 100; ++i) {
            int data = 0;
            scanf("%d", toptr(data));
            if ((err_msg = rbk_insert(my_tree, toptr(data))) != SCL_OK) {
                scl_error_message(err_msg);
            }
        }

        int remove_data = 7;

        // Removes node containing 7 if it exists on my_tree
        if ((err_msg = rbk_delete(my_tree, toptr(remove_data))) != SCL_OK) {
            scl_error_message(err_msg)
        }

        // Do not forget to free memory
        if ((err = free_rbk(my_tree)) != SCL_OK) {
            scl_error_message(err_msg);
        }

        // Not necessary for free function to check for error

        return 0;
    }
```

>**NOTE:** If you will want to use the `toptr` and `ltoptr` macros you will have to include [scl_func_types.h](../src/include/scl_func_types.h). You can use istead the toptr
and ltoptr their definition, but I find it more fancy to use them.

>**NOTE:** You are not allowed to insert different object types into the Red Black Tree. The data has to have the same type, otherwise the behavior will evolve into a segmentation fault.

## Accessing nodes and data from Red Black tree ?

For this section we have the following functions:

```C
    uint8_t is_rbk_empty(
        const rbk_tree_t * const __restrict__ tree
    );

    const void* rbk_find_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ data
    );

    int32_t rbk_data_level(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ data
    );

    const void* get_rbk_root(
        const rbk_tree_t * const __restrict__ tree
    );

    size_t get_rbk_size(
        const rbk_tree_t * const __restrict__ tree
    );

    const void* rbk_max_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ subroot_data
    );

    const void* rbk_min_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ subroot_data
    );

    const void* rbk_predecessor_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ data
    );

    const void* rbk_succecessor_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ data
    );

    const void* rbk_lowest_common_ancestor_data(
        const rbk_tree_t * const __restrict__ tree,
        const void * const __restrict__ data1,
        const void * const __restrict__ data2
    );
```

The functions do exacty what their name says, now let's see some quick examples of how to use them

```C
    /* Again we are working with int type,
       but you can work with any type even your defined type
    */

    // I will need a function to work with the nodes

    void print_data(const void * const data) {
        if (NULL == data) {
            return;
        }

        printf("%d ", *(const int * const)data);
    }

    // Now let the carnage begin
    int main(void) {
        rbk_tree_t *my_tree = create_rbk(&compare_int, 0, sizeof(int));

        for (int i = 0; i < 100; ++i) {
            int data = 0;
            scanf("%d", &data);
            rbk_insert(my_tree, toptr(data));
        }

        int data = 4;
        printf("Level of node 4 is : %d\n", rbk_data_level(my_tree, toptr(data)));

        // Another way to to the same effect is to use the following:
        printf("Level of node 4 is : %d\n", rbk_data_level(my_tree, ltoptr(int, 4)));

        printf("Successor and Predecessor of node 4 is:\n");

        print_data(rbk_successor_data(my_tree, toptr(data)));
        print_data(rbk_predecessor_data(my_tree, toptr(data)));

        int data = 7;
        // !!! I supposed here that there is a successor of a successor
        printf("Successor of node's 7 successor is : %d",
                *(const int *)rbk_successor_data(toptr(data)));

        free_rbk(my_tree);
    }
```

>**NOTE:** The rest of the functions that were not described in the example above work just like them, for example rbk_min_data returns the minimum data node from Red Black tree and rbk_max_data return the maximum data node from the Red Black tree.

## How to print the Red Black tree, can I modify all nodes ?

I have prepared 4 functions that will help you traverse you Red Black tree:

* rbk_traverse_inorder
* rbk_traverse_preorder
* rbk_traverse_postorder
* rbk_traverse_level

>**NOTE:** All of the above functions take as input your Red Black tree and traverse every single node from it.

>**NOTE:** We will discuss just one function of them, because they do the same thing but in different methods.

**Let's print some nodes** :  you want to print all nodes not just successor or the lowest common ancestor, first you will have to define an **action** function that will do this job for you.
The definition of an **action** function is:

```C
    void action(void * const data);
```

>**It takes one data pointer and does whatever it wants**

If we want to print the nodes we will have to define the **print_data** function from the above code:

```C
    // this is an action function, takes a node and does something
    void print_data(void * const data) {
        if (NULL == data) [
            return;
        ]

        printf("%d ", *(const int * const)data);
    }

    int main() {
        // Suppose you've allocated an Red Black tree and inserted in it elements

        // One way of printing the nodes is

        rbk_traverse_inorder(mytree, &print_data); // Not necessary to check for error but if you want go on

        // Yes !!! Simple as that
    }
```

If you want to do something more interesting you can define another action function as follows:

```C
    void map_nodes(void * const data) {
        if (NULL == data) {
            return;
        }

        int * const fa = data;

        *fa = *fa + 10;
    }

    // This function will take every node and will increase it's value with 10
```

>**NOTE:** You are free to define any action function, **BUT** if your action functions modify the data from the nodes you **MUST** provide an injective function, it is a **MUST** because just injective mapping functions will preserve the binary search proprety.

>**NOTE:** Suppose your mapping function is a square function which is not an injective function that **(-1)** node and **(1)** node will have the same value after mapping which will break the Red Black tree.

## What if I want to print the output in a file ?

It is very easy to print your data from the Red Black in another file or another stream, you'll have to use the `freopen` function provided by standard library of C.

Example:

```C
    int main(void) {
        rbk_tree_t *my_tree = create_rbk(&compare_int, 0, sizeof(int));

        // Here insert you're data as you like

        FILE *fout = NULL;

        if ((fout = freopen("outputfile.txt", "w", stdout)) == NULL) {
            printf("Could not redirect stream\n");
            exit(1);
        }

        // Now print every thing you like as printing
        // in stdout stream

        printf("Hi there!\n"); // This will be printed in outputfile.txt

        // This also will be printed in outputfile.txt
        rbk_traverse_level(my_tree, &print_data);
        printf("\n");

        free_rbk(my_tree);
        fclose(fout);
    }
```

>**NOTE:** You can also use freopen on **stdin** stream to read data from a desired file and not to worry about `fscanf, fgets, etc...`.

---
## For some other examples of using Red Black trees you can look up at [examples](../examples/red_black_tree/)