#include "../../src/m_rbk.h"
#include <stdio.h>
#include <time.h>

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

MRBK_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  printf("In this example we will try to do different operations on RBK Tree "
         "and measure the time\n");

  /* Init the working strucutres */
  double exec_time = 0.0;

  test_mrbk_t my_tree = test_mrbk(&compare_int, NULL);
  /* End to init the working structures */

  /* Insert 100 ints into the RBK and count the time */
  clock_t begin = clock();

  for (int i = 0; i < 100; ++i) {
    test_mrbk_push(my_tree, i);
  }

  clock_t end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100 ints into RBK: %lf sec\n", exec_time);
  /* End to insert 100 ints into the RBK */

  /* Delete 100 roots from RBK and count the time */
  begin = clock();

  for (int i = 0; i < 100; ++i) {
    test_mrbk_pop(my_tree, my_tree->root->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100 ints roots from RBK: %lf sec\n", exec_time);
  /* End to delete 100 roots from RBK */

  /* Free RBK tree with size of 100 and count time */
  for (int i = 0; i < 100; ++i) {
    test_mrbk_push(my_tree, i);
  }

  begin = clock();

  test_mrbk_free(&my_tree);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing RBK with 100 ints: %lf sec\n\n", exec_time);

  /* End to free RBK tree with size of 100 */

  ////////////////////////////////////////////////////////////// Increase size 1

  my_tree = test_mrbk(&compare_int, NULL);

  /* Insert 100000 ints into the RBK and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    test_mrbk_push(my_tree, i);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100000 ints into RBK: %lf sec\n", exec_time);
  /* End to insert 100000 ints into the RBK */

  /* Delete 100000 roots from RBK and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    test_mrbk_pop(my_tree, my_tree->root->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100000 ints roots from RBK: %lf sec\n", exec_time);
  /* End to delete 100000 roots from RBK */

  /* Free RBK tree with size of 100000 and count time */
  for (int i = 0; i < 100000; ++i) {
    test_mrbk_push(my_tree, i);
  }

  begin = clock();

  test_mrbk_free(&my_tree);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing RBK with 100000 ints: %lf sec\n\n", exec_time);

  /* End to free RBK tree with size of 100000 */

  ////////////////////////////////////////////////////////////// Increase size 2

  my_tree = test_mrbk(&compare_int, NULL);

  /* Insert 8000000 ints into the RBK and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    test_mrbk_push(my_tree, i);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 8000000 ints into RBK: %lf sec\n", exec_time);
  /* End to insert 8000000 ints into the RBK */

  /* Delete 8000000 roots from RBK and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    test_mrbk_pop(my_tree, my_tree->root->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 8000000 ints roots from RBK: %lf sec\n", exec_time);
  /* End to delete 8000000 roots from RBK */

  /* Free RBK tree with size of 8000000 and count time */
  for (int i = 0; i < 8000000; ++i) {
    test_mrbk_push(my_tree, i);
  }

  begin = clock();

  test_mrbk_free(&my_tree);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing RBK with 8000000 ints: %lf sec\n", exec_time);

  /* End to free RBK tree with size of 8000000 */

  /* Close output file */
  fclose(fout);

  return 0;
}