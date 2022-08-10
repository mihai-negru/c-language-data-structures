# Documentation for graph object ([scl_graph.h](../src/include/scl_graph.h))

The graph object is implemented using adjacency list

## How to create a graph and how to destroy it?

Three main function that will help you to initialize a graph and to free it from heap memory are:

1. **create_graph** - function will take as input an initial number of vertices and will allocate memory for them, however you are free to add or to remove some vertices (We will discuss this in the following sections). The function will return a pointer to the memory location of the graph object

2. **create_transpose_graph** - Function will take as input a pointer to the location of an allocated graph object and will return a new allocated graph which will be the transposed graph of the original graph. The input graph object will be not modified after calling this function.

3. **free_graph** - Function will take as input a pointer to the location of an allocated graph object and will try to delete every byte of memory allocated for the graph object. You also can pass a NULL graph pointer, but no action will be executed and a **scl_error_t** will be returned.

Example of basic functions for graph object:

```C
    #include <scl_datastruc.h>

    int main(void) {
        graph_t *gg = create_graph(10); // Will create 10 vertices with no edges

        graph_t *t_gg = create_transpose_graph(gg);
        // Create the transpose graph in this case will
        // be the same as gg because no edge was inserted

        // If the graoh is not directed then the transpose graph is the same as graph

        free_graph(t_gg); // t_gg or gg can be NULL pointers
        free_graph(gg);   // but if they are NULL no memory will be freed

        return 0;
    }
```

## Functions to maintain the graph object

I created some functions to insert edges, to insert vertices, to remove edges and to remove vertices.

1. **graph_insert_edge** - Function will create an edge between two vertices. Vertices has to be in the graph object (not to be greater than the number of vertices in the selected graph). At the insertion of the graph user has also to specify a weight for the current edge. The edge length shall not be **__ LDBL_MAX __**.

2. **graph_insert_vertices** - Function to new vertices in the graph. Old vertices and old edges will not be modyfied, the function will just add some new vertices with no link to other graph components, if you want to link them you will have to create same edges.

3. **graph_delete_edge** - Function will take as input a pointer to agraph object and two vertices and will check if there exists an edge between two selected vertices. If exists an edge than the function will remove just the first occurence of an edge that links selected vertices. If no edge was found between two selected vertices an error will be returned.

4. **graph_delete_all_edges** - Function does the same thing as **graph_delete_edge**, but removes every edge between two selected vertices from the graph, so if vertex **x** has more than 1 edge to vertex **y** than all of the edges will be removed.

5. **graph_delete_vertex** - Function to delete one vertex from the whole graph, all the edges that start from selected vertex or end in the selected vertex will be removed. **Very IMPORTANT** all the vertices that had a greater number (index) than selected vertex will deacrease their number with one, so if vertex **2** was deleted than vertex **3** becomes vertex **2** and vertex **4** becomes vertex **3** and so on after all vertices are updated. the links will also be updated to the new vertices number.

6. **graph_print** - Function to print the adacency list of the graph object. It will take as input a pointer to a graph object and an array of strings. The string array can be NULL in this case the function will print the number of the vertex, if string array is not NULL than the name of the vertex will be printed instead.

Example of using maintaining functions for the graph:

```C
    #include <scl_datastruc.h>

    int main(void) {
        graph_t *gg = create_graph(3); // Will create 10 vertices with no edges

        scl_error_t err = SCL_OK;

        err = graph_insert_edge(gg, make_triplet(0, 2, 1));

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_insert_edge(gg, make_triplet(3, 2, 1));
        // Wrong because there does not exist a vertex with number 3, the counting
        // of vertices starts from 0.

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_insert_edge(gg, make_triplet(2, 0, -44));
        // Good there can exist negative weighted edges

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_insert_edge(gg, make_triplet(0, 1, 0));
        // Good there can exist 0 weighted edges between vertices

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_insert_edge(gg, make_triplet(2, 2, __LDBL_MAX__));
        // Wrong an edge cannot have such weight

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_insert_vertices(gg, 2);
        // This function will add two more vertices in the graph, the second
        // must not be 0, an error will be thrown

        err = graph_insert_edge(gg, make_triplet(0, 4, 1));
        err = graph_insert_edge(gg, make_triplet(0, 4, 1));
        err = graph_insert_edge(gg, make_triplet(0, 4, 1));
        err = graph_insert_edge(gg, make_triplet(0, 4, 1));
        err = graph_insert_edge(gg, make_triplet(0, 4, 1));
        err = graph_insert_edge(gg, make_triplet(4, 2, 1));
        // Now this works because we added 2 more vertices

        if (SCL_OK != err) {
            scl_error_message(err);
        }

        err = graph_delete_edge(gg, 0, 4);
        // Will remove just one edge between 0 and 4

        err = graph_delete_all_edges(gg, 0, 4);
        // After this 0 will have no edge with 4

        err = graph_delete_vertex(gg, 3);
        // The vertex with number 3 will be removed and 4 will take its number

        graph_print(gg, NULL /* No names for vertices */);

        // (0): (2) -> (1) -> (~)
        // (1): (~)
        // (2): (0) -> (~)
        // (3): (2) -> (~)

        // (~) means no mor evertices in this list

        free_graph(gg);

        return 0;
    }
```

