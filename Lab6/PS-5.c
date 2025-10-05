#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 10; // Example array size
    int A[10] = {1,2,3,4,5,6,7,8,9,10};

    int local_sum = 0;
    if (rank == 0) {
        for(int i = 0; i < n/2; i++)
            local_sum += A[i];
        MPI_Send(&local_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        for(int i = n/2; i < n; i++)
            local_sum += A[i];
        int sum0;
        MPI_Recv(&sum0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int total_sum = sum0 + local_sum;
        printf("Total sum of array: %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}
