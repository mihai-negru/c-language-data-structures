#include "../../src/m_stack.h"
#include <time.h>

MSTACK_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  printf("In this example we will try to do different operations on stacks and "
         "measure the time\n\n");

  /* Init the working strucutres */
  double exec_time = 0.0;

  merr_t err = M_OK;

  test_mstack_t qq = test_mstack(NULL);
  /* End to init the working structures */

  /* Insert 100 ints into the stack and count the time */
  clock_t begin = clock();

  for (int i = 0; i < 100; ++i) {
    err = test_mstack_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  clock_t end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100 ints into stack: %lf sec\n", exec_time);
  /* End to insert 100 ints into the stack */

  /* Delete 100 fronts from stack and count the time */
  begin = clock();

  for (int i = 0; i < 100; ++i) {
    err = test_mstack_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100 ints fronts from stack: %lf sec\n", exec_time);
  /* End to delete 100 fronts from stack */

  /* Free stack with size of 100 and count time */
  for (int i = 0; i < 100; ++i) {
    test_mstack_push(qq, i);
  }

  begin = clock();

  test_mstack_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing stack with 100 ints: %lf sec\n\n", exec_time);

  /* End to free stack with size of 100 */

  ////////////////////////////////////////////////////////////// Increase size 1

  qq = test_mstack(NULL);

  /* Insert 100000 ints into the stack and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    err = test_mstack_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100000 ints into stack: %lf sec\n", exec_time);
  /* End to insert 100000 ints into the stack */

  /* Delete 100000 fronts from stack and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    err = test_mstack_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100000 ints fronts from stack: %lf sec\n", exec_time);
  /* End to delete 100000 fronts from stack */

  /* Free stack with size of 100000 and count time */
  for (int i = 0; i < 100000; ++i) {
    test_mstack_push(qq, i);
  }

  begin = clock();

  test_mstack_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing stack with 100000 ints: %lf sec\n\n", exec_time);

  /* End to free stack with size of 100000 */

  ////////////////////////////////////////////////////////////// Increase size 2

  qq = test_mstack(NULL);

  /* Insert 8000000 ints into the stack and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    err = test_mstack_push(qq, i);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 8000000 ints into stack: %lf sec\n", exec_time);
  /* End to insert 8000000 ints into the stack */

  /* Delete 8000000 fronts from stack and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    err = test_mstack_pop(qq);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 8000000 ints fronts from stack: %lf sec\n", exec_time);
  /* End to delete 8000000 fronts from stack */

  /* Free stack with size of 8000000 and count time */
  for (int i = 0; i < 8000000; ++i) {
    test_mstack_push(qq, i);
  }

  begin = clock();

  test_mstack_free(&qq);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing stack with 8000000 ints: %lf sec\n", exec_time);

  /* End to free stack with size of 8000000 */

  /* Close output file */
  fclose(fout);

  return 0;
}