>**NOTE:** By default the graph is an weighted directed graph, however you can do to get an unordered graph and one without edges weight. To do this you will have when inserting and edge between vertex **a** and vertex **b**, you should insert an edge from **b** to **a**, in such way you will get an **unordered graph**, if you want to get a graph wih=thour weights you shall insert in the third slot the same value for all edges, the best an most optimal value to insert would be **1**, so when inserting an edge set its weight as **1**. If you will do this proccess you will get an unweighted graph. So from one graph object you can make 4 different data structures: *directed weighted graph*, *directed unweighted graph*, *undirected weighted graph*, *undirected unweighted graph*.

## Functions to traverse a graph object

As the standard is, there exists two graph traversal algorithms **depth-first-search** and **breath-first-search**
1. **graph_bfs_traversal** - Function will take as input a pointer to a graph object to traverse, a start vertex and an **ALLOCATED** array to print the path of the bfs traversal. Function will return the number of vertices traversed in one bfs traversal.

2. **graph_dfs_traversal** - Function will take as input a pointer to a graph object to traverse, a start vertex and an **ALLOCATED** array to print the path of the bfs traversal. Function will return the number of vertices traversed in one dfs traversal.

3. **graph_has_cycle** - Function will take a pointer to a graph object as input and will find if there exists a cycle or not, works both for undirected and directed graphs. Function will return 1 if there exists a cycle and 0 if graph is acyclic.

>**NOTE:** When the traversal function is called all the vertices will be marked as unvisited. Also if you do not care about the path of the bfs or dfs traversal you can send a **NULL** pointer instead of an allocated array

Example of using graph traversals:

```C
    // Let's suppose that we have the following graph gg:

    // (0): (2) -> (3) -> (4) -> (~)
    // (1): (0) -> (2) -> (~)
    // (2): (0) -> (~)
    // (3): (4) -> (~)
    // (4): (0) -> (1) -> (~)

    // Now let's allocate an array to print the path
    // The best case is that all the vertices will be traversed
    // so let's allocate the path array with graph size elements

    uint64_t *path = malloc(sizeof(*path) * get_graph_size(gg));

    if (NULL != path) {
        size_t traversed_vertices = graph_bfs_traversal(gg, 0, path);

        printf("BFS traversal:\n");
        for (size_t i = 0; i < traversed_vertices; ++i) {
            printf("%lu ", path[i]);
        }

        printf("\n");
    }

    if (0 == graph_has_cycle(gg)) {
        printf("Gra[h is a DAG\n");
    }

    // BFS: 0 2 3 4 1
```

## Functions just for directed graphs

1. **graph_topological_sort** - Function to calculate the topological sort of the graph. Function will take as input a pointer to a graph object and an **ALLOCATED** array of **uint64_t** to print the topological sort. However we now that the input array has to have **graph size** dimenision because all the vertices will be traversed, the function will return the size of the traversed vertices just in case something went wrong.

2. **graph_is_strongly_connected** - Function to check if a directed graph is strongly connected or not. Function will take as input a pointer to a graph object and will return 1 if it is strongly connected and 0 otherwise.

3. **graph_strongly_connected_components** - Function to calculate all strongly connected components from a directed graph. Function will take as input a pointer to a graph object and a pointer to a size_t element which will represent the number of strongly connected components from the graph which will be calculated as program executes its code. This function will return a matrix of paths, every row represents one strongly connected component, the first element on the row represents the number of vertices from the component.

4. **graph_vertex_past_vertices** - Function to calculate the past vertices of one selected vertex from a **DAG**. It is important that the input graph is a **DAG** if it is not than the program will not fail but will return a wrong answer, because no past vertices can be calculated if there exists cycles in the graph. Function will take as input a graph, a vertex to calculate its past vertices and an **ALLOCATED** array to save those vertices. Past vertices of one vertex are those vertices that result from selected vertex. Think about a tree, all the nodes from the left subtree and the right subtree of one node are its past vertices. The function will return the number of past vertices.

