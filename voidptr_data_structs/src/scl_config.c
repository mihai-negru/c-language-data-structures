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
    
    case SCL_DATA_NOT_FOUND:
        printf("Specified value in the function has no equal member into the selected data structure\n");
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

    case SCL_NOT_EMPTY_PRIORITY_QUEUE:
        printf("Priority queue sent in the heapify function is not empty\n");
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

    case SCL_NULL_ACTION_FUNC:
        printf("Action function(action) is NULL inside called function\n");
        break;

    case SCL_NOT_ENOUGHT_MEM_FOR_NODE:
        printf("Creating a node of any type failed to load heap memory\n");
        break;

    case SCL_NOT_ENOUGHT_MEM_FOR_OBJ:
        printf("Some arrays or data structures subroutines were not allocated successfully\n");
        break;

    case SCL_DELETE_FROM_EMPTY_OBJECT:
        printf("Trying to delete a node object from an empty data structure\n");
        break;
    
    case SCL_INDEX_OVERFLOWS_SIZE:
        printf("Specified index (node_index) is greater than size of the actual data structure or the size is SIZE_MAX\n");
        break;

    case SCL_NULL_SIMPLE_ARRAY:
        printf("Input simple array (not a data structure) is NULL where it should not be\n");
        break;

    case SCL_NUMBER_OF_ELEMS_ZERO:
        printf("Number of elements of the array is zero, so no action needed\n");
        break;

    case SCL_SIMPLE_ELEM_ARRAY_SIZE_ZERO:
        printf("The size of one array element is declared as ZERO, which is wrong\n");
        break;

    case SCL_SIMPLE_ARRAY_COMPAR_FUNC_NULL:
        printf("The compare function to compare the elements of a simple array is NULL\n");
        break;

    case SCL_NULL_HASH_TABLE:
        printf("Hash Table pointer sent into the function is NULL\n");
        break;

    case SCL_UNKNOWN_HASH_NODE_COLOR:
        printf("Color of one hash table key-data node is changed from its default values, check your program\n");
        break;

    case SCL_INVALID_KEY:
        printf("Pointer to key data type specified at input is NULL\n");
        break;

    case SCL_NULL_HASH_ROOTS:
        printf("The buckets of the hash table object are not allocated\n");
        break;

    case SCL_NULL_HASH_FUNCTION:
        printf("The hash function of the hash table is not set\n");
        break;

    case SCL_REHASHING_FAILED:
        printf("Not enought memory to reallocate the buckets to perform rehashing of table\n");
        break;

    case SCL_NULL_GRAPH:
        printf("Graph pointer sent into the function is NULL\n");
        break;

    case SCL_INVALID_EDGE_LENGTH:
        printf("When inserting a new edge the length of the edge is maximum of long double\n");
        break;

    case SCL_NULL_GRAPH_VERTICES:
        printf("The vertices array of the current graph is not allocated on heap\n");
        break;

    case SCL_NULL_GRAPH_VERTEX:
        printf("The vertex struct is not allocated on heap so no action can be performed\n");
        break;

    case SCL_REALLOC_GRAPH_VERTICES_FAIL:
        printf("Reallocation of the vertices array failed and could not resize the array\n");
        break;

    case SCL_VERTEX_OUT_OF_BOUND:
        printf("The input number of vertex is greater than the current graph size\n");
        break;

    case SCL_EDGE_NOT_FOUND:
        printf("No edge was found to delete from a selected vertex edge array list\n");
        break;

    case SCL_NULL_VERTICES_DISTANCES:
        printf("The array provided at input to compute the distances is NULL\n");
        break;

    case SCL_NULL_VERTICES_PARENTS:
        printf("The array provided at input to compute the spanning tree is NULL\n");
        break;

    case SCL_NULL_PATH_MATRIX:
        printf("Matrix provideed at input in Floyd Warshall function is NULL\n");
        break;

    case SCL_GRAPH_INVALID_NEW_VERTICES:
        printf("The number to add to the selected graph is zero, so no action needed\n");
        break;

    default:
        printf("Unknown error check again\n");
    }
}
