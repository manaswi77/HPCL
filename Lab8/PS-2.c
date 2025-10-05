#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char* argv[]) {
    int rank, size;
    float A[N], B[N];
    float local_A[N], local_B[N];
    float local_sum = 0.0, total_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elements_per_proc = N / size;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            A[i] = i + 1;
            B[i] = i + 1;
        }
    }

    MPI_Scatter(A, elements_per_proc, MPI_FLOAT, local_A, elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, elements_per_proc, MPI_FLOAT, local_B, elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < elements_per_proc; i++)
        local_sum += local_A[i] * local_B[i];

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Dot Product = %.2f\n", total_sum);

    MPI_Finalize();
    return 0;
}
