#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
    int n = 1000000;  // Data size
    int *A = (int*)malloc(n * sizeof(int));
    int *B = (int*)malloc(n * sizeof(int));
    int scalar = 5;
    int i;

    // Initialize vector A
    for (i = 0; i < n; i++) {
        A[i] = i;
    }

    double start = omp_get_wtime();

    // Parallel scalar addition
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        B[i] = A[i] + scalar;
    }

    double end = omp_get_wtime();

    printf("Time taken: %f seconds\n", end - start);

    free(A);
    free(B);
    return 0;
}
