#include "../../src/m_queue.h"
#include <time.h>

MQUEUE_ALL(inner, int)

MQUEUE_ALL(test, inner_mqueue_t)

void free_inner(inner_mqueue_t *q) {
  if (q != NULL) {
    inner_mqueue_free(q);
  }
}

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mqueue_t qqs = test_mqueue(&free_inner);

  if (NULL != qqs) {
    srand(time(NULL));

    merr_t err = M_OK;

    for (int i = 0; i < 3; ++i) {
      inner_mqueue_t qq = inner_mqueue(NULL);

      for (int j = 0; j < 10; ++j) {
        err = inner_mqueue_push(qq, rand() % 100 + 10);

        if (M_OK != err) {
          MERROR(err);
        }
      }

      err = test_mqueue_push(qqs, qq);

      if (M_OK != err) {
        MERROR(err);
      }
    }

    int queue_num = 0;

    while (!test_mqueue_empty(qqs)) {
      printf("Queue number %d:\n", queue_num++);

      inner_mqueue_t front = NULL;
      test_mqueue_front(qqs, &front);

      while (!inner_mqueue_empty(front)) {
        int front_elem = 0;
        inner_mqueue_front(front, &front_elem);

        printf("%d ", front_elem);

        err = inner_mqueue_pop(front);

        if (M_OK != err) {
          MERROR(err);
        }
      }

      printf("\n\n");

      err = test_mqueue_pop(qqs);

      if (M_OK != err) {
        MERROR(err);
      }
    }

    test_mqueue_free(&qqs);
  }

  fclose(fout);

  return 0;
}
