#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10
int main() {
    FILE *file;
    int r;
    file = fopen("matrix10.txt", "w");
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            r = rand();
            fprintf(file, "%d", r%12);
            if (i < N - 1) { fprintf(file, ","); }
        }
        fprintf(file,"\n");
    }
    fprintf(file,"0,0\n",r);
    fprintf(file,"%d,%d\n",N-1,N-1);
    fprintf(file,"end",r);
    fclose(file);
}
