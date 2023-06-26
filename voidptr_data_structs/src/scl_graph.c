/**
 * @file scl_graph.c
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

#include "./include/scl_graph.h"
#include "./include/scl_queue.h"
#include "./include/scl_stack.h"
#include "./include/scl_priority_queue.h"

/**
 * @brief Create a new graph object. Function may fail if the
 * number of vertices is zero, or not enough heap memory is
 * left to allocate all vertices.
 * 
 * @param number_of_vertices initial number of vertices to form a graph
 * @return graph_t* a new allocated graph object or `NULL` if function fails
 */
graph_t* create_graph(size_t number_of_vertices) {
    /* Check if input data is valid */
    if (0 == number_of_vertices) {
        errno = EINVAL;
        perror("Number of vertexes of the graph is zero at creation");
        return NULL;
    }

    /* Allocate a new graph object */
    graph_t *new_graph = malloc(sizeof(*new_graph));

    /* Check if graph object was allocated successfully */
    if (NULL != new_graph) {

        /* Set default values of the graph object */
        new_graph->size = number_of_vertices;

        /* Allocate visit array of the current graph object */
        new_graph->visit = malloc(sizeof(*new_graph->visit) * number_of_vertices);

        /* Check if visit array was allocated */
        if (NULL != new_graph->visit) {

            /* Allocate vertices array of the graph object */
            new_graph->vertices = malloc(sizeof(*new_graph->vertices) * number_of_vertices);

            /* Check if vertices array was allocated */
            if (NULL != new_graph->vertices) {

                /* Set every vertex to its default value */
                for (size_t iter = 0; iter < new_graph->size; ++iter) {

                    /* Allocate a vertex node */
                    new_graph->vertices[iter] = malloc(sizeof(*new_graph->vertices[iter]));

                    /* Check if vertex was allocated successfully */
                    if (NULL != new_graph->vertices[iter]) {

                        /* Set default vertex values */
                        new_graph->vertices[iter]->link = NULL;
                        new_graph->vertices[iter]->in_deg = 0;
                        new_graph->vertices[iter]->out_deg = 0;
                    } else {

                        /* Allocation of one vertex failed so free all memory */
                        for (size_t del_iter = 0; del_iter <= iter; ++del_iter) {
                            free(new_graph->vertices[del_iter]);
                            new_graph->vertices[del_iter] = NULL;
                        }

                        free(new_graph->vertices);
                        new_graph->vertices = NULL;

                        free(new_graph->visit);
                        new_graph->visit = NULL;

                        free(new_graph);
                        new_graph = NULL;

                        errno = ENOMEM;
                        perror("Not enought memory to allocate graph links");
                    }
                }
            } else {

                /* Allocation of the vertices array failed */
                free(new_graph->visit);
                new_graph->visit = NULL;

                free(new_graph);
                new_graph = NULL;

                errno = ENOMEM;
                perror("Not enough memory to allocate vertices of the graph");
            }
        } else {

            /* Allocation of the visit array failed */
            free(new_graph);
            new_graph = NULL;

            errno = ENOMEM;
            perror("Not enough memory to allocate visit vector for graph");
        }
    } else {

        /* Allocation of the graph failed */
        errno = ENOMEM;
        perror("Not enough memory to allocate new graph object");
    }

    /* Return a new allocated graph object or `NULL` */
    return new_graph;
}

/**
 * @brief Subroutine function of free_graph, to 
 * free all edges of one vertex from heap memory.
 * No graph object is needed just pointer to 
 * linked list representing the edges.
 * 
 * @param link pointer to a linked list representing the edges array
 */
static void free_graph_link(graph_link_t *link) {
    /* Free memory occupied by edges of one vertex */
    while (NULL != link) {
        graph_link_t *delete_node = link;

        link = link->next;

        free(delete_node);
        delete_node = NULL;
    }
}

/**
 * @brief Function to free all memory allocated for a graph
 * object from heap memory. Function may fail if graph object
 * is `NULL`, however it will not break the program.
 * 
 * @param gr pointer to an allocated graph object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t free_graph(graph_t * const __restrict__ gr) {
    /* Check if graph pointer is valid */
    if (NULL != gr) {

        /* Check if vertices array is allocated */
        if (NULL != gr->vertices) {

            /* Free memory of all vertices */
           for (size_t iter = 0; iter < gr->size; ++iter) {
                if (NULL != gr->vertices[iter]) {

                    /* Free memory for one vertex */
                    free_graph_link(gr->vertices[iter]->link);

                    free(gr->vertices[iter]);
                    gr->vertices[iter] = NULL;
                }
            }

            /* Free vertices array */
            free(gr->vertices);
            gr->vertices = NULL;
        }

        /* Free visit array */
        if (NULL != gr->visit) {
            free(gr->visit);
            gr->visit = NULL;
        }

        /* Free graph object */
        free(gr);

        /* All good */
        return SCL_OK;
    }

    /* `NULL` graph sent to free */
    return SCL_NULL_GRAPH;
}

/**
 * @brief Create a graph edge node for linked list.
 * Function may fail if vertex value is `SIZE_MAX`
 * or edge length is `__LBDL_MAX__` or no heap memory is
 * left for memory allocation.
 * 
 * @param vertex number of the vertex to link the edge
 * @param edge_len the length of the current edge node
 * @return graph_link_t* a new allocated edge node object or `NULL`
 * if function fails
 */
static graph_link_t* create_graph_link(size_t vertex, long double edge_len) {
    /* Check if input data is valid */
    if ((SIZE_MAX == vertex) || (__LDBL_MAX__ == edge_len)) {
        errno = EINVAL;
        perror("Data input for link creation is invalid");
        return NULL;
    }

    /* Allocate a new edge node */
    graph_link_t *new_link = malloc(sizeof(*new_link));

    /* Check if edge was allocated */
    if (NULL != new_link) {

        /* Set edge object data */
        new_link->vertex = vertex;
        new_link->edge_len = edge_len;
        new_link->next = NULL;
    } else {

        /* Edge allocation failed */
        errno = ENOMEM;
        perror("Not enough memory to allocate graph vertex");
    }

    /* Return a new allocated edge(link) or `NULL` */
    return new_link;
}

