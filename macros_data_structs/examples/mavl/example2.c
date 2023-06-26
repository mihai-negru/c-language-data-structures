#include "../../src/m_avl.h"
#include <stdio.h>
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

int32_t compare_student(const student_t *const s1, const student_t *const s2) {
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

void free_student(student_t *st) {
  if (NULL != st) {

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

void print_student(const student_t *const st) {
  if (NULL != st) {
    printf("Student no. %d\n", st->index);
    printf("NAME: %s\n", st->name);
    printf("SURNAME: %s\n", st->surname);
    printf("Student GPA: %lf\n", st->GPA);
    printf("Student age: %u\n\n", st->age);
  }
}

MAVL_ALL(test, student_t)

int main(void) {
  FILE *fin = NULL;

  if ((fin = freopen("example2.in", "r", stdin)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mavl_t data_base = test_mavl(&compare_student, &free_student);

  if (NULL == data_base) {
    exit(EXIT_FAILURE);
  }

  merr_t err = M_OK;

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

    err = test_mavl_push(data_base, st);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  fclose(fin);

  FILE *fout = NULL;

  if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  printf("In the data base are %lu students\n\n", test_mavl_size(data_base));

  test_mavl_traverse_inorder(data_base, &print_student);
  printf("\n\n");

  fclose(fout);

  test_mavl_free(&data_base);

  return 0;
}
