#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int cmp_asc(const void *a, const void *b){ return (*(long long*)a > *(long long*)b) - (*(long long*)a < *(long long*)b); }
int cmp_desc(const void *a, const void *b){ return (*(long long*)b > *(long long*)a) - (*(long long*)b < *(long long*)a); }

int main(int argc, char **argv){
    int n = 10000000; // default size (change as needed)
    if(argc > 1) n = atoi(argv[1]);

    long long *A = malloc(sizeof(long long)*n);
    long long *B = malloc(sizeof(long long)*n);
    if(!A || !B){ perror("malloc"); return 1; }

    // Initialize vectors in parallel
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        unsigned int seed = 12345 + tid;
        #pragma omp for schedule(static)
        for(int i=0;i<n;i++){
            A[i] = rand() % 1000;
            B[i] = rand() % 1000;
        }
    }

    // To minimize scalar product: sort A ascending, B descending (we'll do serial qsort)
    qsort(A, n, sizeof(long long), cmp_asc);
    qsort(B, n, sizeof(long long), cmp_asc);
    // reverse B to descending
    for(int i=0;i<n/2;i++){
        long long tmp = B[i]; B[i] = B[n-1-i]; B[n-1-i] = tmp;
    }

    double start = omp_get_wtime();

    // Parallel dot product using reduction
    long long dot = 0;
    #pragma omp parallel for reduction(+:dot) schedule(static)
    for(int i=0;i<n;i++){
        dot += A[i] * B[i];
    }

    double end = omp_get_wtime();

    printf("n=%d  threads=%d  min_scalar_product=%lld  time=%f s\n",
           n, omp_get_max_threads(), dot, end - start);

    free(A); free(B);
    return 0;
}
