#include <stdio.h>
#include <omp.h>
#define N 3

int main() {
    int A[N][N] = {{1,2,3},{4,5,6},{7,8,9}};
    int V[N] = {1,2,3};
    int R[N] = {0};

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            R[i] += A[i][j] * V[j];
        }
    }

    printf("Matrix-Vector Multiplication Result:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", R[i]);
    }
    printf("\n");
    return 0;
}
