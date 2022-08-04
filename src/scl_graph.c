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

graph_t* create_graph(size_t number_of_vertexes) {
    if (0 == number_of_vertexes) {
        errno = EINVAL;
        perror("Number of vertexes of the graph is zero at creation");
        return NULL;
    }

    graph_t *new_graph = malloc(sizeof(*new_graph));

    if (NULL != new_graph) {
        new_graph->size = number_of_vertexes;

        new_graph->visit = malloc(sizeof(*new_graph->visit) * number_of_vertexes);

        if (NULL != new_graph->visit) {
            new_graph->vertices = malloc(sizeof(*new_graph->vertices) * number_of_vertexes);

            if (NULL != new_graph->vertices) {
                for (size_t iter = 0; iter < new_graph->size; ++iter) {
                    new_graph->vertices[iter] = malloc(sizeof(*new_graph->vertices[iter]));

                    if (NULL != new_graph->vertices[iter]) {
                        new_graph->vertices[iter]->link = NULL;
                        new_graph->vertices[iter]->in_deg = 0;
                        new_graph->vertices[iter]->out_deg = 0;
                    } else {
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
                free(new_graph->visit);
                new_graph->visit = NULL;

                free(new_graph);
                new_graph = NULL;

                errno = ENOMEM;
                perror("Not enough memory to allocate vertices of the graph");
            }
        } else {
            free(new_graph);
            new_graph = NULL;

            errno = ENOMEM;
            perror("Not enough memory to allocate visit vector for graph");
        }
    } else {
        errno = ENOMEM;
        perror("Not enough memory to allocate new graph object");
    }

    return new_graph;
}

static void free_graph_link(graph_link_t *link) {
    while (NULL != link) {
        graph_link_t *delete_node = link;

        link = link->next;

        free(delete_node);
        delete_node = NULL;
    }
}

scl_error_t free_graph(graph_t * const __restrict__ gr) {
    if (NULL != gr) {
        if (NULL != gr->vertices) {
           for (size_t iter = 0; iter < gr->size; ++iter) {
                if (NULL != gr->vertices[iter]) {
                    free_graph_link(gr->vertices[iter]->link);

                    free(gr->vertices[iter]);
                    gr->vertices[iter] = NULL;
                }
            }

            free(gr->vertices);
            gr->vertices = NULL;
        }

        if (NULL != gr->visit) {
            free(gr->visit);
            gr->visit = NULL;
        }

        free(gr);

        return SCL_OK;
    }

    return SCL_NULL_GRAPH;
}

static graph_link_t* create_graph_link(uint64_t vertex, long double edge_len) {
    if ((UINT64_MAX == vertex) || (__LDBL_MAX__ == edge_len)) {
        errno = EINVAL;
        perror("Data input for link creation is invalid");
        return NULL;
    }

    graph_link_t *new_link = malloc(sizeof(*new_link));

    if (NULL != new_link) {
        new_link->vertex = vertex;
        new_link->edge_len = edge_len;
        new_link->next = NULL;
    } else {
        errno = ENOMEM;
        perror("Not enough memory to allocate graph vertex");
    }

    return new_link;
}

scl_error_t graph_insert_edge(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t end_vertex, long double edge_len) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    if ((gr->size >= start_vertex) || (gr->size >= end_vertex)) {
        return SCL_INVALID_VERTEX_NUMBER;
    }

    if ((NULL == gr->vertices[start_vertex]) || (NULL == gr->vertices[end_vertex])) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    if (__LDBL_MAX__ == edge_len) {
        return SCL_INVALID_EDGE_LENGTH;
    }

    graph_link_t *new_vertex = create_graph_link(start_vertex, edge_len);

    if (NULL == new_vertex) {
        return SCL_NOT_ENOUGHT_MEM_FOR_NODE;
    }

    new_vertex->next = gr->vertices[start_vertex]->link;
    gr->vertices[start_vertex]->link = new_vertex;

    ++(gr->vertices[start_vertex]->out_deg);
    ++(gr->vertices[end_vertex]->in_deg);

    return SCL_OK;
}

scl_error_t graph_insert_vertices(graph_t * const __restrict__ gr, size_t new_vertices) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    uint8_t *try_realloc_visit = realloc(gr->visit, sizeof(*gr->visit) * (gr->size + new_vertices));

    if (NULL == try_realloc_visit) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    gr->visit = try_realloc_visit;

    graph_vertex_t **try_realloc = realloc(gr->vertices, sizeof(*try_realloc) * (gr->size + new_vertices));

    if (NULL == try_realloc) {
        return SCL_REALLOC_GRAPH_VERTICES_FAIL;
    }

    for (size_t iter = gr->size; iter < gr->size + new_vertices; ++iter) {
        gr->vertices[iter] = malloc(sizeof(*gr->vertices[iter]));

        if (NULL != gr->vertices[iter]) {
            gr->vertices[iter]->link = NULL;
            gr->vertices[iter]->in_deg = 0;
            gr->vertices[iter]->out_deg = 0;
        } else {
            for (size_t del_iter = gr->size; del_iter <= iter; ++del_iter) {
                free(gr->vertices[del_iter]);
                gr->vertices[del_iter] = NULL;
            }

            free(try_realloc);
            try_realloc = NULL;

            return SCL_REALLOC_GRAPH_VERTICES_FAIL;
        }
    }

    gr->size += new_vertices;

    gr->vertices = try_realloc;

    return SCL_OK;
}

graph_t* create_transpose_graph(const graph_t * const __restrict__ gr) {
    if ((NULL == gr) || (0 == gr->size) || (NULL == gr->vertices)) {
        errno = EINVAL;
        perror("Graph sent as input in transpose function is not valid");

        return NULL;
    }

    graph_t *transpose_gr = create_graph(gr->size);

    if (NULL == transpose_gr) {
        return NULL;
    }

    scl_error_t err = SCL_OK;

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

    return transpose_gr;
}

scl_error_t graph_print(const graph_t * const __restrict__ gr, const void * const data_arr, graph_print_func print) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_LINKS;
    }

    if (NULL == print) {
        return SCL_NULL_GRAPH_PRINT_FUNC;
    }

    if (0 == gr->size) {
        printf("[ ]\n");
    } else {
        for (size_t iter = 0; iter < gr->size; ++iter) {
            if (NULL == gr->vertices[iter]) {
                return SCL_NULL_GRAPH_VERTEX;
            }

            graph_link_t *link = gr->vertices[iter]->link;

            while (NULL != link) {
                print(data_arr, link->vertex);

                link = link->next;
            }
        }
    }
    
    return SCL_OK;
}