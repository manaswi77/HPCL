#include <stdio.h>
#include <omp.h>
#define N 3

int main() {
    int A[N][N] = {{1,2,3},{4,5,6},{7,8,9}};
    int scalar = 5;
    int C[N][N];

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] * scalar;
        }
    }

    printf("Matrix-Scalar Multiplication Result:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    return 0;
}
