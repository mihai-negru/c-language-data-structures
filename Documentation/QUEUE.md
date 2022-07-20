# Documentation for queue object (queueUtils.h)

## How to create a queue and how to destroy it?

In the queueUtils.h you have two functions that will help you by creating a queue and destroying it.

```C
    TQueue* create_queue(void (*freeData)(void *));
    void free_queue(TQueue *queue);
```

First function will take a pointer to a function that will free the content of the actual data. However you can send NULL(0) instead of a pointer. For base types as **int**, **float**, **double**, **char**, **static arrays**, **static structures**, `freeData` function should be **NULL**, if a structure that contains a pointer (and it is allocated with malloc or calloc) then a freeData function (*not NULL*) should be provided.

Example of creating a queue:

```C
    typedef struct {
        char *name;
        int age;
    } person;

    void freePerson(void *a) {
        person *fa = (person *)a;

        free(fa->name);
    }

    int main() {
        TQueue *queue = create_queue(&freePerson);
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

    void printData(void *a) {
        if (a == NULL) return;

        int check = *(int *)a;

        if (check == 0) {
            Int *fb = (Int *)a;

            printf("%d ", fb->elem);
        } else if (check == 1) {
            Float *fb = (Float *)a;

            printf("%f ", fb->elem);
        } else if (check == 2) {
            Char *fb = (Char *)a;

            printf("%c ", fb->elem);
        } else {
            printf("Not known check var\n");
        }
    }
```

>**NOTE:** It is very **IMPORTANT** in this situation for the **check** variable to be the first member of the structure, otherwise it is posibble to access different memory locations which will fail into a Segmentation Fault.

>**NOTE:** Some types of structures as above and functions to print different classes of data types are described in **functionTypes.h/.md** section.

## How to insert and how to remove elements from queue?

You have 4 function that will maintain a queue:

```C
    int queue_push(TQueue *queue, void *data, size_t dataSize);
    void* queue_front(TQueue *queue);
    void* queue_back(TQueue *queue);
    int queue_pop(Tqueue *queue);
```

Function `queue_push` will insert one element into the queue. You should pass an allocated queue into push function, however if queue pointer is NULL than no operation will be executed. As we mentioned above you can insert different data types into the queue, but you will have to provide your functions to maintain the queue (for printing elements).

Function `queue_front(back` will return a pointer to the data content of the first(back) element from the queue. You will have to manually convert the return pointer to your working pointer, or to print int using a printData function, as follows:

```C
    void printInt(void *a) {
        if (a == NULL) return;

        int *fa = (int *)a;

        printf("%d\n", *fa);
    }

    int main() {
        TQueue *queue = create_queue(0);

        for (int i = 0; i < 10; ++i)
            queue_push(queue, &i, sizeof(int));

        int top = *(int *)queue_front(queue);
        // Or queue_back(queue);
        printf("%d\n", top);

        // or using the print function

        printInt(queue_top(queue));
    }
```

>**NOTE:** If you are using a print function to print data than you also should check if returned pointer is not NULL. If queue is empty or it does not exists then queue_front(back) will return a NULL pointer and it will break the program.

Function `queue_pop` will remove one element from the queue. Firstly function will free content of the data pointer if it is necessary (if freeData function is not NULL). The data content will be destroyed according to freeData function, than program will remove data pointer and node pointer and will update the new top of the queue.

Example of using queue_pop:

```C
    int main() {
        TQueue *queue = create_queue(0);

        for (int i = 0; i < 10; ++i)
            if (queue_push(queue, &i, sizeof(int))) {
                printf("Something went wrong inserting &i element\n", i);
                return EXIT_FAILURE;
            }

        for (int i = 0; i < 10; ++i)
            if (queue_pop(queue)) {
                printf("Something went wrong removing %d element\n", i);
                return EXIT_FAILURE;
            }

        free_queue(queue); // In this case it will free just the queue structure
    }
```

## Other functions 

Some functions that also are important for queue maintaining are:
```C
    int is_queue_empty(TQueue *queue);
    int get_queue_size(Tqueue *queue);
```

First function will check if queue exists and if it is empty.

>**NOTE:** A non-existing queue is also considered as an empty queue.

The second function will return the size of the queue or **-1** if queue does not exist.

## For some other examples of using queues you can look up at /examples/queues
