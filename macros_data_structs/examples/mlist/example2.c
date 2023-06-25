#include "../../src/m_list.h"
#include <stdio.h>

#define MAX_STRING_SIZE 100

typedef char *str;

int32_t compare_ptr_str(const str *const elem1, const str *const elem2) {
  return strncmp(*elem1, *elem2, MAX_STRING_SIZE);
}

void free_str(str *data) {
  if (NULL != data) {
    free(*data);
    *data = '\0';
  }
}

void print_str(const str *const data) {
  if (NULL != data) {
    printf(" %s\n", *data);
  }
}

MLIST_ALL(test, str)

int main(void) {
  FILE *fout = NULL;

  if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
    exit(EXIT_FAILURE);
  }

  FILE *fin = NULL;

  if ((fin = freopen("example2.in", "r", stdin)) == NULL) {
    printf("Could not open for reading\n");
    exit(EXIT_FAILURE);
  }

  merr_t err = M_OK;

  fseek(fin, 0, SEEK_SET);

  test_mlist_t strings = test_mlist(&compare_ptr_str, &free_str);

  size_t str_num = 0;
  int check = 0;

  check = scanf("%lu", &str_num);

  if (check > 1) {
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < str_num; ++i) {
    char *ptr_data = malloc(MAX_STRING_SIZE);
    check = scanf("%s", ptr_data);

    if (check > 1) {
      exit(EXIT_FAILURE);
    }

    err = test_mlist_push(strings, ptr_data);

    if (M_OK != err) {
      MERROR(err);
    }
  }

  fclose(fin);

  printf("Print strings dlist:\n");
  test_mlist_traverse(strings, &print_str);
  printf("\n");

  char *find;
  if (test_mlist_find_idx(strings, 4, &find) == M_OK) {
    printf("String from pos 4 is %s\n", find);
  }

  test_mlist_free(&strings);

  fclose(fin);
  fclose(fout);

  return 0;
}