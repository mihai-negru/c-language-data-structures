![Projec Logo](images/logo.png)

# Table of Contents
1. [Getting Started](#start)
2. [Building the Project](#build)
3. [Running examples](#examples)
4. [Using the library](#use)
5. [Contributing](#contributing)

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

I tried for every known object to keep their function definitions as in **C++** and also followed the standart naming of methods according to specific data structure.

**It is very important** not to use directly the members of the structure objects, however if you will safe to access them by yourself there should be no problem.


