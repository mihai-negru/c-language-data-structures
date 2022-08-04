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

    if ((gr->size <= start_vertex) || (gr->size <= end_vertex)) {
        return SCL_INVALID_VERTEX_NUMBER;
    }

    if ((NULL == gr->vertices[start_vertex]) || (NULL == gr->vertices[end_vertex])) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    if (__LDBL_MAX__ == edge_len) {
        return SCL_INVALID_EDGE_LENGTH;
    }

    graph_link_t *new_vertex = create_graph_link(end_vertex, edge_len);

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

scl_error_t graph_print(const graph_t * const __restrict__ gr, const uint8_t ** const data_arr) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_LINKS;
    }

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

            printf("-|\n");
        }
    }
    
    return SCL_OK;
}

scl_error_t graph_delete_edge(const graph_t * const __restrict__ gr, uint64_t first_vertex, uint64_t second_vertex) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    if (first_vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    if (NULL == gr->vertices[first_vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    graph_link_t *delete_link = gr->vertices[first_vertex]->link;
    graph_link_t *parent_delete_link = NULL;

    while ((NULL != delete_link) & (second_vertex != delete_link->vertex)) {
        parent_delete_link = delete_link;
        delete_link = delete_link->next;
    }

    if (NULL == delete_link) {
        return SCL_EDGE_NOT_FOUND;
    }

    if (NULL == parent_delete_link) {
        gr->vertices[first_vertex]->link = delete_link->next;
    } else {
        parent_delete_link->next = delete_link->next;
    }

    delete_link->next = NULL;
    free(delete_link);
    delete_link = NULL;

    --(gr->vertices[first_vertex]->out_deg);

    return SCL_OK;
    
}

scl_error_t graph_delete_all_edges(const graph_t * const __restrict__ gr, uint64_t first_vertex, uint64_t second_vertex) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    if (first_vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    if (NULL == gr->vertices[first_vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    graph_link_t *delete_link = gr->vertices[first_vertex]->link;
    graph_link_t *parent_delete_link = NULL;

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

    return SCL_OK;
}

scl_error_t graph_delete_vertex(graph_t * const __restrict__ gr, uint64_t vertex) {
    if (NULL == gr) {
        return SCL_NULL_GRAPH;
    }

    if (NULL == gr->vertices) {
        return SCL_NULL_GRAPH_VERTICES;
    }

    if (vertex >= gr->size) {
        return SCL_VERTEX_OUT_OF_BOUND;
    }

    if (NULL == gr->vertices[vertex]) {
        return SCL_NULL_GRAPH_VERTEX;
    }

    scl_error_t err = SCL_OK;

    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (iter != vertex) {
            err = graph_delete_all_edges(gr, iter, vertex);

            if (SCL_OK != err) {
                return err;
            }
        }
    }

    free_graph_link(gr->vertices[vertex]->link);
    gr->vertices[vertex]->in_deg = gr->vertices[vertex]->out_deg = 0;

    free(gr->vertices[vertex]);
    gr->vertices[vertex] = NULL;

    --(gr->size);

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

    return SCL_OK;
}

size_t graph_bfs_traverse(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    if (start_vertex >= gr->size) {
        return 0;
    }

    size_t traversed_vex = 0;

    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    queue_t *bfs_queue = create_queue(NULL, sizeof(*vertex_path));

    if (NULL != bfs_queue) {
        gr->visit[start_vertex] = 1;

        if (SCL_OK != queue_push(bfs_queue, &start_vertex)) {
            free_queue(bfs_queue);
            return 0;
        }

        while (!is_queue_empty(bfs_queue)) {
            const uint64_t *front_vertex = queue_front(bfs_queue);

            vertex_path[traversed_vex++] = *front_vertex;
            /* Insted of adding in path variable you can perform an action */

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

    return traversed_vex;
}

static void graph_dfs_traverse_helper(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path, size_t * __restrict__ traversed_vex) {
    gr->visit[start_vertex] = 1;

    vertex_path[(*traversed_vex)++] = start_vertex;

    graph_link_t *link = gr->vertices[start_vertex]->link;

    while (NULL != link) {
        if (0 == gr->visit[link->vertex]) {
            graph_dfs_traverse_helper(gr, link->vertex, vertex_path, traversed_vex);
        }

        link = link->next;
    }
}

size_t graph_dfs_traverse(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    if (start_vertex >= gr->size) {
        return 0;
    }

    size_t traversed_vex = 0;

    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    graph_dfs_traverse_helper(gr, start_vertex, vertex_path, &traversed_vex);

    return traversed_vex;
}

static uint8_t graph_has_cycle_helper(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t current_vertex) {
    if ((1 == gr->visit[start_vertex]) && (start_vertex == current_vertex)) {
        return 1;
    }

    if (1 == gr->visit[current_vertex]) {
        return 0;
    }

    graph_link_t *link = gr->vertices[current_vertex]->link;

    int has_cycle = 0;

    while ((NULL != link) && (0 == has_cycle)) {

        has_cycle = graph_has_cycle_helper(gr, start_vertex, link->vertex);

        link = link->next;
    }

    return has_cycle;
}

uint8_t graph_has_cycle(const graph_t * const __restrict__ gr) {
    if ((NULL == gr) || (NULL == gr->visit) || (NULL == gr->vertices)) {
        return 0;
    }

    uint8_t has_cycle = 0;

    for (size_t iter = 0; (iter < gr->size) && (0 == has_cycle); ++iter) {
        for (size_t make_zero = 0; make_zero < gr->size; ++make_zero) {
            gr->visit[make_zero] = 0;

            has_cycle = graph_has_cycle_helper(gr, iter, iter);
        }
    }

    return has_cycle;
}

size_t graph_vertex_past_vertices(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    if (start_vertex >= gr->size) {
        return 0;
    }

    size_t traversed_vex = 0;

    for (size_t iter = 0; iter < gr->size; ++iter) {
        gr->visit[iter] = 0;
    }

    queue_t *bfs_queue = create_queue(NULL, sizeof(*vertex_path));

    if (NULL != bfs_queue) {
        gr->visit[start_vertex] = 1;

        if (SCL_OK != queue_push(bfs_queue, &start_vertex)) {
            free_queue(bfs_queue);
            return 0;
        }

        while (!is_queue_empty(bfs_queue)) {
            const uint64_t *front_vertex = queue_front(bfs_queue);

            if (start_vertex != *front_vertex) {
                vertex_path[traversed_vex++] = *front_vertex;
                /* Insted of adding in path variable you can perform an action */
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

    return traversed_vex;
}

size_t graph_vertex_future_vertices(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    if (start_vertex >= gr->size) {
        return 0;
    }

    graph_t *transpose_graph = create_transpose_graph(gr);

    size_t traversed_vex = graph_vertex_past_vertices(transpose_graph, start_vertex, vertex_path);

    free_graph(transpose_graph);

    return traversed_vex;
}

size_t graph_vertex_anticone_vertices(const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    if (start_vertex >= gr->size) {
        return 0;
    }

    uint64_t * __restrict__ past_vertices = malloc(sizeof(*past_vertices) * gr->size);

    if (NULL == past_vertices) {
        return 0;
    }

    uint64_t * __restrict__ future_vertices = malloc(sizeof(*future_vertices) * gr->size);

    if (NULL == future_vertices) {
        free(past_vertices);
        return 0;
    }

    size_t past_size = graph_vertex_past_vertices(gr, start_vertex, past_vertices);
    size_t future_size = graph_vertex_future_vertices(gr, start_vertex, future_vertices);
    size_t traversed_size = 0;

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

    return traversed_size;
}

size_t graph_tips_vertices(const graph_t * const __restrict__ gr, uint64_t * __restrict__ vertex_path) {
    if (NULL == gr) {
        return 0;
    }

    if (NULL == gr->vertices) {
        return 0;
    }

    size_t traversed_size = 0;

    for (size_t iter = 0; iter < gr->size; ++iter) {
        if (NULL != gr->vertices[iter]) {
            if (0 == gr->vertices[iter]->in_deg) {
                vertex_path[traversed_size++] = iter;
            }
        }
    }

    return traversed_size;
}





