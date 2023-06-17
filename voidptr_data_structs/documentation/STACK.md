# Documentation for stack object ([scl_stack.h](../src/include/scl_stack.h))

## How to create a stack and how to destroy it?

In the scl_stack.h you have two functions that will help you by creating a stack and destroying it.

```C
    sstack_t*    create_stack(free_func frd, size_t data_size);
    scl_error_t free_stack(sstack_t * const __restrict__ stack);
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
        sstack_t *stack = create_stack(&free_person, sizeof(person));
        free_stack(stack);

        return 0;
    }
```

>**NOTE:** If you want to print whole stack you should call print *print_stack* function that takes a valid pointer to a stack structure location and prints it from top to bottom.

>**NOTE:** All elements of the stack should be of the same type.'

## How to insert and how to remove elements from stack?

You have 3 function that will maintain a stack:

```C
    scl_error_t     stack_push  (sstack_t * const __restrict__ stack, const void * __restrict__ data);
    const void*     stack_top   (const sstack_t * const __restrict__ stack);
    scl_error_t     stack_pop   (sstack_t * const __restrict__ stack);
```

Function `stack_push` will insert one element into the stack. You should pass an allocated stack into push function, however if stack pointer is NULL than no operation will be executed.

Function `stack_top` will return a pointer to the data content of the first element from the stack (the top of the stack). You will have to manually convert the return pointer to your working pointer, or to print int using a print_data function, as follows:

```C
    void print_int(const void * const a) {
        if (a == NULL) return;

        const int * const fa = (const int * const)a;

        printf("%d\n", *fa);
    }

    int main() {

        scl_error_t err = SCL_OK;

        sstack_t *stack = create_stack(NULL, sizeof(int));

        for (int i = 0; i < 10; ++i)
            if ((err = stack_push(stack, toptr(i))) != SCL_OK) {
                scl_error_message(err);
            }

        int top = *(const int *)stack_top(stack);
        printf("%d\n", top);

        // or using the print function

        print_int(stack_top(stack));

        free_stack(stack); // You may not check for errors but if you want go on
    }
```

>**NOTE:** If you are using a print function to print data than you also should check if returned pointer is not NULL. If stack is empty or it does not exists then stack_top will return a NULL pointer and it will break the program.

Function `stack_pop` will remove one element from the stack. Firstly function will free content of the data pointer if it is necessary (if free_data function is not NULL). The data content will be destroyed according to free_data function, than program will remove data pointer and node pointer and will update the new top of the stack.

Example of using stack_pop:

```C
    int main() {
        sstack_t *stack = create_stack(NULL, sizeof(int));

        for (int i = 0; i < 10; ++i)
            if (stack_push(stack, toptr(i)) != SCL_OK) {
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
    uint8_t is_stack_empty(const sstack_t * const __restrict__ stack);
    size_t get_stack_size(const sstack_t * const __restrict__ stack);
```

First function will check if stack exists and if it is empty.

>**NOTE:** A non-existing stack is also considered as an empty stack.

The second function will return the size of the stack or **SIZE_MAX** if stack does not exist.

## For some other examples of using stacks you can look up at [examples](../examples/stack/)
