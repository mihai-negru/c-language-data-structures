# Documentation for queue object (scl_queue.h)

## How to create a queue and how to destroy it?

In the scl_queue.h you have two functions that will help you by creating a queue and destroying it.

```C
    queue_t* create_queue(void (*free_data)(void *));
    scl_error_t free_queue(queue_t* queue);
```

First function will take a pointer to a function that will free the content of the actual data. However you can send NULL(0) instead of a pointer. For base types as **int**, **float**, **double**, **char**, **static arrays**, **static structures**, `free_data` function should be **NULL**, if a structure that contains a pointer (and it is allocated with malloc or calloc) then a free_data function (*not NULL*) should be provided.

Example of creating a queue:

```C
    typedef struct {
        char *name;
        int age;
    } person;

    void free_person(void *a) {
        person *fa = a;

        free(fa->name);
    }

    int main() {
        queue_t* queue = create_queue(&free_person);
        free_queue(queue);

        return 0;
    }
```

>**NOTE:** If you want to print whole queue you should call print *print_queue* function that takes a valid pointer to a queue structure location and prints it from front to back.

>**NOTE:** All elements of the queue should be of the same type. However you can save different types of data in the same exact queue, but functions as print_queue will fail. One solution is: for a different type to make one structure that has its own print function and a variable to check types of the data, for example:

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

## How to insert and how to remove elements from queue?

You have 4 function that will maintain a queue:

```C
    scl_error_t queue_push(queue_t* queue, const void* data, size_t data_size);
    void* queue_front(queue_t* queue);
    void* queue_back(queue_t* queue);
    scl_error_t queue_pop(queue_t* queue);
```

Function `queue_push` will insert one element into the queue. You should pass an allocated queue into push function, however if queue pointer is NULL than no operation will be executed. As we mentioned above you can insert different data types into the queue, but you will have to provide your functions to maintain the queue (for printing elements).

Function `queue_front(back` will return a pointer to the data content of the first(back) element from the queue. You will have to manually convert the return pointer to your working pointer, or to print int using a printData function, as follows:

```C
    void print_int(const void *a) {
        if (a == NULL) return;

        const int *fa = a;

        printf("%d\n", *fa);
    }

    int main() {

        scl_error_t err = SCL_OK;

        queue_t* queue = create_queue(NULL);

        for (int i = 0; i < 10; ++i)
            if ((err = queue_push(queue, &i, sizeof(i))) != SCL_OK) {
                scl_error_message(err);
            }

        int top = *(int *)queue_front(queue);
        // Or queue_back(queue);
        printf("%d\n", top);

        // or using the print function

        print_int(queue_top(queue));

        free_queue(queue); // You may not check free function for error
    }
```

>**NOTE:** If you are using a print function to print data than you also should check if returned pointer is not NULL. If queue is empty or it does not exists then queue_front(back) will return a NULL pointer and it will break the program.

Function `queue_pop` will remove one element from the queue. Firstly function will free content of the data pointer if it is necessary (if freeData function is not NULL). The data content will be destroyed according to free_data function, than program will remove data pointer and node pointer and will update the new top of the queue.

Example of using queue_pop:

```C
    int main() {
        queue_t* queue = create_queue(NULL);

        for (int i = 0; i < 10; ++i)
            if (queue_push(queue, &i, sizeof(i)) != SCL_OK) {
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
    uint8_t is_queue_empty(queue_t* queue);
    size_t get_queue_size(queue_t* queue);
```

First function will check if queue exists and if it is empty.

>**NOTE:** A non-existing queue is also considered as an empty queue.

The second function will return the size of the queue or **SIZE_MAX** if queue does not exist.

## For some other examples of using queues you can look up at /examples/queue