5. **graph_vertex_future_vertices** - Function to calculate the future vertices of one selected vertex from a **DAG**. It is important that the input graph is a **DAG** if it is not than the program will not fail but will return a wrong answer, because no future vertices can be calculated if there exists cycles in the graph. Function will take as input a graph, a vertex to calculate its future vertices and an **ALLOCATED** array to save those vertices. Future vertices of one vertex are those vertices that have a path to selected vertex. Think about a tree, all the ancestors of one selected node will be the future nodes for that tree node. The function will return the number of future vertices.

6. **graph_vertex_anticone_vertices** - Function to calculate the anticone vertices of one selected vertex from a **DAG**. It is important that the input graph is a **DAG** if it is not than the program will not fail but will return a wrong answer, because no anticone vertices can be calculated if there exists cycles in the graph. Function will take as input a graph, a vertex to calculate its anticone vertices and an **ALLOCATED** array to save those vertices. Anticone vertices of one vertex are those vertices that cannot reach the selected vertex or cannot be reached from the selected vertex. The function will return the number of anticone vertices.

7. **graph_tips_vertices** - Function to calculate the tips vertices of one graph. Tips vertices are those vertices that cannot be reached from other vertices. Think of a tree, that thise vertices are the roots of some trees, you cannot reach a root node from a child. Function will take as input a pointer to a graph object and an **ALLOCATED** array of **uint64_t** to save the vertices and will return the number of tips vertices calculated from selected graph object.

Example of using above functions;

```C
    // Let's suppose that we created a graph gg and inserted a lot of edges

    // For every function above the **ALLOCATED** array object 
    // to save paths or vertices should at least
    // have graph size elements so we allocate as follows

    if (0 == graph_has_cycle(gg)) {
        uint64_t *path = malloc(sizeof(*path) * get_graph_size(gg));

        // now

        size_t path_size = graph_vertex_past_vertices(gg, 0, path);

        for (size_t i = 0; i < path_size; ++i) {
            printf("%lu ", path[i]);
        }

        printf("\n");

        free(path);
    }

    size_t number_of_scc = 0;

    uint64_t **path = graph_strongly_connected_componets(gg, &number_of_scc);

    if (NULL != path) {

        for (size_t i = 0; i < number_of_scc; ++i) {
            printf("Strongly connected component number (%lu):\n", i);

            for (size_t j = 1; j <= path[i][0]; ++j) {
                printf("%lu ", path[i][j]);
            }

            printf("\n");
        }

        // Do not forget to free the memory allocated for the matrix
        for (i = 0; i < number_of_scc; ++i) {
            free(path[i]);
        }

        free(path);

    } else {
        printf("Something went wrong");
    }
```

## Functions that work with the weight of the edges

1. **graph_dijkstra** - Function will execute the dijkstra's algorithm on the selected graph. Function will take as input a pointer to a graph object, a start vertex to calculate the distances, an **ALLOCATED** array to calculate distances and an array to calculate the path from start vertex to any other vertex which represent the minimum path to reach the selected vertex. The path array is optional can be **NULL**, but the distances array has to be allocated. Function will return an error if something went wrong or SCL_OK if everything was allright.

2. **graph_prim** - Function will execute the prim's algorithm on the selected graph. Function will take as input a pointer to a graph object, a start vertex to calculate the distances, an **ALLOCATED** array to calculate distances and an **ALLOCATED** array to calculate the minimum cost spanning tree of the graph from start vertex to any other vertex which represent the minimum path to reach the selected vertex. Function will return an error if something went wrong or SCL_OK if everything was allright.

3. **graph_floyd_warshall** - Function to execute the Floyd-Warshall algorithm on the graph. Function will take as input a pointer to a graph object and an **ALLOCATED** square matrix of graph size dimensions. Function will return an error if something went wrong or SCL_OK if everything was allright.

Example of using above functions:

```C
    // Let's suppose that we created graph gg and inserted some edges

    long double *dists = malloc(sizeof(*dists) * get_graph_size(gg));
    uint64_t *path = malloc(sizeof(*path) * gr->size);

    scl_error_t err = graph_dijkstra(gg, 0, dists, path);

    printf("Distances from vertex 0:\n");
    for (size_t i = 0; i < get_graph_size(gg); ++i) {
        printf("Vertex (%lu) -> Dist (%llf)\n", i, dists[i]);
    }

    if (__LDBL_MAX__ != dists[3]) {
        printf("MIN path from vertex 3 to 0 is:\n");

        size_t i = 3;

        while (dists[i] > 0) {
            printf("%lu ", dists[i]);
            i = dists[i];
        }
    }

    free(path);
    free(dists);
```

## For some other examples of using graph objects you can look up at [examples](../examples/graph)