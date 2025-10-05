#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Initialize MPI environment

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Get total number of processes

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Get rank of this process

    printf("Hello world from process %d out of %d\n", world_rank, world_size);

    MPI_Finalize(); // Finalize MPI environment
    return 0;
}
