#include <scl_datastruc.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_2.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS\n");

    fclose(fout);

    return 0;
}