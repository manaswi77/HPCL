#include <stdio.h>
#include <omp.h>

int main() {
    long long num_steps = 1000000000; // Data size
    double step;
    double pi = 0.0;

    step = 1.0 / (double)num_steps;
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        double x;
        double sum = 0.0;
        int i;
        #pragma omp for
        for (i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        pi += sum * step;
    }

    double end = omp_get_wtime();
    printf("Pi value: %.15f\n", pi);
    printf("Time taken: %f seconds\n", end - start);

    return 0;
}
