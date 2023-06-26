#include "../../src/m_pqueue.h"

#define MAX_ELEMENTS 100
#define MAX_STRING 100

typedef char *string;

MPQUEUE_ALL(test, int, string)

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

int32_t compare_string(const string *const str1, const string *const str2) {
  return strncmp(*str1, *str2, MAX_STRING);
}

void free_string(string *str) {
  if (NULL != str) {
    free(*str);
  }
}

void print_string(const string *const str) {
  if (str != NULL) {
    printf("%s, ", *str);
  } else {
    printf("Not ok\n");
  }
}

int32_t compute_voc(string str) {
  int32_t voc_diff = 0;

  for (size_t iter = 0; str[iter] != 0; ++iter) {
    if (('a' == str[iter]) || ('e' == str[iter]) || ('i' == str[iter]) ||
        ('o' == str[iter]) || ('u' == str[iter]) || ('A' == str[iter]) ||
        ('E' == str[iter]) || ('I' == str[iter]) || ('O' == str[iter]) ||
        ('U' == str[iter])) {
      ++voc_diff;
    }
  }

  return voc_diff;
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

  test_mpqueue_t pq = test_mpqueue(MAX_ELEMENTS, &compare_int, NULL,
                                   &compare_string, &free_string);

  int elem_num = 0;

  int check = scanf("%d", &elem_num);

  if (check > 1) {
    exit(EXIT_FAILURE);
  }

  merr_t err = M_OK;

  for (int i = 0; i < elem_num; ++i) {
    string read_data = malloc(MAX_STRING);

    check = scanf("%s", read_data);

    if (check > 1) {
      exit(EXIT_FAILURE);
    }

    err = test_mpqueue_push(pq, compute_voc(read_data), read_data);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  printf("Priority queue data:\n");
  test_mpqueue_traverse(pq, &print_string);
  printf("\n\n");

  string acc = NULL;

  if (test_mpqueue_top(pq, &acc) == M_OK) {
    int voc = -1;

    test_mpqueue_top_pri(pq, &voc);

    printf("The top elements is \"%s\" and has %d vowels:\n\n", acc, voc);
  }

  printf("Let's remove the top element:\n");
  err = test_mpqueue_pop(pq);

  if (M_OK != err) {
    MERROR(err);
  }

  test_mpqueue_traverse(pq, &print_string);
  printf("\n\n");

  printf("Let's change now the top element into \"!!!!HELLO!!!!\" and also to"
         " change its priority\n");

  string chg = malloc(MAX_STRING);
  strcpy(chg, "communication");

  size_t top_index = 0;
  test_mpqueue_find_idx(pq, chg, &top_index);

  strcpy(chg, "!!!!HELLO!!!!");
  err = test_mpqueue_change(pq, top_index, chg);

  if (M_OK != err) {
    MERROR(err);
  }

  err = test_mpqueue_change_pri(pq, top_index, compute_voc(chg));

  if (M_OK != err) {
    MERROR(err);
  }

  test_mpqueue_traverse(pq, &print_string);
  printf("\n\n");

  printf("Let's pop 10 elems from priority queue:\n");
  for (size_t i = 0; i < 10; ++i) {
    err = test_mpqueue_pop(pq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  test_mpqueue_traverse(pq, &print_string);
  fclose(fin);

  test_mpqueue_free(&pq);

  fclose(fout);

  return 0;
}