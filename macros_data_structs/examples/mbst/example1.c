#include "../../src/m_bst.h"
#include <stdio.h>
#include <time.h>

int32_t compare_int(const int *const a, const int *const b) { return *a - *b; }

void print_int(const int *const a) { printf(" %d", *a); }

MBST_ALL(test, int)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  test_mbst_t my_tree = test_mbst(&compare_int, NULL);

  if (NULL == my_tree) {
    printf("BST tree was not allocated\n");
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

    err = test_mbst_push(my_tree, data);

    if (M_OK != err) {
      MERROR(err);
    }
  }
  printf("\n");

  if (test_mbst_empty(my_tree) == mtrue) {
    printf("Something went really wrong try once again\n");
    exit(EXIT_FAILURE);
  }

  printf("\nThe BST tree printed by postorder method is:\n");
  test_mbst_traverse_postorder(my_tree, &print_int);
  printf("\n\n");

  printf("In the BST tree there exist %lu unique random integers\n\n",
         test_mbst_size(my_tree));

  int root_data;
  err = test_mbst_root(my_tree, &root_data);

  if (err == M_OK) {
    printf("The root value of the BST tree is %d\n", root_data);

    int min_max = 0;
    test_mbst_max(my_tree, root_data, &min_max);
    printf("The maximum random number generated is %d\n", min_max);

    test_mbst_min(my_tree, root_data, &min_max);
    printf("The minimum random number generated is %d\n", min_max);

    int root_pred, root_succ;

    if (test_mbst_pred(my_tree, root_data, &root_pred) == M_OK) {
      printf("The inorder predecessor of the root value is %d\n", root_pred);
    } else {
      printf("The root node has no predecessor\n");
    }

    if (test_mbst_succ(my_tree, root_data, &root_succ) == M_OK) {
      printf("The inorder successor of the root value is %d\n\n", root_succ);
    } else {
      printf("The root node has no successor\n\n");
    }

    int anc;
    if (test_mbst_lca(my_tree, root_pred, root_succ, &anc) == M_OK) {
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

    if (test_mbst_root(my_tree, &del) == M_OK) {
      printf("Removing %d value from BST\n", del);

      err = test_mbst_pop(my_tree, del);

      if (err != M_OK) {
        MERROR(err);
      }
    }
  }

  printf("\n");

  printf("After deletion the BST tree shows like (inorder):\n");
  test_mbst_traverse_inorder(my_tree, &print_int);
  printf("\n\n");

  fclose(fout);
  test_mbst_free(&my_tree);

  return 0;
}
