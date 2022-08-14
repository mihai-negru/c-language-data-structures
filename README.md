![Projec Logo](images/logo.png)

[![GitHub issues](https://img.shields.io/github/issues/Matrix22/C-language-Data-Structures?label=Issues)](https://github.com/Matrix22/C-language-Data-Structures/issues)
[![GitHub forks](https://img.shields.io/github/forks/Matrix22/C-language-Data-Structures?label=Forks)](https://github.com/Matrix22/C-language-Data-Structures/network)
[![GitHub stars](https://img.shields.io/github/stars/Matrix22/C-language-Data-Structures?label=Stars)](https://github.com/Matrix22/C-language-Data-Structures/stargazers)
[![GitHub license](https://img.shields.io/github/license/Matrix22/C-language-Data-Structures?label=License)](https://github.com/Matrix22/C-language-Data-Structures/blob/master/LICENCE)

# Table of Contents
1. [Getting Started](#start-description)
2. [Building the Project](#build-description)
3. [Using the library](#use-description)
4. [Running examples](#examples-description)
5. [Contributing](#contributing-description)

<a name="start-description"></a>
## Getting Started

Project "C Language Data Structures" has come as an idea to supply C Language with some basic **generic** data structures, as follows:

|                       Content/Documentation                   |                       Header File                         |                           Source File                     |
|                       :-------------                          |                       :---------:                         |                           :---------:                     |
| [AVL Tree](Documentation/AVL_TREE.md)                         |  [scl_avl_tree.h](src/include/scl_avl_tree.h)             |  [scl_avl_tree.c](src/scl_avl_tree.c)                     |
| [Binary Search Tree](Documentation/BST_TREE.md)               |  [scl_bst_tree.h](src/include/scl_bst_tree.h)             |  [scl_bst_tree.c](src/scl_bst_tree.c)                     |
| Config File (Basic Utils for Error Handling)                  |  [scl_config.h](src/include/scl_config.h)                 |  [scl_config.c](src/scl_config.c)                         |
| [Double Linked List](Documentation/DOUBLE_LINKED_LIST.md)     |  [scl_dlist.h](src/include/scl_dlist.h)                   |  [scl_dlist.c](src/scl_dlist.c)                           |
| [Function File](Documentation/FUNCTION_TYPES.md)              |  [scl_func_types.h](src/include/scl_func_types.h)         |  [scl_func_types.c](src/scl_func_types.c)                 |
| [Graph](Documentation/GRAPH.md)                               |  [scl_graph.h](src/include/scl_graph.h)                   |  [scl_graph.c](src/scl_graph.c)                           |
| [Hash Table](Documentation/HASH_TABLE.md)                     |  [scl_hash_table.h](src/include/scl_hash_table.h)         |  [scl_hash_table.c](src/scl_hash_table.c)                 |
| [Single Linked List](Documentation/SINGLE_LINKED_LIST.md)     |  [scl_list.h](src/include/scl_list.h)                     |  [scl_list.c](src/scl_list.c)                             |
| [Priority Queue](Documentation/PRIORITY_QUEUE.md)             |  [scl_priority_queue.h](src/include/scl_priority_queue.h) |  [scl_priority_queue.c](src/scl_priority_queue.c)         |
| [Queue](Documentation/QUEUE.md)                               |  [scl_queue.h](src/include/scl_queue.h)                   |  [scl_queue.c](src/scl_queue.c)                           |
| [Red Black Tree](Documentation/RED_BLACK_TREE.md)             |  [scl_rbk_tree.h](src/include/scl_rbk_tree.h)             |  [scl_rbk_tree.c](src/scl_rbk_tree.c)                     |
| [Sorting Algorithms](Documentation/SORT_ALGORITHMS.md)        |  [scl_sort_algo.h](src/include/scl_sort_algo.h)           |  [scl_sort_algo.c](src/scl_sort_algo.c)                   |
| [Stack](Documentation/STACK.md)                               |  [scl_stack.h](src/include/scl_stack.h)                   |  [scl_stack.c](src/scl_stack.c)                           |

Every single **data structure** from this project can be used in any scopes and with **different** data types, however you must follow a set of rules so you don't break the program.
Every set of rules for every data structure can be found in [Documentation](Documentation/) folder from current project.

I tried for every known object to keep their function definitions as in **C++** and also followed the standard naming of methods according to specific data structure.

**It is very important** not to use directly the members of the structure objects, however if you feel safe to access them by yourself there should be no problem.

<a name="build-description"></a>
## Building the Project

In order to build and to install the project you must be sure that you are working on a **LINUX** environment or if you are on **WINDOWS** to use WSL(Windows Subsystem for Linux). Even if the building can pass on a windows system the executables generated will be incompatible.

The Project will create you two libraries: **one dynamically generated** and **one statically generated**. For static library there is no instalation process and you can play with the library ass you want (And link it statically to your future projects), however for the dynamic library the program should **register** the library into your system.

### Dependencies

* Linux or WSL System
* GCC Compiler
* ldconfig (For library register)

### Building

For this step make sure that you opened a linux terminal service and have root privileges, also check if you had changed directory to the current working project ("C-language-Data-Structures").

If you want to build the libraries and to install (register) the dynamic library into your system you shall execute:

```BASH
    cd build
    sudo make
```

After executing the following commands you will get prompt some messages showing you that building and installing proccess went successfully

```BASH
    <------ Building Dynamic Library Went Successfully ------->
    <------ Building Static Library Went Successfully ------->
        <------ Building Project Went Successfully ------->

        <------ Installing Project Went Successfully ------->
```

If one of the above messages haven't showed, something went wrong so I encourage you to try ones again.

If you just want to build the libraries and not to register the dynami clibrary into your system you shall run:

```BASH
    cd build
    sudo make build
```

In **build** directory will appear 2 files, representing the dynamic and static libraries.

**If something went wrong** and you want to rerun the building process you may run:

```BASH
    make clean # This command will delete libraries as well
```

After running the above commands, everything is set up and now you can use the library and its beauty!

<a name="use-description"></a>
## Using the library

If you installed either the dynamic library or the static library into your system then in your source file you shall include:

```C
    #include <scl_datastruct.h> // To include all data structures
    #include <scl_avl_tree.h> // For AVL Tree and so on
```

And when you compile your program you shall link it as follows for dynamic library:

```BASH
    gcc -c your_file.c
    gcc your_file.o -ldstruc -o your_file
```

For static library, first copy the library into **your** current working folder:

```BASH
    gcc -c your_file.c
    gcc your_file.o -L. -ldstruc -o your_file
```

If the building process didn't pass you will not be able to use the **libraries**.

<a name="examples-description"></a>
## Running examples

Some data structures have some **examples** for you to undestand what you should and what you should not. Also the examples
will show you some **untraditional** ways to solve some things. In every example folder for different data structure exists one
source file that will be a benchmark to test the power of data structures on your system. The benchmarks can run a little slow
depending on your system.

For examples how to build and how to run the examples you must read the documentation related to the examples section.
The link to documentation **[HERE](examples/README.md)**.

Make sure after you are done with one data structure exaxmple to run:

```BASH
    make clean
```

<a name="contributing-description"></a>

## Contributing

I am very open for contributing, however if you want to take part of this project you will have to follow some set of rules:

* First you should respect the code styling, some references are [C Style](https://github.com/mcinglis/c-style)
* You should provide documentation (respecting doxygen standard) for every function or piece of code
* You should come with some tests or examples to show how to work with your new functions
* Be happy and don't stop coding :) 