/**
 * @brief Function to insert one edge into the graph object or to link
 * two vertices with one edge. Function may fail if input graph object is
 * or input data is not valid. 
 * 
 * @param gr pointer to an allocated graph object
 * @param start_vertex number of vertex that edge starts from
 * @param end_vertex number of vertex that edge ends to
 * @param edge_len the length of the edge that links two vertices
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_insert_edge(const graph_t * const __restrict__ gr, size_t start_vertex, size_t end_vertex, long double edge_len) {
    /* Check if graph object is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is valid */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if two vertices are in the current graph object */
    if ((gr->size <= start_vertex) || (gr->size <= end_vertex)) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    /* Check if selected vertices are allocated */
    if ((NULL == gr->vertices[start_vertex]) || (NULL == gr->vertices[end_vertex])) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    /* Check if edge length is valid */
    if (__LDBL_MAX__ == edge_len) {
        return SCL_INVALID_EDGE_LENGTH;
    }

    /* Create a new link between two vertices */
    graph_link_t *new_vertex = create_graph_link(end_vertex, edge_len);

    /* Check if allocation went successfully */
    if (NULL == new_vertex) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    /* Link edge to start_vertex linked list */
    new_vertex->next = gr->vertices[start_vertex]->link;
    gr->vertices[start_vertex]->link = new_vertex;

    /* Update in and out degree of the vertices */
    ++(gr->vertices[start_vertex]->out_deg);
    ++(gr->vertices[end_vertex]->in_deg);

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to add more vertices in the current graph object.
 * Function may fail if no heap memory is for the new vertices.
 * 
 * @param gr a pointer to an allocated graph object
 * @param new_vertices number of vertices to add in graph object
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_insert_vertices(graph_t * const __restrict__ gr, size_t new_vertices) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if new_vertices will add nodes on graph */
    if (0 == new_vertices) {
        return SCL_GRAPH_INVALID_NEW_VERTICES;
    }

    /* Try to realloc visit array of the graph */
    uint8_t *try_realloc_visit = realloc(gr->visit, sizeof(*gr->visit) * (gr->size + new_vertices));

    if (NULL == try_realloc_visit) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    gr->visit = try_realloc_visit;

    /* Try to realloc vertices array of the graph */
    graph_vertex_t **try_realloc = realloc(gr->vertices, sizeof(*try_realloc) * (gr->size + new_vertices));

    if (NULL == try_realloc) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    /* Allocate the new created vertices and set default values */
    for (size_t iter = gr->size; iter < gr->size + new_vertices; ++iter) {
        try_realloc[iter] = malloc(sizeof(*try_realloc[iter]));

        if (NULL != try_realloc[iter]) {
            try_realloc[iter]->link = NULL;
            try_realloc[iter]->in_deg = 0;
            try_realloc[iter]->out_deg = 0;
        } else {
            for (size_t del_iter = gr->size; del_iter <= iter; ++del_iter) {
                free(try_realloc[del_iter]);
                try_realloc[del_iter] = NULL;
            }

            return SCL_REALLOC_GRAPH_VERTICES_FAIL;
        }
    }

    /* If adding new vertices went successfully update size */
    gr->size += new_vertices;

    gr->vertices = try_realloc;

    /* All good */
    return SCL_OK;
}

/**
 * @brief Create the transpose graph of the selected graph. The object
 * graph sent as input has to be a valid object. Function may fail if no memory
 * is left on heap to allcoate the new transposed graph.
 * 
 * @param gr a pointer to an allocated graph object
 * @return graph_t* allocated transposed graph of selected object or `NULL`
 */
graph_t* create_transpose_graph(const graph_t * const __restrict__ gr) {
    /* Check if the graph object is valid */
    if ((NULL == gr) || (0 == gr->size) || (NULL == gr->vertices)) {
        errno = EINVAL;
        perror("Graph sent as input in transpose function is not valid");

        return NULL;
    }

    /* Create a new graph */
    graph_t *transpose_gr = create_graph(gr->size);

    /* Check if transposed graph was created */
    if (NULL == transpose_gr) {
        return NULL;
    }

    scl_error_t err = SCL_OK;

    /* Invert every edges from original graph into transposed graph */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (NULL == gr->vertices[iter]) {
            return NULL;
        }

        graph_link_t *link = gr->vertices[iter]->link;

        while (NULL != link) {
            err = graph_insert_edge(transpose_gr, link->vertex, iter, link->edge_len);

            if (SCL_OK != err) {
                return NULL;
            }

            link = link->next;
        }
    }

    /* Return an allocated transposed graph */
    return transpose_gr;
}

