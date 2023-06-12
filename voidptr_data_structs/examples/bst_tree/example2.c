#include <scl_datastruc.h>
#include <time.h>

#define MAX_NAME 20
#define MAX_SURNAME 30

typedef struct student_s {
    char *name;
    char *surname;
    unsigned int age;
    unsigned int class;
    unsigned char class_label;
    double GPA;
    int index;
} student_t;

int32_t compare_student(const void * const elem1, const void * const elem2) {
    const student_t * const s1 = elem1;
    const student_t * const s2 = elem2;

    int cmp = strncmp(s1->name, s2->name, MAX_NAME);

    if (0 != cmp) {
        return cmp;
    }

    cmp = strncmp(s1->surname, s2->surname, MAX_SURNAME);

    if (0 != cmp) {
        return cmp;
    }

    if (s1->GPA > s2->GPA) {
        cmp = 1;
    } else if (s1->GPA < s2->GPA) {
        cmp = -1;
    } else {
        cmp = 0;
    }

    if (cmp == 0) {
        return s1->index - s2->index;
    }

    return cmp;
}

void free_student(void * const elem) {
    if (NULL != elem) {
        student_t * const st = elem;

        free(st->name);
        st->name = NULL;

        free(st->surname);
        st->surname = NULL;

        st->age = 0;
        st->class = 0;
        st->class_label = '\0';
        st->GPA = 0.0;
        st->index = -1;
    }
}

void print_student(void * const elem) {
    if (NULL != elem) {
        const student_t * const st = elem;

        printf("Student no. %d\n", st->index);
        printf("NAME: %s\n", st->name);
        printf("SURNAME: %s\n", st->surname);
        printf("Student GPA: %lf\n", st->GPA);
        printf("Student age: %u\n\n", st->age);
    }
}

void two_years_passed(void * const elem) {
    if (NULL != elem) {
        student_t * const st = elem;

        st->age = st->age + 2;

        print_student(st);
    }
}

int main(void) {
    FILE *fin = NULL;

    if ((fin = freopen("example2.in", "r", stdin)) == NULL) {
        exit(EXIT_FAILURE);
    }

    bst_tree_t *data_base = create_bst(&compare_student, &free_student, sizeof(student_t));

    if (NULL == data_base) {
        exit(EXIT_FAILURE);
    }

    scl_error_t err = SCL_OK;


    int number_of_students = 0;
    int read = scanf("%d", &number_of_students);

    if (read > 1) {
        exit(EXIT_FAILURE);
    }

    student_t st; // Encapsulate the reading

    for (int i = 0; i < number_of_students; ++i) {
        st.name = malloc(MAX_NAME);
        st.surname = malloc(MAX_SURNAME);

        read = scanf("%s %s", st.name, st.surname);

        if (read > 2) {
            exit(EXIT_FAILURE);
        }

        read = scanf("%u %u %c %lf", &st.age, &st.class, &st.class_label, &st.GPA);

        if (read > 4) {
            exit(EXIT_FAILURE);
        }

        st.index = i;

        err = bst_insert(data_base, toptr(st));

        if (SCL_OK != err) {
            scl_error_message(err);
        }
    }

    fclose(fin);

    FILE *fout = NULL;

    if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("In the data base are %lu students\n\n", get_bst_size(data_base));

    bst_traverse_inorder(data_base, &print_student);
    printf("\n\n");

    printf("After two years our students got older so let's change their age\n\n");
    bst_traverse_inorder(data_base, &two_years_passed);    

    fclose(fout);

    free_bst(data_base);

    return 0;
}