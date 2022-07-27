# Documentation for binary search tree object (bstUtils.h)

## How to create a binary search tree and how to destroy it?

If you read all previous documentations on lists, stacks and queue that the process of creating and removing are quite similar.

For creating a bst tree object you just need to:

```C
    // Somewhere in your program

    bst_tree_t *my_tree = create_bst(&compare_data, &free_data);
```

>**NOTE:** As you observe you should provide a compare and a free data function, however if the compare function is a must, because data is inserted in the bst according to compare function the free function is optional. If you insert data that is not dynamically allocated by yourself and it is maintained on the stack that you must not provide a free function.

Example of free function:

```C
    // Suppose your desire data is like
    typedef struct {
        char *name;
        int age;
    } person;

    // Than your free function mush be like

    void free_person(const void *data) {
        if (NULL == data) return;

        preson *t_data = data;

        free(t_data->name);
    }
```

>**NOTE:** In any other case that you do not allocate memory for your single data element that no free function should be provided.

If you want to delete a binary search tree object and to free evry byte of memory ocupied by it you should call the following function:

```C
    // Somewhere in your code

    free_bst(my_tree);

    // As simple as that, you also can pass a NULL tree
```

## How to insert and how to remove elements from binary search tree ?

According to following functions:

```C
    scl_error_t bst_insert(bst_tree_t *tree, const void *data, size_t data_size);
    scl_error_t bst_delete(bst_tree_t *tree, void *data, size_t data_size);
```

Let's assume that we work with integers in our program, so for now no need for a free function that's good:

```C
    int main(void) {
        scl_error_t err_msg = SCL_OK;

        bst_tree_t *my_tree = create_bst(&compare_int, 0);
        // You can find compare_int into scl_func_types.h

        for (int i = 0; i < 100; ++i) {
            int data = 0;
            scanf("%d", &data);
            if ((err_msg = bst_insert(my_tree, &data, sizeof(data))) != SCL_OK) {
                scl_error_message(err_msg);
            }
        }

        int remove_data = 7;

        // Removes node containing 7 if it exists on my_tree
        if ((err_msg = bst_delete(my_tree, &remove_data, sizeof(remove_data))) != SCL_OK) {
            scl_error_message(err_msg);
        }

        // Do not forget to free memory
        free_bst(my_tree);

        return 0;
    }
```

>**NOTE:** You are not allowed to insert different object types into the bst Tree. The data has to have the same type, otherwise the behavior will evolve into a segmentation fault.

## Accessing nodes and data from binary search tree ?

For this section we have the following functions:

```C
    uint8_t is_bst_empty(bst_tree_t *tree);
    bst_tree_node_t* bst_find_data(bst_tree_t *tree, const void *data);
    int32_t bst_node_level(bst_tree_t *tree, bst_tree_node_t *baseNode);
    bst_tree_node_t* get_bst_root(bst_tree_t *tree);
    size_t get_bst_size(bst_tree_t *tree);
    bst_tree_node_t* bst_max_node(bst_tree_t *tree, bst_tree_node_t *root);
    bst_tree_node_t* bst_min_node(bst_tree_t *tree, bst_tree_node_t *root);
    void* bst_max_data(bst_tree_t *tree, bst_tree_node_t *root);
    void* bst_min_data(bst_tree_t *tree, bst_tree_node_t *root);
    bst_tree_node_t* bst_predecessor_node(bst_tree_t *tree, const void *data);
    bst_tree_node_t* bst_successor_node(bst_tree_t *tree, const void *data);
    void* bst_predecessor_data(bst_tree_t *tree, const void *data);
    void* bst_succecessor_data(bst_tree_t *tree, const void *data);
    bst_tree_node_t* bst_lowest_common_ancestor_node(
        bst_tree_t *tree, const void *data1, const void *data2
    );
    void* bst_lowest_common_ancestor_data(
        bst_tree_t *tree, const void *data1, const void *data2
    );
```

