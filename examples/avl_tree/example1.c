#include <scl_datastruc.h>

int main(void) {
    FILE *fout = NULL;

    if ((fout = freopen("out_1.txt", "w", stdout)) == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS\n");

    fclose(fout);

    return 0;
}