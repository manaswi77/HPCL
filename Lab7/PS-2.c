#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    int rank, size;
    int N=4, M=4, P=4;
    int A[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int B[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    int C[4][4]; // Final result
    int local_A[2][4]; // Assuming 2 processes
    int local_C[2][4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N/size;

    MPI_Scatter(A, rows_per_proc*M, MPI_INT, local_A, rows_per_proc*M, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, M*P, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i=0;i<rows_per_proc;i++){
        for(int j=0;j<P;j++){
            local_C[i][j]=0;
            for(int k=0;k<M;k++){
                local_C[i][j] += local_A[i][k] * B[k][j];
            }
        }
    }

    MPI_Gather(local_C, rows_per_proc*P, MPI_INT, C, rows_per_proc*P, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank==0){
        printf("Result Matrix:\n");
        for(int i=0;i<N;i++){
            for(int j=0;j<P;j++)
                printf("%d ", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
