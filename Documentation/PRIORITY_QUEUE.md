# Documentation for queue object (scl_priority_queue.h)

## How to create a priority queue and how to destroy it?

In the scl_priority_queue.h you have three functions that will help you by creating a queue and destroying it.

```C
    priority_queue_t* create_priority_queue(
        size_t init_capacity,
        int (*compare_data)(const void*, const void*),
        int (*compare_priority)(const void*, const void*),
        void (*free_data)(void*),
        void (*free_priority)(void*)
    );

    void free_priority_queue(
        priority_queue_t* pqueue
    );

    priority_queue_t* heapify(
        const void* data,
        const void* priority,
        size_t data_size,
        size_t pri_size,
        size_t number_of_data,
        int (*compare_data)(const void*, const void*),
        int (*compare_priority)(const void*, const void*), 
        void (*free_data)(void*),
        void (*free_priority)(void*)
    );
```

`create_priority_queue` and `heapify` will both allocate memory on heap and create a priority queue and will return a pointer to it's location on heap memory. Priority queue is implemented with **HEAPS**. First function `create_priority_queue` will create an empty priority queue, while `heapify` will take an array of the same data and priority type and will make a priority queue using input data (in C++ the heapify method is equal to **emplace** from priority_queue template).

Example of creating and deleting a priority queue:

```C
    #include <scl_datastruc.h>

    int main(void) {
        priority_queue_t* pq = create_priority_queue(
            20, &compare_int, &compare_int, NULL, NULL);

        // pq = [];

        /* Using heapify method */

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        priority_queue* pq = heapify(
            arr, arr, sizeof(int), sizeof(int),
            sizeof(arr)/ sizeof(arr[0]), &compare_int, &compare_int,
            NULL, NULL);
    }
```