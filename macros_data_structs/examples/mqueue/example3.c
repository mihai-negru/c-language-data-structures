#include "../../src/m_queue.h"
#include <time.h>

MQUEUE_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  printf("In this example we will try to do different operations on Queues and "
         "measure the time\n\n");

  /* Init the working strucutres */
  double exec_time = 0.0;

  merr_t err = M_OK;

  test_mqueue_t qq = test_mqueue(NULL);
  /* End to init the working structures */

  /* Insert 100 ints into the queue and count the time */
  clock_t begin = clock();

  for (int i = 0; i < 100; ++i) {
    err = test_mqueue_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  clock_t end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100 ints into queue: %lf sec\n", exec_time);
  /* End to insert 100 ints into the queue */

  /* Delete 100 fronts from queue and count the time */
  begin = clock();

  for (int i = 0; i < 100; ++i) {
    err = test_mqueue_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100 ints fronts from queue: %lf sec\n", exec_time);
  /* End to delete 100 fronts from queue */

  /* Free queue with size of 100 and count time */
  for (int i = 0; i < 100; ++i) {
    test_mqueue_push(qq, i);
  }

  begin = clock();

  test_mqueue_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing queue with 100 ints: %lf sec\n\n", exec_time);

  /* End to free queue with size of 100 */

  ////////////////////////////////////////////////////////////// Increase size 1

  qq = test_mqueue(NULL);

  /* Insert 100000 ints into the queue and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    err = test_mqueue_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100000 ints into queue: %lf sec\n", exec_time);
  /* End to insert 100000 ints into the queue */

  /* Delete 100000 fronts from queue and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    err = test_mqueue_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100000 ints fronts from queue: %lf sec\n", exec_time);
  /* End to delete 100000 fronts from queue */

  /* Free queue with size of 100000 and count time */
  for (int i = 0; i < 100000; ++i) {
    test_mqueue_push(qq, i);
  }

  begin = clock();

  test_mqueue_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing queue with 100000 ints: %lf sec\n\n", exec_time);

  /* End to free queue with size of 100000 */

  ////////////////////////////////////////////////////////////// Increase size 2

  qq = test_mqueue(NULL);

  /* Insert 8000000 ints into the queue and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    err = test_mqueue_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 8000000 ints into queue: %lf sec\n", exec_time);
  /* End to insert 8000000 ints into the queue */

  /* Delete 8000000 fronts from queue and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    err = test_mqueue_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 8000000 ints fronts from queue: %lf sec\n", exec_time);
  /* End to delete 8000000 fronts from queue */

  /* Free queue with size of 8000000 and count time */
  for (int i = 0; i < 8000000; ++i) {
    test_mqueue_push(qq, i);
  }

  begin = clock();

  test_mqueue_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing queue with 8000000 ints: %lf sec\n", exec_time);

  /* End to free queue with size of 8000000 */

  /* Close output file */
  fclose(fout);

  return 0;
}