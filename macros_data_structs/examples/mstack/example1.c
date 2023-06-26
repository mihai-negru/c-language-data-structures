#include "../../src/m_stack.h"
#include <time.h>

MSTACK_ALL(test, int)

void print_int_br(const int *const data) {
  if (NULL != data) {
    static int br = 0;

    if ((br % 10 == 0) && (0 != br)) {
      printf("\n");
    }

    printf("%d ", *data);

    ++br;
  }
}

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));

  test_mstack_t st = test_mstack(NULL);

  if (NULL == st) {
    exit(EXIT_FAILURE);
  }

  merr_t err = M_OK;

  for (int i = 0; i < 100; ++i) {
    err = test_mstack_push(st, rand() % 100 + 10);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  if (mtrue == test_mstack_empty(st)) {
    printf("Oops !!! Something went wring with insertion\n");
    exit(EXIT_FAILURE);
  }

  printf("Generated queue has %lu elements:\n", test_mstack_size(st));
  test_mstack_traverse(st, &print_int_br);
  printf("\n");

  int top = 0;

  if (test_mstack_top(st, &top) == M_OK) {
    printf("Top element is %d\n\n", top);
  } else {
    printf("Could not fetch top element\n\n");
  }

  printf("Let's pop half of the queue:\n");

  for (int i = 0; i < 50; ++i) {
    err = test_mstack_pop(st);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  test_mstack_traverse(st, &print_int_br);
  printf("\n");

  test_mstack_free(&st);

  fclose(fout);

  return 0;
}
