#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 6

// 0 = path, 1 = wall
int maze[N][N] = {
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 0},
    {1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0}
};

int visited[N][N] = {0};
int found = 0; // shared flag

int isSafe(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0 && !visited[x][y]);
}

void solveMaze(int x, int y) {
    if (found) return;

    if (x == N - 1 && y == N - 1) {
        #pragma omp critical
        {
            printf("Exit found by thread %d at (%d, %d)\n", omp_get_thread_num(), x, y);
            found = 1;
        }
        return;
    }

    visited[x][y] = 1;

    int moves[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // down, right, up, left

    #pragma omp parallel for shared(found)
    for (int i = 0; i < 4; i++) {
        int newX = x + moves[i][0];
        int newY = y + moves[i][1];

        if (!found && isSafe(newX, newY)) {
            solveMaze(newX, newY);
        }
    }

    visited[x][y] = 0; // backtrack
}

int main() {
    double start = omp_get_wtime();

    printf("Solving maze using Exploratory Decomposition...\n");

    #pragma omp parallel
    {
        #pragma omp single
        solveMaze(0, 0);
    }

    double end = omp_get_wtime();

    if (!found)
        printf("No path found!\n");

    printf("Execution time: %lf seconds\n", end - start);
    return 0;
}
