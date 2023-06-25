#include "../../src/m_list.h"
#include <stdio.h>
#include <time.h>

MLIST_ALL(test, int)

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_3.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  printf("In this example we will try to do different operations on "
         "linked lists and measure the time\n\n");

  /* Init the working strucutres */
  double exec_time = 0.0;

  test_mlist_t ll = test_mlist(&compare_int, NULL);
  /* End to init the working structures */

  /* Insert 100 ints into the list and count the time */
  clock_t begin = clock();

  for (int i = 0; i < 100; ++i) {
    test_mlist_push(ll, i);
  }

  clock_t end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100 ints into list: %lf sec\n", exec_time);
  /* End to insert 100 ints into the list */

  /* Delete 100 heads from list and count the time */
  begin = clock();

  for (int i = 0; i < 100; ++i) {
    test_mlist_pop(ll, ll->head->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100 ints heads from list: %lf sec\n", exec_time);
  /* End to delete 100 heads from list */

  /* Free list with size of 100 and count time */
  for (int i = 0; i < 100; ++i) {
    test_mlist_push(ll, i);
  }

  begin = clock();

  test_mlist_free(&ll);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing list with 100 ints: %lf sec\n\n", exec_time);

  /* End to free list with size of 100 */

  ////////////////////////////////////////////////////////////// Increase size 1

  ll = test_mlist(&compare_int, NULL);

  /* Insert 100000 ints into the list and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    test_mlist_push(ll, i);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 100000 ints into list: %lf sec\n", exec_time);
  /* End to insert 100000 ints into the list */

  /* Delete 100000 heads from list and count the time */
  begin = clock();

  for (int i = 0; i < 100000; ++i) {
    test_mlist_pop(ll, ll->head->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 100000 ints heads from list: %lf sec\n", exec_time);
  /* End to delete 100000 heads from list */

  /* Free list with size of 100000 and count time */
  for (int i = 0; i < 100000; ++i) {
    test_mlist_push(ll, i);
  }

  begin = clock();

  test_mlist_free(&ll);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing list with 100000 ints: %lf sec\n\n", exec_time);

  /* End to free list with size of 100000 */

  ////////////////////////////////////////////////////////////// Increase size 2

  ll = test_mlist(&compare_int, NULL);

  /* Insert 8000000 ints into the list and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    test_mlist_push(ll, i);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Inserting 8000000 ints into list: %lf sec\n", exec_time);
  /* End to insert 8000000 ints into the list */

  /* Delete 8000000 heads from list and count the time */
  begin = clock();

  for (int i = 0; i < 8000000; ++i) {
    test_mlist_pop(ll, ll->head->data);
  }

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Deleting 8000000 ints heads from list: %lf sec\n", exec_time);
  /* End to delete 8000000 heads from list */

  /* Free list with size of 8000000 and count time */
  for (int i = 0; i < 8000000; ++i) {
    test_mlist_push(ll, i);
  }

  begin = clock();

  test_mlist_free(&ll);

  end = clock();

  exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Freeing list with 8000000 ints: %lf sec\n", exec_time);

  /* End to free list with size of 8000000 */

  /* Close output file */
  fclose(fout);

  return 0;
}