/**
 * @brief Function to print the vertices of the graph and their edges with
 * other vertices. Function may fail if one vertex os not allcoated or graph
 * object is not valid.
 * 
 * @param gr a pointer to an allocated graph object.
 * @param data_arr array of words to print the mapping of the vertices can be `NULL`
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_print(const graph_t * const __restrict__ gr, const uint8_t ** const data_arr) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Print vertices of the graph and their edges */
    if (0 == gr->size) {
        printf("[ ]\n");
    } else {
        for (size_t iter = 0; iter < gr->size; ++iter) {
            if (NULL == gr->vertices[iter]) {
                return SCL_NULL_GRAPH_VERTEX;
            }

            if (NULL != data_arr) {
                printf("(%s) ", data_arr[iter]);
            } else {
                printf("(%ld) ", iter);
            }

            graph_link_t *link = gr->vertices[iter]->link;

            while (NULL != link) {
                if (NULL != data_arr) {
                    printf("-> [%s] ", data_arr[link->vertex]);
                } else {
                    printf("-> (%ld) ", link->vertex);
                }

                link = link->next;
            }

            printf(" -> (~)\n");
        }
    }
    
    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to delete one edge that links two vertices.
 * Function may fail if the graph is not valid or selected
 * edge is not in the current graph.
 * 
 * @param gr a pointer to an allocated graph object
 * @param first_vertex first vertex that edge starts from
 * @param second_vertex second vertex that edge end to
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_delete_edge(const graph_t * const __restrict__ gr, size_t first_vertex, size_t second_vertex) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if start vertex is in the current graph */
    if (first_vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    /* Check if start vertex is allocated */
    if (NULL == gr->vertices[first_vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    graph_link_t *delete_link = gr->vertices[first_vertex]->link;
    graph_link_t *parent_delete_link = NULL;

    /* Find selected edge from graph */
    while ((NULL != delete_link) & (second_vertex != delete_link->vertex)) {
        parent_delete_link = delete_link;
        delete_link = delete_link->next;
    }

    /* Edge was not found in the graph */
    if (NULL == delete_link) {
        return SCL_EDGE_NOT_FOUND;
    }

    /* Update the linked list */
    if (NULL == parent_delete_link) {
        gr->vertices[first_vertex]->link = delete_link->next;
    } else {
        parent_delete_link->next = delete_link->next;
    }

    /* Delete edge from memory */
    delete_link->next = NULL;
    free(delete_link);
    delete_link = NULL;

    /* Update the out degree of the start vertex */
    --(gr->vertices[first_vertex]->out_deg);

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to delete all edges that link the same two nodes.
 * This function is a bigger brother of the graph_delete_edge, however
 * an oriented graph can have more than two edges that link the same
 * two vertices so the function will delete all these edges.
 * 
 * @param gr a pointer to an allocated graph object
 * @param first_vertex first vertex that edge starts from
 * @param second_vertex second vertex that edge end to
 * @return scl_error_t enum object for handling errors 
 */
scl_error_t graph_delete_all_edges(const graph_t * const __restrict__ gr, size_t first_vertex, size_t second_vertex) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if start vertex is in the current graph */
    if (first_vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    /* Check if start vertex is allocated */
    if (NULL == gr->vertices[first_vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    graph_link_t *delete_link = gr->vertices[first_vertex]->link;
    graph_link_t *parent_delete_link = NULL;

    /* Find every occurence of the selected edge and remove them from graph */
    while (NULL != delete_link) {
        if (second_vertex == delete_link->vertex) {
            if (NULL == parent_delete_link) {
                gr->vertices[first_vertex]->link = delete_link->next;

                delete_link->next = NULL;
                free(delete_link);
                
                delete_link = gr->vertices[first_vertex]->link;
            } else {
                parent_delete_link->next = delete_link->next;

                delete_link->next = NULL;
                free(delete_link);

                delete_link = parent_delete_link->next;
            }

            --(gr->vertices[first_vertex]->out_deg);
        } else {
            parent_delete_link = delete_link;
            delete_link = delete_link->next;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to delete a selected vertex from the current
 * graph object. Vertices greater than selected vertex will 
 * deacrease their number by one so vertex `3` maps into the
 * vertex `2` and vertex `2` becomes `1` if vertex 1 was deleted.
 * When vertex is deleted all of its in and out edges will be removed
 * from the current working graph object.
 * 
 * @param gr a pointer to an allocated graph object
 * @param vertex selected vertex number to delet from graph
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_delete_vertex(graph_t * const __restrict__ gr, size_t vertex) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if selected vertex is from current graph */
    if (vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    /* Check if selected vertex is allocated and can be removed */
    if (NULL == gr->vertices[vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    scl_error_t err = SCL_OK;

    /* Delete IN edges from the graph */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (iter != vertex) {
            err = graph_delete_all_edges(gr, iter, vertex);

            if (SCL_OK != err) {
                return err;
            }
        }
    }

    /* Delete OUT edges of the selected vertex */
    free_graph_link(gr->vertices[vertex]->link);
    gr->vertices[vertex]->in_deg = gr->vertices[vertex]->out_deg = 0;

    /* Free memory allocated for vertex */
    free(gr->vertices[vertex]);
    gr->vertices[vertex] = NULL;

    --(gr->size);

    /* Try to realloc memory to reduce memory space */
    for (size_t iter = vertex; iter < gr->size; ++iter) {
        gr->vertices[iter] = gr->vertices[iter + 1];
    }

    uint8_t *try_realloc_visit = realloc(gr->visit, sizeof(*gr->visit) * gr->size);

    if (NULL == try_realloc_visit) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    gr->visit = try_realloc_visit;

    graph_vertex_t **try_realloc = realloc(gr->vertices, sizeof(*try_realloc) * gr->size);

    if (NULL == try_realloc) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    gr->vertices = try_realloc;

    /* If reallocation went successfully update the vertices number */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (NULL == gr->vertices[iter]) {
            return SCL_NULL_GRAPH_VERTEX;
        }

        graph_link_t *link = gr->vertices[iter]->link;

        while (NULL != link) {
            if (link->vertex > vertex) {
                --(link->vertex);
            }

            link = link->next;
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Function to get the number of vertices from the
 * selected graph. If pointer to graph object is NULL than
 * SIZE_MAX will be returned
 * 
 * @param gr a pointer to an allocated graph object
 * @return size_t SIZE_MAX or the actual size of the graph
 */
size_t get_graph_size(const graph_t * const __restrict__ gr) {
    if (NULL == gr) {
        return SIZE_MAX;
    }

    return gr->size;
}

/**
 * @brief Function to traverse the vertices of the graph object by
 * breath-first-search method. Function will get as input an allocated
 * array to save the path of bfs, if the array is `NULL` then the path
 * will not be saved however function will return the exact size of the
 * traversed vertices in this bfs call.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex a vertex to start the bfs
 * @param vertex_path an array to save the path of bfs traversal can be `NULL`
 * @return size_t size of the traversed vertices or 0 if function failed
 */
size_t graph_bfs_traverse(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path) {
    /* Check if input data is valids */
    if ((NULL == gr) || (NULL == gr->vertices) || (start_vertex >= gr->size)) {
        return 0;
    }

    /* No vertex was traversed */
    size_t traversed_vex = 0;

    /* Set all vertices as unvisited */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    /* Create the bfs traversal queue */
    queue_t *bfs_queue = create_queue(NULL, sizeof(*vertex_path));

    if (NULL != bfs_queue) {
        
        /* Start the breath-first-search from the start vertex */

        gr->visit[start_vertex] = 1;

        if (SCL_OK != queue_push(bfs_queue, &start_vertex)) {
            free_queue(bfs_queue);
            return 0;
        }

        while (!is_queue_empty(bfs_queue)) {
            const size_t *front_vertex = queue_front(bfs_queue);

            
            if (NULL != vertex_path) {
                
                /* Insted of adding in path variable you can perform an action */
                vertex_path[traversed_vex] = *front_vertex; 
            }

            /* Increase traversed vertices size */
            ++traversed_vex;

            if (SCL_OK != queue_pop(bfs_queue)) {
                free_queue(bfs_queue);
                return 0;
            }

            graph_link_t *link = gr->vertices[*front_vertex]->link;

            while (NULL != link) {
                if (0 == gr->visit[link->vertex]) {
                    gr->visit[link->vertex] = 1;

                    if (SCL_OK != queue_push(bfs_queue, &link->vertex)) {
                        free_queue(bfs_queue);
                        return 0;
                    }
                }

                link = link->next;
            }
        }

        free_queue(bfs_queue);
    }

    /* Return the traversed vertices size */
    return traversed_vex;
}

/**
 * @brief Subroutine function for graph_dfs_traversal to 
 * calculate recursively the depth-first-search vertices.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex number of vertex to start the dfs traversal
 * @param vertex_path an array to save dfs path can be `NULL`
 * @param traversed_vex size of the traversed vertices in the dfs call
 */
static void graph_dfs_traverse_helper(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path, size_t * __restrict__ traversed_vex) {
    /* Visit the current vertex */
    gr->visit[start_vertex] = 1;

    /* Insert vertex into the path */
    if (NULL != vertex_path) {
       vertex_path[*traversed_vex] = start_vertex; 
    }
    
    /* Increase traversed size */
    ++(*traversed_vex);

    graph_link_t *link = gr->vertices[start_vertex]->link;

    /* Visit the neighbours vertices */
    while (NULL != link) {
        if (0 == gr->visit[link->vertex]) {
            graph_dfs_traverse_helper(gr, link->vertex, vertex_path, traversed_vex);
        }

        link = link->next;
    }
}

/**
 * @brief Function to traverse the vertices of the graph object by
 * depth-first-search method. Function will get as input an allocated
 * array to save the path of dfs, if the array is `NULL` then the path
 * will not be saved however function will return the exact size of the
 * traversed vertices in this dfs call.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex a vertex to start the dfs
 * @param vertex_path an array to save the path of dfs traversal can be `NULL`
 * @return size_t size of the traversed vertices or 0 if function failed 
 */
size_t graph_dfs_traverse(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path) {
    /* Check if input data is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (start_vertex >= gr->size)) {
        return 0;
    }

    /* No vertex was traversed */
    size_t traversed_vex = 0;

    /* Set every vertex as unvisited */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    /* Call helper function the real dfs call */
    graph_dfs_traverse_helper(gr, start_vertex, vertex_path, &traversed_vex);

    /* Return the size of the traversed vertices */
    return traversed_vex;
}

/**
 * @brief Subroutine function of the graph_has_cycle to perform
 * a special dfs traversal of the graph and to find if graph has or
 * not a cycle.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex first vertex to check if it forms a cycle
 * @param current_vertex current working vertex of the dfs traversal
 * @return uint8_t 1 if graph has a cycle starting from start_vertex or 0 otherwise
 */
static uint8_t graph_has_cycle_helper(const graph_t * const __restrict__ gr, size_t start_vertex, size_t current_vertex) {
    /* All vertices were visited and start vertex is the last visited vertex */
    if ((1 == gr->visit[start_vertex]) && (start_vertex == current_vertex)) {
        return 1;
    }

    /* All vertices were visited and no cycle was found */
    if (1 == gr->visit[current_vertex]) {
        return 0;
    }

    graph_link_t *link = gr->vertices[current_vertex]->link;

    int has_cycle = 0;

    /* Perform the dfs call */
    while ((NULL != link) && (0 == has_cycle)) {

        has_cycle = graph_has_cycle_helper(gr, start_vertex, link->vertex);

        link = link->next;
    }

    /* Return if graph has or not a cycle */
    return has_cycle;
}

/**
 * @brief Function to check if a graph object has or has not a cycle.
 * Function may fail if graph object is not valid in that case false will
 * be returned because an invalid graph has no cycle.
 * 
 * @param gr a pointer to an allocated graph object
 * @return uint8_t if graph has a cycle or 0 otherwise
 */
uint8_t graph_has_cycle(const graph_t * const __restrict__ gr) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->visit) || (NULL == gr->vertices)) {
        return 0;
    }

    uint8_t has_cycle = 0;

    /* Traverse every vertex by dfs method and check if it has a cycle */
    for (size_t iter = 0; (iter < gr->size) && (0 == has_cycle); ++iter) {
        
        /* Reset the visit array */
        for (size_t make_zero = 0; make_zero < gr->size; ++make_zero) {
            gr->visit[make_zero] = 0;
        }
        
        /* Check if graph has a cycle */
        has_cycle = graph_has_cycle_helper(gr, iter, iter);
    }

    /* 1 if graph has a cycle or 0 otherwise */
    return has_cycle;
}

/**
 * @brief Function to get all vertices that are resulting
 * from the start_vertex in other words there exists a path
 * from node X to node Y, in this case node Y will be part of
 * past nodes.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex a vertex to get its past vertices
 * @param vertex_path an allocated array to save all past vertices
 * @return size_t size of the past vertices or 0 if function failed 
 */
size_t graph_vertex_past_vertices(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (start_vertex >= gr->size) || (NULL == vertex_path)) {
        return 0;
    }

    size_t traversed_vex = 0;

    /* Reset the visit array */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    /* Create the bfs queue to find past nodes */
    queue_t *bfs_queue = create_queue(NULL, sizeof(*vertex_path));

    if (NULL != bfs_queue) {
        gr->visit[start_vertex] = 1;

        if (SCL_OK != queue_push(bfs_queue, &start_vertex)) {
            free_queue(bfs_queue);
            return 0;
        }

        /* Start the bfs traversal */
        while (!is_queue_empty(bfs_queue)) {
            const size_t *front_vertex = queue_front(bfs_queue);

            /* Add node to past nodes */
            if (start_vertex != *front_vertex) {
                vertex_path[traversed_vex++] = *front_vertex;
            }

            if (SCL_OK != queue_pop(bfs_queue)) {
                free_queue(bfs_queue);
                return 0;
            }

            graph_link_t *link = gr->vertices[*front_vertex]->link;

            while (NULL != link) {
                if (0 == gr->visit[link->vertex]) {
                    gr->visit[link->vertex] = 1;

                    if (SCL_OK != queue_push(bfs_queue, &link->vertex)) {
                        free_queue(bfs_queue);
                        return 0;
                    }
                }

                link = link->next;
            }
        }

        free_queue(bfs_queue);
    }

    /* Return number of the past vertices */
    return traversed_vex;
}

/**
 * @brief Function to get the future vertices of the selected vertex.
 * Future vertices are the vertices that have a path ending in the selected node.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex a vertex to get its future vertices
 * @param vertex_path an allocated array to save all future vertices
 * @return size_t size of the future vertices or 0 if function failed 
 */
size_t graph_vertex_future_vertices(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path) {
    /* Check if the graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (start_vertex >= gr->size) || (NULL == vertex_path)) {
        return 0;
    }

    /* Compute the transposed graph */
    graph_t *transpose_graph = create_transpose_graph(gr);

    /*
     * Calculate the past vertices of the transposed graph
     * that will be the future vertices of the selected graph
     */
    size_t traversed_vex = graph_vertex_past_vertices(transpose_graph, start_vertex, vertex_path);

    /* Free memory of the transposed graph */
    free_graph(transpose_graph);

    /* Return the size of the future vertices */
    return traversed_vex;
}

/**
 * @brief Function to get the anticone vertices of the selected vertex.
 * Anticone vertices are the vertices that do not have any path to selected
 * vertex.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex a vertex to get its anticone vertices
 * @param vertex_path an allocated array to save all anticone vertices
 * @return size_t size of the anticone vertices or 0 if function failed 
 */
size_t graph_vertex_anticone_vertices(const graph_t * const __restrict__ gr, size_t start_vertex, size_t * __restrict__ vertex_path) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (start_vertex >= gr->size) || (NULL == vertex_path)) {
        return 0;
    }

    /* Allocate the past vertices array */
    size_t * __restrict__ past_vertices = malloc(sizeof(*past_vertices) * gr->size);

    if (NULL == past_vertices) {
        return 0;
    }

    /* Allocate the future vertices array */
    size_t * __restrict__ future_vertices = malloc(sizeof(*future_vertices) * gr->size);

    if (NULL == future_vertices) {
        free(past_vertices);
        return 0;
    }

    /* Compute the past and future vertices of the selected vertex */
    size_t past_size = graph_vertex_past_vertices(gr, start_vertex, past_vertices);
    size_t future_size = graph_vertex_future_vertices(gr, start_vertex, future_vertices);
    size_t traversed_size = 0;

    /*
     * Iterate through all vertices and if current vertex
     * is not in the past or future vertices than the vertex
     * is part of the anticone vertices
     */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        uint8_t node_is_anticone = 1;

        for (size_t iter_i = 0; (iter_i < past_size) && (1 == node_is_anticone); ++iter_i) {
            if (iter == past_vertices[iter_i]) {
                node_is_anticone = 0;
            }
        }

        for (size_t iter_i = 0; (iter_i < future_size) && (1 == node_is_anticone); ++iter_i) {
            if (iter == future_vertices[iter_i]) {
                node_is_anticone = 0;
            }
        }

        if ((1 == node_is_anticone) && (start_vertex != iter)) {
            vertex_path[traversed_size++] = iter;
        }
    }

    /* Return the size of the anticone vertices */
    return traversed_size;
}

/**
 * @brief Function to get the tips vertices of the graph.
 * The tips vertices are the vertices that have the in-degree
 * zero, but can have out-degree as bigger as they want
 * 
 * @param gr a pointer to an allocated graph object
 * @param vertex_path an allocated array to save all tips vertices
 * @return size_t size of the tips vertices or 0 if function failed 
 */
size_t graph_tips_vertices(const graph_t * const __restrict__ gr, size_t * __restrict__ vertex_path) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (NULL == vertex_path)) {
        return 0;
    }

    size_t traversed_size = 0;

    /* Iterate through all vertices and check their in degree */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (NULL != gr->vertices[iter]) {
            if (0 == gr->vertices[iter]->in_deg) {
                vertex_path[traversed_size++] = iter;
            }
        }
    }

    /* Return the number of the tips vertices */
    return traversed_size;
}

/**
 * @brief Subroutine function to push on stack dfs traversed
 * vertices to perform a topological sort of the graph
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex vertex to perform a dfs traversa;
 * @param stack pointer to a stack object to push vertices
 */
static void graph_topological_sort_helper(const graph_t * const __restrict__ gr, size_t start_vertex, sstack_t * const __restrict__ stack) {
    /* Check if selected vertex is allocated */
    if (NULL == gr->vertices[start_vertex]) {
        return;
    }

    /* Visit the current selected vertex */
    gr->visit[start_vertex] = 1;

    graph_link_t * link = gr->vertices[start_vertex]->link;

    /* Perform a dfs call on start_index */
    for(; link != NULL; link = link->next) {
        if (0 == gr->visit[link->vertex]) {
            graph_topological_sort_helper(gr, link->vertex, stack);
        }
    }

    /* Push the selected vertex after dfs call */
    stack_push(stack, &start_vertex);
}

/**
 * @brief Function to get the topological sort of the
 * selected graph. Function may fail if graph object is invalid.
 * Or no heap memory is left for stack memory allocation
 * 
 * @param gr a pointer to an allocated graph object
 * @param vertex_path an allocated array to save the path of topological sort
 * @return size_t the number of the traversed vertices by topological sort
 */
size_t graph_topological_sort(const graph_t * const __restrict__ gr, size_t * __restrict__ vertex_path) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (NULL == gr->visit) || (NULL == vertex_path)) {
        return 0;
    }

    /* Create a stack subroutine for topological sort */
    sstack_t *sort_stack = create_stack(NULL, sizeof(*vertex_path));

    if (NULL == sort_stack) {
        return 0;
    }

    /* Reset the visit array */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    /* Perform the topological sort dfs call */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (0 == gr->visit[iter]) {
            graph_topological_sort_helper(gr, iter, sort_stack);
        }
    }

    size_t traversed_size = 0;
    scl_error_t err = SCL_OK;

    /* The topological sort of the graph is the reverse of the dfs calls */
    while (!is_stack_empty(sort_stack)) {
        const size_t *top_vertex = stack_top(sort_stack);

        vertex_path[traversed_size++] = *top_vertex;

        err = stack_pop(sort_stack);

        if (SCL_OK != err) {
            free_stack(sort_stack);
            return traversed_size;
        }
    }

    free_stack(sort_stack);

    /*
     * Return the number of traversed vertices,
     * should be gr->size if everything went good
     */
    return traversed_size;
}

