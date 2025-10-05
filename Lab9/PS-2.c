#include <stdio.h>
#include <math.h>
#include <omp.h>

double branchA(double x) { return sqrt(x); }
double branchB(double x) { return log(fabs(x)); }

int main() {
    double x = -25.0;  // You can test with positive and negative values
    double resultA = 0.0, resultB = 0.0;
    double start, end;

    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        { resultA = branchA(fabs(x)); }

        #pragma omp section
        { resultB = branchB(x); }
    }

    double finalResult;
    if (x > 0)
        finalResult = resultA;
    else
        finalResult = resultB;

    end = omp_get_wtime();

    printf("Input x = %.2f\n", x);
    printf("Final Result = %.4f\n", finalResult);
    printf("Speculative Execution Time: %lf seconds\n", end - start);

    return 0;
}
