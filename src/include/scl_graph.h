/**
 * @file scl_graph.h
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

#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "scl_config.h"

typedef struct graph_link_s {
    uint64_t vertex;
    long double edge_len;
    struct graph_link_s *next;
} graph_link_t;

typedef struct graph_vertex_s {
    graph_link_t *link;
    size_t in_deg;
    size_t out_deg;
} graph_vertex_t;

typedef struct graph_s {
    graph_vertex_t **vertices;
    uint8_t *visit;
    size_t size;
} graph_t;

graph_t*            create_graph                            (size_t number_of_vertexes);
scl_error_t         free_graph                              (graph_t * const __restrict__ gr);

scl_error_t         graph_insert_edge                       (const graph_t * const __restrict__ gr, uint64_t first_vertex, uint64_t second_vertex, long double edge_len);
scl_error_t         graph_insert_vertices                   (graph_t * const __restrict__ gr, size_t new_vertices);
graph_t*            create_transpose_graph                  (const graph_t * const __restrict__ gr);
scl_error_t         graph_print                             (const graph_t * const __restrict__ gr, const uint8_t ** const data_arr);

scl_error_t         graph_delete_edge                       (const graph_t * const __restrict__ gr, uint64_t first_vertex, uint64_t second_vertex);
scl_error_t         graph_delete_all_edges                  (const graph_t * const __restrict__ gr, uint64_t first_vertex, uint64_t second_vertex);
scl_error_t         graph_delete_vertex                     (graph_t * const __restrict__ gr, uint64_t vertex);

size_t              graph_bfs_traverse                      (const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path);
size_t              graph_dfs_traverse                      (const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path);

uint8_t             graph_has_cycle                         (const graph_t * const __restrict__ gr);
size_t              graph_vertex_past_vertices              (const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path);
size_t              graph_vertex_future_vertices            (const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path);
size_t              graph_vertex_anticone_vertices          (const graph_t * const __restrict__ gr, uint64_t start_vertex, uint64_t * __restrict__ vertex_path);
size_t              graph_tips_vertices                     (const graph_t * const __restrict__ gr, uint64_t * __restrict__ vertex_path);

#endif /* GRAPH_UTILS_H_ */