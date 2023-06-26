#include "../../src/m_dlist.h"
#include <stdio.h>
#include <time.h>

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

void print_int(const int *const a) { printf(" %d", *a); }

mbool_t fil(const int *const data) {
  if (NULL != data) {
    return (*data % 2) == 1;
  }

  return 0;
}

int map(const int *const data) { return (*data) * (*data); }

MDLIST_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mdlist_t ll = test_mdlist(&compare_int, NULL);

  if (NULL == ll) {
    printf("Something went wrong on creation\n");
    fclose(fout);

    exit(EXIT_FAILURE);
  }

  srand(time(NULL));

  merr_t err = M_OK;

  printf("Inserting some random numbers:\n");
  for (int i = 0; i < 10; ++i) {
    int data = rand() % 100 + 10;

    err = test_mdlist_push(ll, data);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  printf("Inserting some random numbers in fornt of the dlist:\n");
  for (int i = 0; i < 10; ++i) {
    int data = rand() % 100 + 10;

    err = test_mdlist_push_front(ll, data);

    if (err != M_OK) {
      MERROR(err);
    }
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  printf("Inserting some random number in order:\n");
  for (int i = 0; i < 10; ++i) {
    int data = rand() % 100 + 10;

    err = test_mdlist_push_order(ll, data);

    if (err != M_OK) {
      MERROR(err);
    }
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  printf("Inserting some random number at different positions:\n");
  for (int i = 0; i < 10; ++i) {
    int data = rand() % 100 + 10;
    size_t index = rand() % test_mdlist_size(ll);

    err = test_mdlist_push_idx(ll, data, index);

    if (err != M_OK) {
      MERROR(err);
    }
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  if (mtrue == test_mdlist_empty(ll)) {
    printf("Something went wrong\n");
    test_mdlist_free(&ll);
    fclose(fout);

    exit(EXIT_FAILURE);
  }

  int head, tail;

  if (test_mdlist_head(ll, &head) == M_OK) {
    printf("The head of the dlist is: %d\n", head);
  } else {
    printf("The head could not be fetched\n");
  }

  if (test_mdlist_tail(ll, &tail) == M_OK) {
    printf("The tail of the dlist is: %d\n\n", tail);
  } else {
    printf("The tail could not be fetched\n\n");
  }

  printf("Let's swap head with te tail:\n");
  err = test_mdlist_swap(ll, head, tail);

  if (M_OK != err) {
    MERROR(err);
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  printf("Now we will erase a good chunk from dlist from [10, 40] range:\n");
  err = test_mdlist_erase(ll, 10, 20);

  if (err != M_OK) {
    MERROR(err);
  }

  test_mdlist_traverse(ll, &print_int);
  printf("\n\n");

  int find1 = rand() % 100 + 10;
  int find2 = rand() % 100 + 10;

  printf("I want to find %d and %d in the dlist:\n", find1, find2);

  if (test_mdlist_find(ll, find1, NULL) == M_OK) {
    printf("I found %d\n", find1);
  } else {
    printf("I did not find the %d value in current dlist\n", find1);
  }

  if (test_mdlist_find(ll, find2, NULL) == M_OK) {
    printf("I found %d\n\n", find2);
  } else {
    printf("I did not find the %d value in current dlist\n\n", find2);
  }

  if (test_mdlist_find_idx(ll, 4, &find1) == M_OK) {
    printf("Data element from index 4 is %d\n", find1);
  } else {
    printf("Could not fetch index 4's data\n");
  }

  if (test_mdlist_find_idx(ll, 420, &find2) == M_OK) {
    printf("Data element from index 420 is %d\n", find2);
  } else {
    printf("Could not fetch index 420's data\n\n");
  }

  test_mdlist_t odd_dlist = test_mdlist_filter(ll, &fil);

  printf("Created a new dlist with just odd elements:\n");
  test_mdlist_traverse(odd_dlist, &print_int);
  printf("\n\n");

  test_mdlist_t square =
      test_to_test_mdlist_map(odd_dlist, &map, &compare_int, NULL);
  printf("Let's find the square number of odd elements:\n");
  test_mdlist_traverse(square, &print_int);
  printf("\n");

  // As seen here is used one traverse call function to modify and to print the
  // values

  test_mdlist_free(&ll);
  test_mdlist_free(&odd_dlist);
  test_mdlist_free(&square);

  fclose(fout);

  return 0;
}
