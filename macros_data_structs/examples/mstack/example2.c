#include "../../src/m_stack.h"
#include <time.h>

MSTACK_ALL(inner, int)

MSTACK_ALL(test, inner_mstack_t)

void free_inner(inner_mstack_t *st) {
  if (st != NULL) {
    inner_mstack_free(st);
  }
}

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mstack_t sts = test_mstack(
      &free_inner); // Can be NULL however you should take care of freeing

  if (NULL != sts) {
    srand(time(NULL));

    merr_t err = M_OK;

    for (int i = 0; i < 3; ++i) {
      inner_mstack_t st = inner_mstack(NULL);

      for (int j = 0; j < 10; ++j) {
        err = inner_mstack_push(st, rand() % 100 + 10);

        if (M_OK != err) {
          MERROR(err);
        }
      }

      err = test_mstack_push(sts, st);

      if (M_OK != err) {
        MERROR(err);
      }
    }

    int queue_num = 0;

    while (!test_mstack_empty(sts)) {
      printf("Stack number %d:\n", queue_num++);

      inner_mstack_t top = NULL;
      test_mstack_top(sts, &top);

      while (!inner_mstack_empty(top)) {
        int top_elem = 0;
        inner_mstack_top(top, &top_elem);

        printf("%d ", top_elem);

        err = inner_mstack_pop(top);

        if (M_OK != err) {
          MERROR(err);
        }
      }

      printf("\n\n");

      err = test_mstack_pop(sts);

      if (M_OK != err) {
        MERROR(err);
      }

      // inner_mstack_free(&top);If you set the free_inner to NULL you should
      // take care of freeing
    }

    test_mstack_free(&sts);
  }

  fclose(fout);

  return 0;
}