/**
 * @brief Compare function to create a min priority queue.
 * 
 * @param elem1 constant pointer to first element to compare
 * @param elem2 constant pointer to second element to compare
 * @return int32_t 1 if elem1 < elem2, -1 if elem1 > elem2, 0 if elem1 == elem2
 */
static int32_t min_heap_cmp_func(const void * const elem1, const void * const elem2) {
    if ((NULL != elem1) && (NULL != elem2)) {
        const size_t * const f_elem1 = elem1;
        const size_t * const f_elem2 = elem2;

        if (*f_elem1 > *f_elem2) {
            return -1;
        } else if (*f_elem1 < *f_elem2) {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
} 

/**
 * @brief Function to compute the minimum distances starting
 * from selected vertex to all other graph vertices.
 * If the selected vertex has no path to another vertex the
 * distance will be __LDBL_MAX__. The distances will be computed
 * by dijkstra method, so graphs with negative cycles will fail.
 * The parent path array can be `NULL`.
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex vertex to compute the distances beginning from it
 * @param vertex_dists an allocated array with all distances beginning from start_vertex
 * @param vertex_parents an array with vertices showing the minimum path from
 * start_vertex to any other vertex
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_dijkstra(const graph_t * const __restrict__ gr, size_t start_vertex, long double * __restrict__ vertex_dists, size_t * __restrict__ vertex_parents) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if distances array is allocated */
    if (NULL == vertex_dists) {
        return SCL_NULL_VERTICES_DISTANCES;
    }

    /* Allocate a vertices array to heapify the min heap */
    size_t *vertices = malloc(sizeof(*vertices) * gr->size);

    if (NULL == vertices) {
        return SCL_NOT_ENOUGHT_MEM_FOR_OBJ;
    }

    if (NULL != vertex_parents) {
        for (size_t iter = 0; iter < gr->size; ++iter) {
            vertex_parents[iter] = SIZE_MAX;
        }

        vertex_parents[start_vertex] = -1;
    }

    /* Set default values */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        vertices[iter] = iter;
        vertex_dists[iter] = __LDBL_MAX__;
    }

    vertex_dists[start_vertex] = 0;

    /* Create the min heap priority queue */
    priority_queue_t *min_heap = create_priority_queue(gr->size, &min_heap_cmp_func, &min_heap_cmp_func, NULL, NULL, sizeof(*vertex_dists), sizeof(*vertices));

    if (NULL == min_heap) {
        free(vertices);

        return SCL_NULL_PRIORITY_QUEUE;
    }

    /* Heapify the vertices and the default distances */
    scl_error_t err = heapify(min_heap, vertices, vertex_dists);
    free(vertices);

    if (SCL_OK != err) {
        free_priority_queue(min_heap);

        return err;
    }

    /* Perform the dijkstra's algorithm */
    while (!is_priq_empty(min_heap)) {

        /* Get the vertex with the minimum distance */
        size_t min_dist_vertex = *(size_t *)pri_queue_top(min_heap);
        
        err = pri_queue_pop(min_heap);

        if (SCL_OK != err) {
            free_priority_queue(min_heap);

            return err;
        }

        if (NULL == gr->vertices[min_dist_vertex]) {
            free_priority_queue(min_heap);

            return SCL_NULL_GRAPH_VERTEX;
        }

        graph_link_t *link = gr->vertices[min_dist_vertex]->link;

        /* Iterate through edges of the current minimum vertex */
        for (; link != NULL; link = link->next) {
            size_t vertex_heap_index = pri_find_data_index(min_heap, &link->vertex);

            /* Update destination vertex distance if any improvement can be done */
            if ((SIZE_MAX != vertex_heap_index) &&
                (vertex_dists[min_dist_vertex] != __LDBL_MAX__) &&
                (link->edge_len + vertex_dists[min_dist_vertex] < vertex_dists[link->vertex])) {
                if (NULL != vertex_parents) {
                    vertex_parents[link->vertex] = min_dist_vertex;
                }

                vertex_dists[link->vertex] = vertex_dists[min_dist_vertex] + link->edge_len;

                err = change_node_priority(min_heap, pri_find_data_index(min_heap, &link->vertex), &vertex_dists[link->vertex]);

                if (SCL_OK != err) {
                    free_priority_queue(min_heap);

                    return err;
                }
            }
        }
    }

    /* Free priority queue and return SCL_OK */
    return free_priority_queue(min_heap);
}

