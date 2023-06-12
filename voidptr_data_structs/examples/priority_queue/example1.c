#include <scl_datastruc.h>

#define MAX_ELEMENTS 100
#define MAX_STRING 100

int32_t compute_voc(const char * const str) {
    int32_t voc_diff = 0;
    
    for (size_t iter = 0; str[iter] != 0; ++iter) {
        if (('a' == str[iter]) || 
            ('e' == str[iter]) ||
            ('i' == str[iter]) ||
            ('o' == str[iter]) ||
            ('u' == str[iter]) ||
            ('A' == str[iter]) ||
            ('E' == str[iter]) ||
            ('I' == str[iter]) ||
            ('O' == str[iter]) ||
            ('U' == str[iter])) {
                ++voc_diff;
        }
    }

    return voc_diff;
}

void capitalize(void * const data) {
    if (NULL != data) {
        char * const t_d = data;

        if ((t_d[0] >= 97) && (t_d[0] <= 122)) {
            t_d[0] -= 32;
        } 
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    FILE *fin = NULL;

    if ((fin = freopen("example1.in", "r", stdin)) == NULL) {
        fclose(fout);
        exit(EXIT_FAILURE);
    }

    priority_queue_t *pq = create_priority_queue(MAX_ELEMENTS, &compare_int, &compare_string, NULL, NULL, sizeof(int), MAX_STRING);

    int elem_num = 0; 
    
    int check = scanf("%d", &elem_num);

    if (check > 1) {
        exit(EXIT_FAILURE);
    }

    
    scl_error_t err = SCL_OK;

    for (int i = 0; i < elem_num; ++i) {
        char read_data[MAX_STRING];

        check = scanf("%s", read_data);

        if (check > 1) {
            exit(EXIT_FAILURE);
        }    

        err = pri_queue_push(pq, ltoptr(int32_t, compute_voc(read_data)), read_data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    printf("Priority queue data:\n");
    pri_queue_traverse(pq, &print_string);
    printf("\n\n");

    const char *top = pri_queue_top(pq);
    
    if (NULL != top) {
        const int *voc = pri_queue_top_pri(pq);

        printf("The top elements is \"%s\" and has %d vowels:\n\n", top, *voc);
    }

    printf("Let's remove the top element:\n");
    err = pri_queue_pop(pq);

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    pri_queue_traverse(pq, &print_string);
    printf("\n\n");

    printf("Let's change now the top element into \"!!!!HELLO!!!!\" and also to change it's priority\n");

    size_t top_index = pri_find_data_index(pq, "communication");

    err = change_node_data(pq, top_index, "!!!!HELLO!!!!");

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    err = change_node_priority(pq, top_index, ltoptr(int32_t, compute_voc("!!!!HELLO!!!!")));

    if (SCL_OK != err) {
        scl_error_message(err);
    }

    pri_queue_traverse(pq, &print_string);
    printf("\n\n");

    printf("Let's pop 10 elems from priority queue:\n");
    for (size_t i = 0; i < 10; ++i) {
        err = pri_queue_pop(pq);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    pri_queue_traverse(pq, &print_string);
    printf("\n\n");

    printf("For the remaining strings let's capitalize them\n");
    pri_queue_traverse(pq, &capitalize);
    pri_queue_traverse(pq, &print_string);
    printf("\n");

    fclose(fin);

    free_priority_queue(pq);

    fclose(fout);

    return 0;
}