#include <stdio.h>
#include <math.h>
#include <omp.h>

// Function for x > 0 branch
double positiveBranch(double x) {
    double res = 0.0;
    for (int i = 0; i < 1000000; i++)
        res += sqrt(x + i * 0.000001);
    return res;
}

// Function for x <= 0 branch
double negativeBranch(double x) {
    double res = 0.0;
    for (int i = 0; i < 1000000; i++)
        res += log(fabs(x) + i * 0.000001);
    return res;
}

int main() {
    double x = -25.0;  // Try with both positive and negative values
    double resultPos = 0.0, resultNeg = 0.0;

    double start = omp_get_wtime();

    // Run both branches in parallel (speculative execution)
    #pragma omp parallel sections
    {
        #pragma omp section
        { resultPos = positiveBranch(fabs(x)); }

        #pragma omp section
        { resultNeg = negativeBranch(x); }
    }

    // Condition resolves here
    double finalResult;
    if (x > 0)
        finalResult = resultPos;
    else
        finalResult = resultNeg;

    double end = omp_get_wtime();

    printf("x = %.2f\n", x);
    printf("Final Result = %.4f\n", finalResult);
    printf("Speculative Execution Time = %lf seconds\n", end - start);

    return 0;
}