/**
 * @brief Function to compute the minimum cost spanning tree
 * of the selected graph where the root of the spanning tree
 * is the start_index vertex. 
 * 
 * @param gr a pointer to an allocated graph object
 * @param start_vertex vertex to start to compute the spanning tree
 * @param vertex_dists an allocated array with all distances beginning from start_vertex
 * @param vertex_parents an allocated array with vertices showing the minimum cost spanning tree
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_prim(const graph_t * const __restrict__ gr, size_t start_index, long double * __restrict__ vertex_dists, size_t * __restrict__ vertex_parents) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if distances array is allocated */
    if (NULL == vertex_dists) {
        return SCL_NULL_VERTICES_DISTANCES;
    }

    /* Check if parents array is allocated */
    if (NULL == vertex_parents) {
        return SCL_NULL_VERTICES_PARENTS;
    }

    /* Allocate a vertices array to heapify the min heap */
    size_t *vertices = malloc(sizeof(*vertices) * gr->size);

    if (NULL == vertices) {
        return SCL_NOT_ENOUGHT_MEM_FOR_OBJ;
    }

    /* Set default values */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        vertices[iter] = iter;
        vertex_dists[iter] = __LDBL_MAX__;
        vertex_parents[iter] = SIZE_MAX;
    }

    vertex_parents[start_index] = -1;
    vertex_dists[start_index] = 0;

    /* Create the min heap priority queue */
    priority_queue_t *min_heap = create_priority_queue(gr->size, &min_heap_cmp_func, &min_heap_cmp_func, NULL, NULL, sizeof(*vertex_dists), sizeof(*vertices));

    if (NULL == min_heap) {
        free(vertices);

        return SCL_NULL_PRIORITY_QUEUE;
    }

    /* Heapify the vertices and the default distances */
    scl_error_t err = heapify(min_heap, vertices, vertex_dists);
    free(vertices);

    if (SCL_OK != err) {
        free_priority_queue(min_heap);

        return err;
    }

    /* Perform the prim's algorithm */
    while (!is_priq_empty(min_heap)) {

        /* Get the vertex with the minimum distance */
        size_t min_dist_vertex = *(size_t *)pri_queue_top(min_heap);
        
        err = pri_queue_pop(min_heap);

        if (SCL_OK != err) {
            free_priority_queue(min_heap);

            return err;
        }

        if (NULL == gr->vertices[min_dist_vertex]) {
            free_priority_queue(min_heap);

            return SCL_NULL_GRAPH_VERTEX;
        }

        graph_link_t *link = gr->vertices[min_dist_vertex]->link;

        /* Iterate through edges of the current minimum vertex */
        for (; link != NULL; link = link->next) {
            size_t vertex_heap_index = pri_find_data_index(min_heap, &link->vertex);

            /* Update destination vertex distance if any improvement can be done */
            if ((SIZE_MAX != vertex_heap_index) &&
                (vertex_dists[min_dist_vertex] != __LDBL_MAX__) &&
                (link->edge_len < vertex_dists[link->vertex])) {

                vertex_parents[link->vertex] = min_dist_vertex;
                vertex_dists[link->vertex] = link->edge_len;

                err = change_node_priority(min_heap, pri_find_data_index(min_heap, &link->vertex), &vertex_dists[link->vertex]);

                if (SCL_OK != err) {
                    free_priority_queue(min_heap);

                    return err;
                }
            }
        }
    }

    /* Free priority queue and return SCL_OK */
    return free_priority_queue(min_heap);
}