The functions do exacty what their name says, now let's see some quick examples of how to use them

```C
    /* Again we are working with int type,
       but you can work with any type even your defined type
    */

    // I will need a function to work with the nodes

    void printData(bst_tree_t *tree, const bst_tree_node_t *node) {
        if (node == tree->nil || node->data == NULL)
            return;

        printf("%d ", *(const int *)node->data);
    }

    // Now let the carnage begin
    int main(void) {
        bst_tree_t *my_tree = create_bst(&compare_int, 0);

        for (int i = 0; i < 100; ++i) {
            int data = 0;
            scanf("%d", &data);
            bst_insert(my_tree, &data, sizeof(data));
        }

        int data = 4;
        printf("Level of node 4 is : %d\n", bst_node_level(my_tree, bst_find_data(my_tree, &data)));

        printf("Successor and Predecessor of node 4 is:\n");
        printData(my_tree, bst_successor_node(my_tree, &data));
        printData(my_tree, bst_predecessor_node(my_tree, &data));

        int data = 7;
        // !!! I supposed here that there is a successor of a successor
        printf("Successor of node's 7 successor is : %d",
                *(const int *)bst_successor_data(my_tree, &data));

        

        free_bst(bst);
    }
```

>**NOTE:** The rest of the functions that were not described in the example above work just like them, for example bst_min_node returns the minimum node from binary search tree and bst_min_data return the minimum data from the binary search tree.

## How to print the binary search tree, can I modify all nodes ?

I have prepared 4 functions that will help you traverse you binary search tree:

* bst_traverse_inorder
* bst_traverse_preorder
* bst_traverse_postorder
* bst_traverse_level

>**NOTE:** All of the above functions take as input your binary search tree and traverse evry single node from it.

>**NOTE:** We will discuss juust one function of them, because they do the same thing but in different methods.

**Let's print some nodes** :  you want to print all nodes not just successor or the lowest common ancestor, first you will have to define an **action** function that will do this job for you.
The definition of an **action** function is:

```C
    void action(bst_tree_t* t, const bst_tree_node_t *node);
```

>**It takes one node and does whatever it wants**

If we want to print the nodes we will have to define the **printData** functions from the above code:

```C
    // this is an action function, takes a node and does something
    void printData(bst_tree_t* tree, const bst_tree_node_t *node) {
        if (node == tree->nil || node->data == NULL)
            return;

        printf("%d ", *(const int *)node->data);
    }

    int main() {
        // Suppose you've allocated a bst tree and inserted in it elements

        // One way of printing the nodes is

        bst_traverse_inorder(mytree, &print_data); // You may not check such function for error but if you want go on

        // Yes !!! Simple as that
    }
```

If you want to do something more interesting you can define another action function as follows:

```C
    void map_nodes(bst_tree_t* tree, const bst_tree_node_t *node) {
        if (node == tree->nil || node->data == NULL)
            return;

        int *fa = node->data;

        *fa = *fa + 10;
    }

    // This function will take every node and will increase it's value with 10
```

>**NOTE:** You are free to define any action function, **BUT** if your action functions modify the data from the nodes you **MUST** provide an injective function, it is a **MUST** because just injective mapping functions will preserve the binary search proprety.

>**NOTE:** Suppose your mapping function is a square function which is not an injective function that **(-1)** node and **(1)** node will have the same value after mapping which will break the binary search tree.

## What if I want to print the output in a file ?

It is very easy to print your data from the bst in another file or another stream, you'll have to use the `freopen` function provided by standard library of C.

Example:

```C
    int main(void) {
        bst_tree_t *my_tree = create_bst(&compare_int, 0);

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
        bst_traverse_level(mytree, &print_data);
        printf("\n");

        free_bst(my_tree);
        fclose(fout);
    }
```

>**NOTE:** You can also use freopen on **stdin** stream to read data from a desired file and not to worry about `fscanf, fgets, etc...`.

---
## For some other examples of using binary search trees you can look up at /examples/bst_tree