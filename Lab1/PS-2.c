#include <stdio.h>
#include <omp.h>

int main() {
    int num_of_threads;

    printf("Enter number of threads: ");
    scanf("%d", &num_of_threads);

    omp_set_num_threads(num_of_threads);

    printf("\n--- Sequential Execution ---\n");
    for (int i = 0; i < num_of_threads; i++) {
        printf("Hello, World from thread %d out of %d (Sequential)\n", i, num_of_threads);
    }

    printf("\n--- Parallel Execution ---\n");

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total = omp_get_num_threads();
        printf("Hello, World from thread %d out of %d (Parallel)\n", tid, total);
    }

    return 0;
}