/**
 * @brief Function to calculate a matrix of minimum distances between
 * any two vertices if no path exists between two vertices than
 * __LDBL_MAX__ will be calculated.
 * 
 * @param gr a pointer to an allocated graph object
 * @param vertices_dists an allocated matrix of gr->size X gr->size dimension
 * @return scl_error_t enum object for handling errors
 */
scl_error_t graph_floyd_warshall(const graph_t * const __restrict__ gr, long double ** __restrict__ vertices_dists) {
    /* Check if graph pointer is valid */
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    /* Check if vertices array is allocated */
    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    /* Check if vertices distances matrix is allocated */
    if (NULL == vertices_dists) {
        return SCL_NULL_PATH_MATRIX;
    }

    /* Set default values of the distances matrix */
    for (size_t iter_i = 0; iter_i < gr->size; ++iter_i) {
        for (size_t iter_j = 0; iter_j < gr->size; ++iter_j) {
            vertices_dists[iter_i][iter_j] = __LDBL_MAX__;
        }
    }

    /* Set the initial edge length between two vertices */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (NULL == gr->vertices[iter]) {
            return SCL_NULL_GRAPH_VERTEX;
        }

        graph_link_t *link = gr->vertices[iter]->link;

        for (; link != NULL; link = link->next) {
            vertices_dists[iter][link->vertex] = link->edge_len;
        }
    }

    /* Calculate the minimum distances between any two nodes */
    for (size_t iter_k = 0; iter_k < gr->size; ++iter_k) {
        for (size_t iter_i = 0; iter_i < gr->size; ++iter_i) {
            for (size_t iter_j = 0; iter_j < gr->size; ++iter_j) {
                if (vertices_dists[iter_i][iter_k] + vertices_dists[iter_k][iter_j] < vertices_dists[iter_i][iter_j]) {
                    vertices_dists[iter_i][iter_j] = vertices_dists[iter_i][iter_k] + vertices_dists[iter_k][iter_j];
                }
            }
        }
    }

    /* All good */
    return SCL_OK;
}

