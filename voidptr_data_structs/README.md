![Projec Logo](../images/logo.png)

[![GitHub issues](https://img.shields.io/github/issues/Matrix22/C-language-Data-Structures?label=Issues)](https://github.com/Matrix22/C-language-Data-Structures/issues)
[![GitHub forks](https://img.shields.io/github/forks/Matrix22/C-language-Data-Structures?label=Forks)](https://github.com/Matrix22/C-language-Data-Structures/network)
[![GitHub stars](https://img.shields.io/github/stars/Matrix22/C-language-Data-Structures?label=Stars)](https://github.com/Matrix22/C-language-Data-Structures/stargazers)
[![GitHub license](https://img.shields.io/github/license/Matrix22/C-language-Data-Structures?label=License)](https://github.com/Matrix22/C-language-Data-Structures/blob/master/LICENCE)

# **Table of Contents**
1. [Getting Started](#start-description)
2. [Building the Project](#build-description)
3. [Using the library](#use-description)
4. [Running examples](#examples-description)
5. [Contributing](#contributing-description)

<a name="start-description"></a>

## **Getting Started**

Project "C Language Data Structures" has come as an idea to supply C Language with some basic **generic** data structures, as follows:

|                       Content/documentation                   |                       Header File                         |                           Source File                     |
|                       :-------------                          |                       :---------:                         |                           :---------:                     |
| [AVL Tree](documentation/AVL_TREE.md)                         |  [scl_avl_tree.h](src/include/scl_avl_tree.h)             |  [scl_avl_tree.c](src/scl_avl_tree.c)                     |
| [Binary Search Tree](documentation/BST_TREE.md)               |  [scl_bst_tree.h](src/include/scl_bst_tree.h)             |  [scl_bst_tree.c](src/scl_bst_tree.c)                     |
| Config File (Basic Utils for Error Handling)                  |  [scl_config.h](src/include/scl_config.h)                 |  [scl_config.c](src/scl_config.c)                         |
| [Double Linked List](documentation/DOUBLE_LINKED_LIST.md)     |  [scl_dlist.h](src/include/scl_dlist.h)                   |  [scl_dlist.c](src/scl_dlist.c)                           |
| [Function File](documentation/FUNCTION_TYPES.md)              |  [scl_func_types.h](src/include/scl_func_types.h)         |  [scl_func_types.c](src/scl_func_types.c)                 |
| [Graph](documentation/GRAPH.md)                               |  [scl_graph.h](src/include/scl_graph.h)                   |  [scl_graph.c](src/scl_graph.c)                           |
| [Hash Table](documentation/HASH_TABLE.md)                     |  [scl_hash_table.h](src/include/scl_hash_table.h)         |  [scl_hash_table.c](src/scl_hash_table.c)                 |
| [Single Linked List](documentation/SINGLE_LINKED_LIST.md)     |  [scl_list.h](src/include/scl_list.h)                     |  [scl_list.c](src/scl_list.c)                             |
| [Priority Queue](documentation/PRIORITY_QUEUE.md)             |  [scl_priority_queue.h](src/include/scl_priority_queue.h) |  [scl_priority_queue.c](src/scl_priority_queue.c)         |
| [Queue](documentation/QUEUE.md)                               |  [scl_queue.h](src/include/scl_queue.h)                   |  [scl_queue.c](src/scl_queue.c)                           |
| [Red Black Tree](documentation/RED_BLACK_TREE.md)             |  [scl_rbk_tree.h](src/include/scl_red_black_tree.h)             |  [scl_rbk_tree.c](src/scl_rbk_tree.c)                     |
| [Sorting Algorithms](documentation/SORT_ALGORITHMS.md)        |  [scl_sort_algo.h](src/include/scl_sort_algo.h)           |  [scl_sort_algo.c](src/scl_sort_algo.c)                   |
| [Stack](documentation/STACK.md)                               |  [scl_stack.h](src/include/scl_stack.h)                   |  [scl_stack.c](src/scl_stack.c)                           |

Every single **data structure** from this project can be used in any scopes and with **different** data types, however you must follow a set of rules so you don't break the program.
Every set of rules for every data structure can be found in [documentation](documentation/) folder from current project.

I tried for every known object to keep their function definitions as in **C++** and also followed the standard naming of methods according to specific data structure.

**It is very important** not to use directly the members of the structure objects, however if you feel safe to access them by yourself there should be no problem.

<a name="build-description"></a>

## **Building the Project**

In order to build and to install the project you must be sure that you are working on a **LINUX** environment or if you are on **WINDOWS** to use WSL(Windows Subsystem for Linux). Even if the building can pass on a windows system the executables generated will be incompatible.

The Project will create you two libraries: **one dynamically generated** and **one statically generated**. For static library there is no instalation process and you can play with the library ass you want (And link it statically to your future projects), however for the dynamic library the program should **register** the library into your system.

### **Dependencies**

* Linux or WSL System
* GCC Compiler
* ldconfig (For library register)

### **Building**

For this step make sure that you opened a linux terminal service and have root privileges, also check if you had changed directory to the current working project ("C-language-Data-Structures").

If you want to build the libraries and to install (register) the dynamic library into your system you shall execute:

```BASH
    cd build
    sudo make
```

After executing the following commands you will get prompt some messages showing you that building and installing proccess went successfully

```BASH
    Building dynamic scl_sort_algo ....................... PASSED
    Building dynamic scl_red_black_tree .................. PASSED
    Building dynamic scl_func_types ...................... PASSED
    Building dynamic scl_config .......................... PASSED
    Building dynamic scl_dlist ........................... PASSED
    Building dynamic scl_graph ........................... PASSED
    Building dynamic scl_bst_tree ........................ PASSED
    Building dynamic scl_list ............................ PASSED
    Building dynamic scl_queue ........................... PASSED
    Building dynamic scl_hash_table ...................... PASSED
    Building dynamic scl_priority_queue .................. PASSED
    Building dynamic scl_avl_tree ........................ PASSED
    Building dynamic scl_stack ........................... PASSED

    Building Dynamic Library ............................. PASSED

    Building static scl_sort_algo ........................ PASSED
    Building static scl_red_black_tree ................... PASSED
    Building static scl_func_types ....................... PASSED
    Building static scl_config ........................... PASSED
    Building static scl_dlist ............................ PASSED
    Building static scl_graph ............................ PASSED
    Building static scl_bst_tree ......................... PASSED
    Building static scl_list ............................. PASSED
    Building static scl_queue ............................ PASSED
    Building static scl_hash_table ....................... PASSED
    Building static scl_priority_queue ................... PASSED
    Building static scl_avl_tree ......................... PASSED
    Building static scl_stack ............................ PASSED

    Building Static Library .............................. PASSED

    Installing Header Files .............................. PASSED
    Installing Dynamic Library ........................... PASSED
    Loading Dynamic Library .............................. PASSED

    Finished Project ..................................... PASSED
```

If one of the above messages haven't showed, something went wrong so I encourage you to try ones again. (The files will be automatically deleted and the process will generate new object files)

If you just want to build the libraries and not to register the dynamic library into your system you shall run:

```BASH
    cd build
    make build
```

In **libs** directory will appear 2 files, representing the dynamic and static libraries.

After if you want to install the dynamic library by yourself you must run:

```BASH
    cd build
    sudo make install
```

After that some messages will be prompt to show that everything went alright.

**If something went wrong** and you want to rerun the building process you may run:

```BASH
    make cleanall # This command will delete libraries as well
```

After running the above commands, everything is set up and now you can use the library and its beauty!

### **Developing the Project**

If you are working on the project and you want to improve some functions or code logic than I recommend not to use **all** target from makefile.
In order not to compile every object file even if you haven't modified it you should run:

```BASH
    cd build
    make build # Repeat the command when you modify just one source file or header file
    sudo make install # For dynamic library instalation
```

The command **sudo make** will create the libraries and then will wipe out all the temporary files as (object files) and if you will want to recompile it will
compile everything from the beginning. The **sudo make** command is good then you want to rebuild the program from the first file even if it was not modified, however
if you work on changing some files you should not call **sudo make**.

When running **make build** two new folders will appear in the **build** folder:
* **dynamic_obj** - a folder containing all object files that compose the dynamic library
* **static_obj** - a folder containing all object files that compose the satic library

If a folder is deleted that the **make build** proccess will create just the folder that was deleted and if any source or header file was modified then the object file
will be recompiled and the library will be recreated.

<a name="use-description"></a>

## **Using the library**

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

## **Running examples**

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

## **Contributing**

I am very open for contributing, however if you want to take part of this project you will have to follow some set of rules:

* First you should respect the code styling, some references are [C Style](https://github.com/mcinglis/c-style)
* You should provide documentation (respecting doxygen standard) for every function or piece of code
* You should come with some tests or examples to show how to work with your new functions
* Be happy and don't stop coding :) 




