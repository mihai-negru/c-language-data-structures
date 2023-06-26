#include "../../src/m_rbk.h"
#include <stdio.h>
#include <time.h>

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

void print_int(const int *const a) { printf(" %d", *a); }

MRBK_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mrbk_t my_tree = test_mrbk(&compare_int, NULL);

  if (NULL == my_tree) {
    printf("RBK tree was not allocated\n");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));

  merr_t err = M_OK;

  size_t number_of_elem = 100;

  printf("Generated data:\n");
  for (size_t i = 1; i <= number_of_elem; ++i) {
    if (i % 10 == 0) {
      printf("\n");
    }

    int data = rand() % 2000 + 100;

    printf("%d ", data);

    err = test_mrbk_push(my_tree, data);

    if (M_OK != err) {
      MERROR(err);
    }
  }
  printf("\n");

  if (test_mrbk_empty(my_tree) == mtrue) {
    printf("Something went really wrong try once again\n");
    exit(EXIT_FAILURE);
  }

  printf("\nThe RBK tree printed by postorder method is:\n");
  test_mrbk_traverse_postorder(my_tree, &print_int);
  printf("\n\n");

  printf("In the RBK tree there exist %lu unique random integers\n\n",
         test_mrbk_size(my_tree));

  int root_data;
  err = test_mrbk_root(my_tree, &root_data);

  if (err == M_OK) {
    printf("The root value of the RBK tree is %d\n", root_data);

    int min_max = 0;
    test_mrbk_max(my_tree, root_data, &min_max);
    printf("The maximum random number generated is %d\n", min_max);

    test_mrbk_min(my_tree, root_data, &min_max);
    printf("The minimum random number generated is %d\n", min_max);

    int root_pred, root_succ;

    if (test_mrbk_pred(my_tree, root_data, &root_pred) == M_OK) {
      printf("The inorder predecessor of the root value is %d\n", root_pred);
    } else {
      printf("The root node has no predecessor\n");
    }

    if (test_mrbk_succ(my_tree, root_data, &root_succ) == M_OK) {
      printf("The inorder successor of the root value is %d\n\n", root_succ);
    } else {
      printf("The root node has no successor\n\n");
    }

    int anc;
    if (test_mrbk_lca(my_tree, root_pred, root_succ, &anc) == M_OK) {
      printf("The lowest common ancestor for the predecessor and successor of "
             "the root is %d\n\n",
             anc);
    } else {
      printf("Could not find the lowest comon ancestor for predecessor and "
             "successor of the root node\n\n");
    }
  } else {
    printf("Could not find the root node\n\n");
  }

  printf("Let's remove some elements:\n");

  for (size_t i = 0; i < 10; ++i) {
    int del;

    if (test_mrbk_root(my_tree, &del) == M_OK) {
      printf("Removing %d value from RBK\n", del);

      err = test_mrbk_pop(my_tree, del);

      if (err != M_OK) {
        MERROR(err);
      }
    }
  }

  printf("\n");

  printf("After deletion the RBK tree shows like (inorder):\n");
  test_mrbk_traverse_inorder(my_tree, &print_int);
  printf("\n\n");

  fclose(fout);
  test_mrbk_free(&my_tree);

  return 0;
}
