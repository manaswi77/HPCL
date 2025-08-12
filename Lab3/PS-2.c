#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void fill_matrix(double *M, int rows, int cols){
    #pragma omp parallel for schedule(static)
    for(long long i=0;i<(long long)rows*cols;i++) M[i] = (double)(i % 100);
}

void add_matrix(double *A, double *B, double *C, int rows, int cols){
    #pragma omp parallel for collapse(2) schedule(static)
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            C[i*cols + j] = A[i*cols + j] + B[i*cols + j];
        }
    }
}

int main(int argc, char **argv){
    int rows=1000, cols=1000;
    if(argc>1){ rows = atoi(argv[1]); cols = rows; }

    double *A = malloc(sizeof(double)*rows*cols);
    double *B = malloc(sizeof(double)*rows*cols);
    double *C = malloc(sizeof(double)*rows*cols);
    if(!A||!B||!C){ perror("malloc"); return 1; }

    fill_matrix(A, rows, cols);
    fill_matrix(B, rows, cols);

    double start = omp_get_wtime();
    add_matrix(A, B, C, rows, cols);
    double end = omp_get_wtime();

    // sample checksum to ensure correctness
    double s=0;
    for(long long i=0;i<(long long)rows*cols;i++) s += C[i];

    printf("size=%dx%d threads=%d time=%f checksum=%e\n", rows, cols, omp_get_max_threads(), end-start, s);

    free(A); free(B); free(C);
    return 0;
}
