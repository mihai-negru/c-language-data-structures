# What is scl function types series?

## Prototypes of printing and comparing same data types, also some special macros that will make your code lookmore beautiful.

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

## Three macros in the file

1. toptr -> this macro will take a variabile and will calculate its address to pass into special function from this current working project

2. ltoptr -> will take a type and a lvalue and will calculate an address having the same value as specified value

3. make_pair -> this macro does not do something special, it is usually used in data structures that have {key data} members, so this looks more beautiful and explains better what happens with the created pair.