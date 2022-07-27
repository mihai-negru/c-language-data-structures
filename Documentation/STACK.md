# Documentation for stack object (scl_stack.h)

## How to create a stack and how to destroy it?

In the scl_stack.h you have two functions that will help you by creating a stack and destroying it.

```C
    stack_t* create_stack(void (*free_data)(void*));
    scl_error_t free_stack(stack_t *stack);
```

First function will take a pointer to a function that will free the content of the actual data. However you can send NULL(0) instead of a pointer. For base types as **int**, **float**, **double**, **char**, **static arrays**, **static structures**, `free_data` function should be **NULL**, if a structure that contains a pointer (and it is allocated with malloc or calloc) then a free_data function (*not NULL*) should be provided.

Example of creating a stack:

```C
    typedef struct {
        char *name;
        int age;
    } person;

    void free_person(void *a) {
        person *fa = (person *)a;

        free(fa->name);
    }

    int main() {
        stack_t *stack = create_stack(&free_person);
        free_stack(stack);

        return 0;
    }
```

>**NOTE:** If you want to print whole stack you should call print *print_stack* function that takes a valid pointer to a stack structure location and prints it from top to bottom.

>**NOTE:** All elements of the stack should be of the same type. However you can save different types of data in the same exact stack, but functions as print_stack will fail. One solution is: for a different type to make one structure that has its own print function and a variable to check types of the data, for example:

```C
    typedef struct {
        int check; // For int 0
        int elem;
    } Int;

    typedef struct {
        int check; // for float 1
        float elem;
    } Float;

    typedef struct {
        int check; // For char 2
        char elem;
    } Char;

    // And so on for every data type;

    // One way to print it would be:

    void print_data(void *a) {
        if (a == NULL) return;

        int check = *(int *)a;

        if (check == 0) {
            Int *fb = a;

            printf("%d ", fb->elem);
        } else if (check == 1) {
            Float *fb = a;

            printf("%f ", fb->elem);
        } else if (check == 2) {
            Char *fb = a;

            printf("%c ", fb->elem);
        } else {
            printf("Not known check var\n");
        }
    }
```

>**NOTE:** It is very **IMPORTANT** in this situation for the **check** variable to be the first member of the structure, otherwise it is posibble to access different memory locations which will fail into a Segmentation Fault.

>**NOTE:** Some types of structures as above and functions to print different classes of data types are described in **scl_func_types.h/.md** section.

## How to insert and how to remove elements from stack?

You have 3 function that will maintain a stack:

```C
    scl_error_t stack_push(stack_t *stack, const void *data, size_t data_size);
    void* stack_top(stack_t *stack);
    scl_error_t stack_pop(stack_t *stack);
```

Function `stack_push` will insert one element into the stack. You should pass an allocated stack into push function, however if stack pointer is NULL than no operation will be executed. As we mentioned above you can insert different data types into the stack, but you will have to provide your functions to maintain the stack (for printing elements).

Function `stack_top` will return a pointer to the data content of the first element from the stack (the top of the stack). You will have to manually convert the return pointer to your working pointer, or to print int using a printData function, as follows:

```C
    void print_int(void *a) {
        if (a == NULL) return;

        int *fa = (int *)a;

        printf("%d\n", *fa);
    }

    int main() {

        scl_error_t err = SCL_OK;

        stack_t *stack = create_stack(NULL);

        for (int i = 0; i < 10; ++i)
            if ((err = stack_push(stack, &i, sizeof(i))) != SCL_OK) {
                scl_error_message(err);
            }

        int top = *(int *)stack_top(stack);
        printf("%d\n", top);

        // or using the print function

        print_int(stack_top(stack));

        free_stack(stack); // You may not check for errors but if you want go on
    }
```

>**NOTE:** If you are using a print function to print data than you also should check if returned pointer is not NULL. If stack is empty or it does not exists then stack_top will return a NULL pointer and it will break the program.

Function `stack_pop` will remove one element from the stack. Firstly function will free content of the data pointer if it is necessary (if freeData function is not NULL). The data content will be destroyed according to freeData function, than program will remove data pointer and node pointer and will update the new top of the stack.

Example of using stack_pop:

```C
    int main() {
        stack_t *stack = create_stack(NULL);

        for (int i = 0; i < 10; ++i)
            if (stack_push(stack, &i, sizeof(i)) != SCL_OK) {
                printf("Something went wrong inserting &i element\n", i);
                return EXIT_FAILURE;
            }

        for (int i = 0; i < 10; ++i)
            if (stack_pop(stack) != SCL_OK) {
                printf("Something went wrong removing %d element\n", i);
                return EXIT_FAILURE;
            }

        free_stack(stack); // In this case it will free just the stack structure
    }
```

## Other functions

Some functions that also are important for stack maintaining are:

```C
    uint8_t is_stack_empty(stack_t* stack);
    size_t get_stack_size(stack_t* stack);
```

First function will check if stack exists and if it is empty.

>**NOTE:** A non-existing stack is also considered as an empty stack.

The second function will return the size of the stack or **SIZE_MAX** if stack does not exist.

## For some other examples of using stacks you can look up at /examples/stack
