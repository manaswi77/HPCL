#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 6      // Image size (6x6)
#define K 3      // Kernel size (3x3)

int main(int argc, char* argv[]) {
    int rank, size;
    int image[N][N], kernel[K][K], result[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;

    // Initialize image and kernel only in master
    if (rank == 0) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                image[i][j] = i + j;

        int temp[K][K] = {
            {1, 0, -1},
            {1, 0, -1},
            {1, 0, -1}
        };

        for (int i = 0; i < K; i++)
            for (int j = 0; j < K; j++)
                kernel[i][j] = temp[i][j];
    }

    // Broadcast kernel to all processes
    MPI_Bcast(kernel, K * K, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter rows of image to all processes
    int sub_image[rows_per_proc][N];
    MPI_Scatter(image, rows_per_proc * N, MPI_INT,
                sub_image, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes convolution on its part
    int sub_result[rows_per_proc][N];
    for (int i = 1; i < rows_per_proc - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += sub_image[i + ki][j + kj] * kernel[ki + 1][kj + 1];
            sub_result[i][j] = sum;
        }
    }

    // Gather all parts of result
    MPI_Gather(sub_result, rows_per_proc * N, MPI_INT,
               result, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final Convolution Output:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%4d", result[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