/**
 * @brief Check if the oriented graph object is strongly connected.
 * 
 * @param gr a pointer to an allocated graph object
 * @return uint8_t 1 if graph is strongly connected or 0 otherwise
 */
uint8_t graph_is_strongly_connected(const graph_t * const __restrict__ gr) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->visit) || (NULL == gr->vertices)) {
        return 0;
    }

    /* Check the size of the dfs traversal */
    size_t dfs_size = graph_dfs_traverse(gr, 0, NULL);

    /* If all vertices were not traversed that graph is not sc */
    if (dfs_size != gr->size) {
        return 0;
    }

    /* Compute the transposed graph */
    graph_t *transpose_gr = create_transpose_graph(gr);

    /* Check if transposed graph is valid */
    if ((NULL == transpose_gr) || (NULL == transpose_gr->visit) || (NULL == transpose_gr->vertices)) {
        return 0;
    }

    /* Check the size of the dfs traversal of the travsposed graph */
    dfs_size = graph_dfs_traverse(transpose_gr, 0, NULL);

    free_graph(transpose_gr);

    /* If all vertices were not traversed in the transposed graph than it is not sc*/
    if (dfs_size != gr->size) {
        return 0;
    }

    /* The graph object is strongly connected */
    return 1;
}

/**
 * @brief Function to calculate all the strongly connected components from the graph.
 * Function will return a matrix of vertices that will calculate every strongly connected
 * components. The number_of_scc will show the number of strongly connected components.
 * In the matrix the first cell from every row will be the number of the vertices from a
 * strongly connected component.
 * 
 * @param gr a pointer to an allocated graph object
 * @param number_of_scc pointer to location of the number of strongly connected components
 * @return size_t** matrix containing on every row the vertices from a strongly connected
 * component or NULL if function fails
 */
