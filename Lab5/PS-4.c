#include <stdio.h>
#include <omp.h>
#define N 8

int main() {
    int A[N] = {1,2,3,4,5,6,7,8};
    int prefix[N];

    prefix[0] = A[0];

    #pragma omp parallel for
    for (int i = 1; i < N; i++) {
        prefix[i] = prefix[i-1] + A[i]; // depends on i-1 (not fully parallel)
    }

    printf("Prefix Sum Result:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", prefix[i]);
    }
    printf("\n");
    return 0;
}
