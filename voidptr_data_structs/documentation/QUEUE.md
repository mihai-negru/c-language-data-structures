# Documentation for queue object ([scl_queue.h](../src/include/scl_queue.h))

## How to create a queue and how to destroy it?

In the scl_queue.h you have two functions that will help you by creating a queue and destroying it.

```C
    queue_t*    create_queue(free_func frd, size_t data_size);
    scl_error_t free_queue(queue_t * const __restrict__ queue);
```

First function will take a pointer to a function that will free the content of the actual data. However you can send NULL(0) instead of a pointer. For base types as **int**, **float**, **double**, **char**, **static arrays**, **static structures**, `free_data` function should be **NULL**, if a structure that contains a pointer (and it is allocated with malloc or calloc) then a free_data function (*not NULL*) should be provided.

Example of creating a queue:

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
        queue_t *queue = create_queue(&free_person, sizeof(person));
        free_queue(queue);

        return 0;
    }
```

>**NOTE:** If you want to print whole queue you should call print *print_queue* function that takes a valid pointer to a queue structure location and prints it from top to bottom.

>**NOTE:** All elements of the queue should be of the same type.'

## How to insert and how to remove elements from queue?

You have 3 function that will maintain a queue:

```C
    scl_error_t     queue_push  (queue_t * const __restrict__ queue, const void * __restrict__ data);
    const void*     queue_top   (const queue_t * const __restrict__ queue);
    scl_error_t     queue_pop   (queue_t * const __restrict__ queue);
```

Function `queue_push` will insert one element into the queue. You should pass an allocated queue into push function, however if queue pointer is NULL than no operation will be executed.

Function `queue_top` will return a pointer to the data content of the first element from the queue (the top of the queue). You will have to manually convert the return pointer to your working pointer, or to print int using a print_data function, as follows:

```C
    void print_int(const void * const a) {
        if (a == NULL) return;

        const int * const fa = (const int * const)a;

        printf("%d\n", *fa);
    }

    int main() {

        scl_error_t err = SCL_OK;

        queue_t *queue = create_queue(NULL, sizeof(int));

        for (int i = 0; i < 10; ++i)
            if ((err = queue_push(queue, toptr(i))) != SCL_OK) {
                scl_error_message(err);
            }

        int top = *(const int *)queue_front(queue);
        printf("%d\n", top);

        // or using the print function

        print_int(queue_front(queue));

        free_queue(queue); // You may not check for errors but if you want go on
    }
```

>**NOTE:** If you are using a print function to print data than you also should check if returned pointer is not NULL. If queue is empty or it does not exists then queue_top will return a NULL pointer and it will break the program.

Function `queue_pop` will remove one element from the queue. Firstly function will free content of the data pointer if it is necessary (if free_data function is not NULL). The data content will be destroyed according to free_data function, than program will remove data pointer and node pointer and will update the new top of the queue.

Example of using queue_pop:

```C
    int main() {
        queue_t *queue = create_queue(NULL, sizeof(int));

        for (int i = 0; i < 10; ++i)
            if (queue_push(queue, toptr(i)) != SCL_OK) {
                printf("Something went wrong inserting &i element\n", i);
                return EXIT_FAILURE;
            }

        for (int i = 0; i < 10; ++i)
            if (queue_pop(queue) != SCL_OK) {
                printf("Something went wrong removing %d element\n", i);
                return EXIT_FAILURE;
            }

        free_queue(queue); // In this case it will free just the queue structure
    }
```

## Other functions

Some functions that also are important for queue maintaining are:

```C
    uint8_t is_queue_empty(const queue_t * const __restrict__ queue);
    size_t get_queue_size(const queue_t * const __restrict__ queue);
```

First function will check if queue exists and if it is empty.

>**NOTE:** A non-existing queue is also considered as an empty queue.

The second function will return the size of the queue or **SIZE_MAX** if queue does not exist.

## For some other examples of using queues you can look up at [examples](../examples/queue/)
