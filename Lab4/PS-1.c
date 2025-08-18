#include <stdio.h>
#include <omp.h>

// Function to compute Fibonacci iteratively
long long fibonacci(int n) {
    if (n <= 1) return n;

    long long f1 = 0, f2 = 1, f;
    for (int i = 2; i <= n; i++) {
        f = f1 + f2;
        f1 = f2;
        f2 = f;
    }
    return f2;
}

int main() {
    int n = 20;  // number of terms
    long long fib[n+1];

    #pragma omp parallel for shared(fib) schedule(dynamic)
    for (int i = 0; i <= n; i++) {
        long long result = fibonacci(i);

        // Synchronization using critical section
        #pragma omp critical
        {
            fib[i] = result;
        }
    }

    printf("Fibonacci Series up to %d terms:\n", n);
    for (int i = 0; i <= n; i++) {
        printf("%lld ", fib[i]);
    }
    printf("\n");

    return 0;
}
