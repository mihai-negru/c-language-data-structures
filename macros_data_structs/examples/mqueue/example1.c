#include "../../src/m_queue.h"
#include <time.h>

MQUEUE_ALL(test, int)

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

  test_mqueue_t qq = test_mqueue(NULL);

  if (NULL == qq) {
    exit(EXIT_FAILURE);
  }

  merr_t err = M_OK;

  for (int i = 0; i < 100; ++i) {
    err = test_mqueue_push(qq, rand() % 100 + 10);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  if (mtrue == test_mqueue_empty(qq)) {
    printf("Oops !!! Something went wring with insertion\n");
    exit(EXIT_FAILURE);
  }

  printf("Generated queue has %lu elements:\n", test_mqueue_size(qq));
  test_mqueue_traverse(qq, &print_int_br);
  printf("\n");

  int front = 0, back = 0;

  if (test_mqueue_front(qq, &front) == M_OK) {
    printf("Front element is %d\n", front);
  } else {
    printf("Could not fetch front element\n");
  }

  if (test_mqueue_back(qq, &back) == M_OK) {
    printf("Back element is %d\n\n", back);
  } else {
    printf("Could not fetch back element\n\n");
  }

  printf("Let's pop half of the queue:\n");

  for (int i = 0; i < 50; ++i) {
    err = test_mqueue_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  test_mqueue_traverse(qq, &print_int_br);
  printf("\n");

  test_mqueue_free(&qq);

  fclose(fout);

  return 0;
}
