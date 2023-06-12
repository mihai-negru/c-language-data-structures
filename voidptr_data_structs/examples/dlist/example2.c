#include <scl_datastruc.h>

#define MAX_STRING_SIZE 100

int32_t compare_ptr_str(const void * const elem1, const void * const elem2) {
    const char * const * const f1 = elem1;
    const char * const * const f2 = elem2;

    return strncmp(*f1, *f2, MAX_STRING_SIZE);
}

void free_str(void * const data) {
    if (NULL != data) {
        char ** const t_d = data;
        free(*t_d);
        *t_d = '\0';
    }
}

void print_str(void * const data) {
    if (NULL != data) {
        const char * const * const t_d = data;

        printf("%s ", *t_d);
    }
}

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    FILE *fin = NULL;

    if ((fin = freopen("example2.in", "r", stdin)) == NULL) {
        printf("Could not open for reading\n");
        exit(EXIT_FAILURE);
    }

    dlist_t *strings = create_dlist(&compare_string, NULL, MAX_STRING_SIZE); 

    size_t str_num = 0;
    int check = 0;

    check = scanf("%lu", &str_num);

    if (check > 1) {
        exit(EXIT_FAILURE);
    }

    /////////////////////////////// Method 1 to work with strings

    char data[MAX_STRING_SIZE];
    scl_error_t err = SCL_OK;

    for (size_t i = 0; i < str_num; ++i) {
        check = scanf("%s", data);

        if (check > 1) {
            exit(EXIT_FAILURE);
        }

        err = dlist_insert(strings, data);

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    //////////////////////////////// Method 2 to work with strings

    fseek(fin, 0, SEEK_SET);

    dlist_t *strings_2 = create_dlist(&compare_ptr_str, &free_str, sizeof(char *)); 

    str_num = 0;
    check = 0;

    check = scanf("%lu", &str_num);

    if (check > 1) {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < str_num; ++i) {
        char *ptr_data = malloc(MAX_STRING_SIZE);
        check = scanf("%s", ptr_data);

        if (check > 1) {
            exit(EXIT_FAILURE);
        }

        err = dlist_insert(strings_2, toptr(ptr_data));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    fclose(fin);

    printf("Print strings dlist by first method:\n");
    dlist_traverse(strings, &print_string);
    printf("\n\n");

    printf("Print strings dlist by second method:\n");
    dlist_traverse(strings_2, &print_str);
    printf("\n");

    char * const * find_1 = dlist_find_index(strings_2, 4);

    if (NULL != find_1) {
        printf("String from pos 4 is %s\n", *find_1);
    }

    free_dlist(strings);
    free_dlist(strings_2);

    fclose(fin);
    fclose(fout);

    return 0;
}