# What is scl function types series?

## Prototypes of new data types for using in data structures from current project.

```C
    typedef struct {
        char pass;
        long int val;
    } long_int_t;
    long_int_t long_int_def(void);

    /////

    typedef struct {
        char pass;
        unsigned int val;
    } uint_t;
    uint_t uint_def(void);
```

>**NOTE:** You must use these types just when you want different types in one data structures, for eample in a queue or stack.

Example of correct use of predefined data types from scl_func_types.h

```C
    int main() {
        queue_t *queue = create_queue(NULL);

        int_t var1 = int_def();
        uint_t var2 = uint_def();
        char_t var3 = char_def();

        var1.val = 5;
        var2.val = 76;
        var3.val = 'A';

        queue_push(queue, &var1, sizeof(var1));
        queue_push(queue, &var2, sizeof(var2));
        queue_push(queue, &var2, sizeof(var3));

        print_queue(queue, &print_all_data);

        free_queue(queue);
    }

    // Output:
        5 76 A
```

>**NOTE:** `print_all_data` is a predefined function that prints the data based on **pass** variable value.

>**NOTE:** Also you have different function to print data types for example:

* print_int
* print_short_int
* print_double
* print_long_double
* ......

>**NOTE:** functiontypes Series contains also function to compare two data of the same type for example:

* compare_int
* compare_short_int
* compare_double
* ...

Example:

```C
    int main() {
        int a[] = {7, -1, 4, 1, 3, 9, 8, 4, -1};

        qsort(a, 9, sizeof(int), &compare_int);
    }
```

## For more examples on functionTypes series check /examples/stack and /examples/queue