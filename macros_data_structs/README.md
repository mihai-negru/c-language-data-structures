# macro generic data structures

![Projec Logo](../images/logo.png)

[![GitHub issues](https://img.shields.io/github/issues/mihai-negru/c-language-data-structures?label=Issues)](https://github.com/mihai-negru/c-language-data-structures/issues)
[![GitHub forks](https://img.shields.io/github/forks/mihai-negru/c-language-data-structures?label=Forks)](https://github.com/mihai-negru/c-language-data-structures/network)
[![GitHub stars](https://img.shields.io/github/stars/mihai-negru/c-language-data-structures?label=Stars)](https://github.com/mihai-negru/c-language-data-structures/stargazers)
[![GitHub license](https://img.shields.io/github/license/mihai-negru/c-language-data-structures?label=License)](https://github.com/mihai-negru/c-language-data-structures/blob/master/LICENCE)

## **Table of Contents**

1. [`Getting Started`](#getting-started)
2. [`Using the library`](#using-the-library)
3. [`Running examples`](#running-examples)

## **Getting Started**

Project "c-language-data-structures" has come as an idea to supply C Language with some basic **generic** data structures, as follows:

|                       Content/documentation                   |             Header/Source Macro File            |
|                       :-------------                          |                       :---------:               |
| [AVL Tree](documentation/MAVL.md)                             |  [m_avl.h](src/m_avl.h)                         |
| [Binary Search Tree](documentation/MBST.md)                   |  [m_bst.h](src/m_bst.h)                         |
| Config File (Basic Utils for Error Handling)                  |  [m_config.h](src/m_config.h)                   |
| [Double Linked List](documentation/MDLIST.md)                 |  [m_dlist.h](src/m_dlist.h)                     |
| [Single Linked List](documentation/MLIST.md)                  |  [m_list.h](src/m_list.h)                       |
| [Priority Queue](documentation/MPQUEUE.md)                    |  [m_pqueue.h](src/m_pqueue.h)                   |
| [Queue](documentation/MQUEUE.md)                              |  [m_queue.h](src/m_queue.h)                     |
| [Red Black Tree](documentation/MRBK.md)                       |  [m_rbk.h](src/m_rbk.h)                         |
| [Stack](documentation/MSTACK.md)                              |  [m_stack.h](src/m_stack.h)                     |

Every single **data structure** from this project can be used in any scopes and with **different** data types, however you must follow a set of rules so you don't break the program.
Every set of rules for every data structure can be found in [documentation](documentation/) folder from current project.

I tried for every known object to keep their function definitions as in **C++** and also followed the standard naming of methods according to specific data structure.

**It is very important** not to use directly the members of the structure objects, however if you feel safe to access them by yourself there should be no problem.

### **Dependencies**

* Linux, MacOS or WSL System
* GCC Compiler

## **Using the library**

The only thing is to copy the *header* file of the desired data structure into your project into a specified directory,
then to add to a file by:

```c
  #include "/path/to/file/m_avl.h" // or any other m_name.h
```

As you may have observed all the methods return a **merr_t** type, which is used for error handling.

```c
  // For example
  M_OK - when the method executed successfully
  M_NULL_INPUT - when the input is null
  ...

  // In order to get a message for any error you have to call the macro

  merr_t err = function_from_library(of_some_input);

  if (err != M_OK) {
    MERROR(err); // Which will print to stderr the error and the meaning of it
  }
```

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
