#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 8  // Board size

// Check if a queen can be placed on board[row][col]
int isSafe(int board[N][N], int row, int col) {
    int i, j;

    // Check left row
    for (i = 0; i < col; i++)
        if (board[row][i])
            return 0;

    // Check upper diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return 0;

    // Check lower diagonal
    for (i = row, j = col; i < N && j >= 0; i++, j--)
        if (board[i][j])
            return 0;

    return 1;
}

// Recursive backtracking function
int solveNQUtil(int board[N][N], int col) {
    if (col >= N)  // All queens placed successfully
        return 1;

    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col)) {
            board[i][col] = 1;
            if (solveNQUtil(board, col + 1))
                return 1;
            board[i][col] = 0;  // Backtrack
        }
    }
    return 0;
}

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", board[i][j]);
        printf("\n");
    }
}

int main() {
    int totalSolutions = 0;
    double start = omp_get_wtime();

    // Each thread handles one possible queen position in column 0
    #pragma omp parallel for reduction(+:totalSolutions)
    for (int i = 0; i < N; i++) {
        int board[N][N] = {0};
        board[i][0] = 1;  // Place queen in first column
        if (solveNQUtil(board, 1)) {
            #pragma omp critical
            {
                printf("\nThread %d found a solution (starting row %d):\n",
                       omp_get_thread_num(), i);
                printBoard(board);
                printf("\n");
            }
            totalSolutions++;
        }
    }

    double end = omp_get_wtime();

    printf("\nTotal Solutions Found: %d\n", totalSolutions);
    printf("Parallel Execution Time: %lf seconds\n", end - start);

    return 0;
}
