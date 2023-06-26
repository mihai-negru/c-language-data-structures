# How to run the examples of a specific data structure ?

First change the directory to the specific data structure, for example:

```BASH
    cd mavl
```

In every data structure example directory you willl find one **Makefile** and some source files labeled as **example1.c** and so on.

If you want just to execute one example, for example the first one you should type in your terminal:

```BASH
    make e1
```

If you want to execute all examples you shall type in your terminal:

```BASH
    make
```

>**NOTE:** I recommend you to execute all of examples at once.
>**NOTE:** After every successfully executed example a message will be prompt in the terminal to inform you that execution went successfully

After compiling and running the examples some files will be generated in current working directory, such as:

* `out_**X**.txt` - where **X** is the number of the example, all the output generated by the example number **X** will be redirected in that file.

* `valgrind_log_**X**.txt` - where **X** is the number of the example, the valgrind output log information will be redirected in that file.

If you want to remove all the **txt** files and to clear all the directory from junk files you shall run:

```BASH
    make clean
```

>**NOTE:** Same actions you can do in any example data structure directory from **/examples/** directory
>**NOTE:** Some examples are benchmarks, usually example number **3**, so do not worry if the running time is a bit to much.