size_t** graph_strongly_connected_components(const graph_t * const __restrict__ gr, size_t *number_of_scc) {
    /* Check if graph object is valid */
    if ((NULL == gr) || (NULL == gr->vertices) || (NULL == gr->visit) || (NULL == number_of_scc)) {
        return NULL;
    }

    /* Create the subroutine stack for strongly connected components */
    sstack_t *scc_stack = create_stack(NULL, sizeof(size_t));

    if (NULL == scc_stack) {
        return NULL;
    }

    /* Reset the visit array of the graph */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    /* Compute the topological sort stack of the graph */
    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (0 == gr->visit[iter]) {
            graph_topological_sort_helper(gr, iter, scc_stack);
        }
    }

    /* Create the transposed graph of the graph object */
    graph_t *transpose_gr = create_transpose_graph(gr);

    if ((NULL == transpose_gr) || (NULL == transpose_gr->visit)) {
        free_stack(scc_stack);

        return NULL;
    }

    /* Reset the visit array of the transposed graph */
    for (size_t iter = 0; iter < transpose_gr->size; ++iter) {
        transpose_gr->visit[iter] = 0;
    }

    *number_of_scc = 0;
    size_t **scc_paths = NULL;

    /* Compute the strongly connected components */
    while (!is_stack_empty(scc_stack)) {

        /* Get the top vertex from stack subroutine */
        size_t top_vertex = *(size_t *)stack_top(scc_stack);

        stack_pop(scc_stack);

        /* Check if current selected vertex is not of another strongly connected component */
        if (0 == transpose_gr->visit[top_vertex]) {
            if (0 == *number_of_scc) {

                /* Allocate first slot for strongly connected component */
                scc_paths = malloc(sizeof(*scc_paths));

                if (NULL == scc_paths) {
                    free_stack(scc_stack);
                    free_graph(transpose_gr);

                    return NULL;
                }
            } else {

                /* Add one more slot for strongly connected component */
                size_t **try_realloc = realloc(scc_paths, sizeof(*try_realloc) * (*number_of_scc + 1));

                if (NULL == try_realloc) {
                    free_stack(scc_stack);
                    free_graph(transpose_gr);

                    return scc_paths;
                }

                scc_paths = try_realloc;
            }

            /* Allocate the array for strongly connected component */
            scc_paths[*number_of_scc] = malloc(sizeof(*scc_paths[*number_of_scc]) * (gr->size + 1));

            if (NULL != scc_paths[*number_of_scc]) {

                /* Traverse the graph starting from top_vertex node to find the strongly connected components */
                scc_paths[*number_of_scc][0] = 0;

                graph_dfs_traverse_helper(transpose_gr, top_vertex, scc_paths[*number_of_scc] + 1, &scc_paths[*number_of_scc][0]);

                /* Increase the number of scc */
                ++(*number_of_scc);
            } else {

                /* Could not allocate the memory for strongly connected component */
                free_stack(scc_stack);
                free_graph(transpose_gr);

                return scc_paths;
            }
        }
    }

    /* No strongly connected component */
    if ((0 == *number_of_scc) && (NULL != scc_paths)) {
        free(scc_paths);
        scc_paths = NULL;
    }

    free_graph(transpose_gr);
    free_stack(scc_stack);

    /* Return the matrix of the strongly connected components */
    return scc_paths;
}
