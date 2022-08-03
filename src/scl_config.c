/**
 * @file scl_config.c
 * @author Mihai Negru (determinant289@gmail.com)
 * @version 1.0.0
 * @date 2022-06-21
 * 
 * @copyright Copyright (C) 2022-2023 Mihai Negru <determinant289@gmail.com>
 * This file is part of C-language-Data-Structures.
 *
 * C-language-Data-Structures is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C-language-Data-Structures is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with C-language-Data-Structures.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "./include/scl_config.h"

/**
 * @brief Function to show more information about
 * error resulting from a function call.
 * 
 * @param error_message member of scl_error_t to show
 * verbose message of the current error
 */
void scl_error_message(scl_error_t error_message) {
    switch (error_message) {
    case SCL_OK:
        printf("No error thorwn\n");
        break;

    case SCL_INVALID_INPUT:
        printf("Some params sent in function are NULL or undefined\n");
        break;

    case SCL_INVALID_DATA:
        printf("Data pointer sent in the function is pointing to NULL\n");
        break;

    case SCL_CANNOT_SWAP_DATA:
        printf("One or both data pointers of the nodes are pointing to NULL\n");
        break;

    case SCL_DATA_SIZE_ZERO:
        printf("Size of the data specified in param list is zero\n");
        break;

    case SCL_SWAP_SAME_DATA:
        printf("Data pointers of the nodes are pointing to the same memory location\n");
        break;

    case SCL_CANNOT_CHANGE_DATA:
        printf("Data pointers of the nodes or nodes are pointing to NULL\n");
        break;

    case SCL_UNDEFINED_DATA:
        printf("Data pointer of an allocated node is pointing to NULL\n");
        break;

    case SCL_DATA_NOT_FOUND_FOR_DELETE:
        printf("No node data was found in the working object pointing to the same location as data pointer\n");
        break;

    case SCL_NULL_LIST:
        printf("Sending as input a NULL linked list object\n");
        break;

    case SCL_NULL_DLIST:
        printf("Sending as input a NULL double linked listobject\n");
        break;

    case SCL_NULL_STACK:
        printf("Sending as input a NULL stack object\n");
        break;

    case SCL_NULL_QUEUE:
        printf("Sending as input a NULL queue object\n");
        break;

    case SCL_NULL_PRIORITY_QUEUE:
        printf("Sending as input a NULL priority queue object\n");
        break;

    case SCL_NULL_PQUEUE_NODES:
        printf("The array of nodes of a priority queue object is pointing to NULL\n");
        break;

    case SCL_PRIORITY_SIZE_ZERO:
        printf("The size of priority queue is zero and trying to access a node\n");
        break;

    case SCL_CHANGE_PRIORITY_TO_NULL:
        printf("Trying to change priority of a NULL priority pointer\n");
        break;

    case SCL_REALLOC_PQNODES_FAIL:
        printf("Reallocation of the pririty queue nodes array failed\n");
        break;

    case SCL_PQUEUE_CAPACITY_ZERO:
        printf("The capacity of priority queue is zero\n");
        break;

    case SCL_UNDEFINED_PRIORITY:
        printf("Trying to work with a NULL priority pointer\n");
        break;

    case SCL_INVALID_PRIORITY:
        printf("Priority pointer sent in the function is pointing to NULL\n");
        break;

    case SCL_NULL_COMPARE_PRIORITY_FUNC:
        printf("Function to compare two sets of priority data is undefined\n");
        break;

    case SCL_NULL_BST:
        printf("Sending as input a NULL binary search tree object\n");
        break;

    case SCL_NULL_RBK:
        printf("Sending as input a NULL red-black tree object\n");
        break;

    case SCL_UNKNOWN_RBK_COLOR:
        printf("User program changed the color of a node from black or red to something else, check your program\n");
        break;

    case SCL_NULL_AVL:
        printf("Sending as input a NULL avl tree object\n");
        break;

    case SCL_FIXING_NULL_TREE_NODE:
        printf("Trying to call a fixing function for trees on a NULL pointing node\n");
        break;

    case SCL_MAP_EMPTY_OBJECT:
        printf("Mapping function is undefined or NULL\n");
        break;

    case SCL_NULL_ACTION_FUNC:
        printf("Action function(action) is NULL inside called function\n");
        break;

    case SCL_NOT_ENOUGHT_MEM_FOR_NODE:
        printf("Creating a node of any type failed to load heap memory\n");
        break;

    case SCL_DELETE_FROM_EMPTY_OBJECT:
        printf("Trying to delete a node object from an empty data structure\n");
        break;
    
    case SCL_INDEX_OVERFLOWS_SIZE:
        printf("Specified index (node_index) is greater than size of the actual data structure or the size is SIZE_MAX\n");
        break;
    
    default:
        printf("Unknown error check again what are you sending n");
    }
}