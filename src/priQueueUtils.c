/**
 * @file priQueueUtils.c
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

#include "./include/priQueueUtils.h"

#define DEFAULT_CAPACITY 10
#define DEFAULT_REALLOC_RATIO 2

priority_queue* create_priority_queue(
    size_t init_capacity,
    int (*compare_data)(const void *, const void *),
    int (*compare_priority)(const void *, const void *),
    void (*free_data)(void *),
    void (*free_priority)(void *)
) {
    if (compare_priority == NULL) {
        errno = EINVAL;
        perror("Compare function undefined for priority queue");
        return NULL;
    }

    if (init_capacity == 0)
        init_capacity = DEFAULT_CAPACITY;

    priority_queue *new_pri_queue = (priority_queue *)malloc(sizeof(priority_queue));

    if (new_pri_queue) {
        new_pri_queue->capacity = init_capacity;
        new_pri_queue->compare_data = compare_data;
        new_pri_queue->compare_priority = compare_priority;
        new_pri_queue->free_data = free_data;
        new_pri_queue->free_priority = free_priority;

        new_pri_queue->size = 0;

        new_pri_queue->nodes = (pri_node **)malloc(init_capacity * sizeof(pri_node *));

        if (new_pri_queue->nodes == NULL) {
            free(new_pri_queue);
            return NULL;
        }

        for (size_t iter = 0; iter < init_capacity; ++iter)
            new_pri_queue->nodes[iter] = NULL;
    }

    return new_pri_queue;
}

static void free_priority_queue_node(priority_queue *pqueue, pri_node **free_node) {
    if (pqueue != NULL && free_node != NULL && (*free_node != NULL)) {
        if (pqueue->free_data != NULL && (*free_node)->data != NULL)
            pqueue->free_data((*free_node)->data);

        if ((*free_node)->data != NULL)
            free((*free_node)->data);

        (*free_node)->data = NULL;

        if (pqueue->free_priority != NULL && (*free_node)->pri != NULL)
            pqueue->free_priority((*free_node)->pri);

        if ((*free_node)->pri != NULL)
            free((*free_node)->pri);

        (*free_node)->pri = NULL;

        free((*free_node));

        (*free_node) = NULL;
    }
}

void free_priority_queue(priority_queue *pqueue) {
    if (pqueue != NULL) {
        if (pqueue->nodes != NULL) {
            for (size_t iter = 0; iter < pqueue->size; ++iter)
                free_priority_queue_node(pqueue, &pqueue->nodes[iter]);

            free(pqueue->nodes);
            pqueue->nodes = NULL;  
        }

        free(pqueue);
        pqueue = NULL;
    }
}

#define get_node_left_child_pos(node_index) (2 * (node_index) + 1)

#define get_node_right_child_pos(node_index) (2 * (node_index) + 2)

#define get_node_parent_pos(node_index) (((node_index) - 1) / 2)

static void swap_pri_queue_nodes(pri_node **first_node, pri_node **second_node) {
    pri_node *temp_node = *first_node;
    *first_node = *second_node;
    *second_node = temp_node;
}

static int sift_node_up(priority_queue *pqueue, size_t start_index) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->compare_priority == NULL)
        return 1;

    while (start_index > 0 && pqueue->compare_priority(pqueue->nodes[start_index]->pri, pqueue->nodes[get_node_parent_pos(start_index)]->pri) >= 1) {
        swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[get_node_parent_pos(start_index)]);
        
        start_index = get_node_parent_pos(start_index);
    }

    return 0;
}

static int sift_node_down(priority_queue *pqueue, size_t start_index) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->compare_priority == NULL)
        return 1;

    size_t swap_index = start_index;
    size_t check_index = get_node_left_child_pos(start_index);

    if (check_index < pqueue->size && pqueue->compare_priority(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)
        swap_index = check_index;

    check_index = get_node_right_child_pos(start_index);

    if (check_index < pqueue->size && pqueue->compare_priority(pqueue->nodes[check_index]->pri, pqueue->nodes[swap_index]->pri) >= 1)
        swap_index = check_index;

    if (start_index != swap_index) {
        swap_pri_queue_nodes(&pqueue->nodes[start_index], &pqueue->nodes[swap_index]);

        return sift_node_down(pqueue, swap_index); 
    }

    return 0;
}

static pri_node* create_priority_queue_node(const void *data, const void *priority, size_t data_size, size_t pri_size) {
    if (priority == NULL || pri_size == 0)
        return NULL;

    pri_node *new_pri_queue_node = (pri_node *)malloc(sizeof(pri_node));

    if (new_pri_queue_node) {
        if (data != NULL && data_size != 0) {
            new_pri_queue_node->data = malloc(data_size);

            if (new_pri_queue_node->data == NULL) {
                free(new_pri_queue_node);
            
                errno = ENOMEM;
                perror("Not enough memory for data allocation");
            
                return NULL;
            }

            memcpy(new_pri_queue_node->data, data, data_size);
        } else {
            new_pri_queue_node->data = NULL;
        }

        new_pri_queue_node->pri = malloc(pri_size);

        if (new_pri_queue_node->pri == NULL) {
            free(new_pri_queue_node->data);
            free(new_pri_queue_node);
            
            errno = ENOMEM;
            perror("Not enough memory for priority allocation");
            
            return NULL;
        }

        memcpy(new_pri_queue_node->pri, priority, pri_size);
    } else {
        errno = ENOMEM;
        perror("Not enough memory for priority queue node allocation");
    }

    return new_pri_queue_node;
}

priority_queue* heapify(
    const void *data,
    const void *priority,
    size_t data_size,
    size_t pri_size,
    size_t number_of_data,
    int (*compare_data)(const void *, const void *),
    int (*compare_priority)(const void *, const void *),
    void (*free_data)(void *),
    void (*free_priority)(void *)
) {
    if (priority == NULL || pri_size == 0 || number_of_data == 0 || compare_priority == NULL)
        return NULL;

    priority_queue *new_pqueue = create_priority_queue(number_of_data, compare_data, compare_priority, free_data, free_priority);

    if (new_pqueue) {
        for (size_t iter = 0; iter < new_pqueue->capacity; ++iter) {
            pri_node *new_pqueue_node = NULL;

            if (data != NULL && data_size != 0) {
                new_pqueue_node = create_priority_queue_node(data + iter * data_size, priority + iter * pri_size, data_size, pri_size);
            } else {
                new_pqueue_node = create_priority_queue_node(NULL, priority + iter * pri_size, 0, pri_size);
            }

            if (new_pqueue_node == NULL)
                return NULL;
            
            new_pqueue->nodes[iter] = new_pqueue_node;

            ++(new_pqueue->size);
        }

        for (int iter = (int)(new_pqueue->size / 2 - 1); iter >= 0; --iter)
            sift_node_down(new_pqueue, (size_t)iter);
    }

    return new_pqueue;
}

int change_node_priority(priority_queue *pqueue, size_t node_index, const void *new_pri, size_t pri_size) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->nodes[node_index] == NULL || new_pri == NULL || pri_size == 0)
        return 1;

    if (node_index == __SIZE_MAX__ || node_index >= pqueue->size)
        return 1;

    if (pqueue->nodes[node_index]->pri == NULL || pqueue->compare_priority == NULL)
        return 1;

    if (pqueue->compare_priority(pqueue->nodes[node_index]->pri, new_pri) >= 1) {
        memmove(pqueue->nodes[node_index]->pri, new_pri, pri_size);

        return sift_node_down(pqueue, node_index);
    }

    if (pqueue->compare_priority(pqueue->nodes[node_index]->pri, new_pri) <= -1) {
        memmove(pqueue->nodes[node_index]->pri, new_pri, pri_size);

        return sift_node_up(pqueue, node_index);
    }

    return 0;
}

size_t pri_find_data_index(priority_queue *pqueue, const void *data) {
    if (pqueue == NULL || pqueue->size == 0 || pqueue->nodes == NULL || data == NULL || pqueue->compare_data == NULL)
        return __SIZE_MAX__;

    for (size_t iter = 0; iter < pqueue->size; ++iter)
        if (pqueue->nodes[iter] != NULL)
            if (pqueue->compare_data(pqueue->nodes[iter]->data, data) == 0)
                return iter;

    return __SIZE_MAX__;
}

size_t pri_find_pri_index(priority_queue *pqueue, const void *priority) {
    if (pqueue == NULL || pqueue->size == 0 || pqueue->nodes == NULL || priority == NULL || pqueue->compare_priority == NULL)
        return __SIZE_MAX__;

    for (size_t iter = 0; iter < pqueue->size; ++iter)
        if (pqueue->nodes[iter] != NULL)
            if (pqueue->compare_priority(pqueue->nodes[iter]->pri, priority) == 0)
                return iter;

    return __SIZE_MAX__;
}

int pri_queue_push(priority_queue *pqueue, const void *data, const void *priority, size_t data_size, size_t pri_size) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->capacity == 0 || priority == NULL || pri_size == 0)
        return 1;

    if (pqueue->size >= pqueue->capacity) {
        pqueue->capacity *= DEFAULT_REALLOC_RATIO;
        
        pri_node **try_realloc = (pri_node **)realloc(pqueue->nodes, pqueue->capacity * sizeof(pri_node *));

        if (try_realloc == NULL) {
            pqueue->capacity /= DEFAULT_REALLOC_RATIO;

            errno = ENOMEM;
            perror("Not enough memory to reallocate new nodes");

            return 1;
        }

        pqueue->nodes = try_realloc;
    }

    pri_node *add_node = create_priority_queue_node(data, priority, data_size, pri_size);

    if (add_node == NULL)
        return 1;

    pqueue->nodes[pqueue->size] = add_node;

    ++(pqueue->size);

    return sift_node_up(pqueue, (pqueue->size - 1));
}

const void* pri_queue_top_data(priority_queue *pqueue) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->nodes[0] == NULL)
        return NULL;

    return pqueue->nodes[0]->data;
}

const void* pri_queue_top_pri(priority_queue *pqueue) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->nodes[0] == NULL)
        return NULL;

    return pqueue->nodes[0]->pri;
}

int pri_queue_pop(priority_queue *pqueue) {
    if (pqueue == NULL || pqueue->nodes == NULL || pqueue->capacity == 0 || pqueue->size == 0)
        return 1;

    swap_pri_queue_nodes(&pqueue->nodes[0], &pqueue->nodes[pqueue->size - 1]);

    free_priority_queue_node(pqueue, &pqueue->nodes[pqueue->size - 1]);

    --(pqueue->size);

    return sift_node_down(pqueue, 0);
}

size_t pri_queue_size(priority_queue *pqueue) {
    if (pqueue == NULL)
        return __SIZE_MAX__;
    return pqueue->size;
}

int is_priq_empty(priority_queue *pqueue) {
    if (pqueue == NULL || pqueue->capacity == 0 || pqueue->nodes == NULL || pqueue->size == 0)
        return 1;

    return 0;
}

void pri_queue_traverse(priority_queue *pqueue, void (*action)(const pri_node *)) {
    if (pqueue == NULL || pqueue->nodes == NULL || action == NULL)
        return;

    for (size_t iter = 0; iter < pqueue->size; ++iter)
        action(pqueue->nodes[iter]);
}

void heap_sort(void *arr, size_t number_of_arr, size_t arr_elem_size, int (*compare_arr)(const void *, const void *)) {
    if (arr == NULL || number_of_arr == 0 || arr_elem_size == 0 || compare_arr == NULL)
        return;

    priority_queue *heap = heapify(NULL, arr, 0, arr_elem_size, number_of_arr, NULL, compare_arr, NULL, NULL);

    for (size_t iter = 0; iter < number_of_arr; ++iter) {
        memcpy(arr + iter * arr_elem_size, pri_queue_top_pri(heap), arr_elem_size);
        pri_queue_pop(heap);
    }

    free_priority_queue(heap);
}