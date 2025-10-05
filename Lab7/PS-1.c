#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    int N = 4, M = 4; // Example size, can be larger
    int A[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int V[4] = {1,1,1,1};
    int R[4]; // Final result
    int local_R[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;

    int local_A[rows_per_proc][M];
    
    // Scatter rows of A to all processes
    MPI_Scatter(A, rows_per_proc*M, MPI_INT, local_A, rows_per_proc*M, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast vector V to all processes
    MPI_Bcast(V, M, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its partial result
    for(int i=0; i<rows_per_proc; i++){
        local_R[i] = 0;
        for(int j=0; j<M; j++){
            local_R[i] += local_A[i][j] * V[j];
        }
    }

    // Gather results to master
    MPI_Gather(local_R, rows_per_proc, MPI_INT, R, rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        printf("Result Vector:\n");
        for(int i=0; i<N; i++)
            printf("%d ", R[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
