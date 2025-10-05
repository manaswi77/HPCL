#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

// Function to swap
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function
int partition(int arr[], int low, int high, int pivot) {
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Sequential QuickSort
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int pi = partition(arr, low, high, pivot);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Speculative QuickSort
void speculativeQuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot1 = arr[low];
        int pivot2 = arr[(low + high) / 2];

        int arr1[N], arr2[N];
        for (int i = 0; i < N; i++) { arr1[i] = arr[i]; arr2[i] = arr[i]; }

        int pi1, pi2;

        #pragma omp parallel sections
        {
            #pragma omp section
            { pi1 = partition(arr1, low, high, pivot1); }

            #pragma omp section
            { pi2 = partition(arr2, low, high, pivot2); }
        }

        // Choose the partition closer to middle (better balance)
        int chosenPivot = (abs(pi1 - (N / 2)) < abs(pi2 - (N / 2))) ? pivot1 : pivot2;

        int chosenArr[N];
        for (int i = 0; i < N; i++) chosenArr[i] = arr[i];

        int pi = partition(chosenArr, low, high, chosenPivot);

        quicksort(chosenArr, low, pi - 1);
        quicksort(chosenArr, pi + 1, high);

        // Copy back to original
        for (int i = 0; i < N; i++)
            arr[i] = chosenArr[i];
    }
}

int main() {
    int arr[N] = {29, 10, 14, 37, 13, 25, 1, 45, 33, 19};

    printf("Original Array:\n");
    for (int i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    double start = omp_get_wtime();
    speculativeQuickSort(arr, 0, N - 1);
    double end = omp_get_wtime();

    printf("\nSorted Array (Speculative QuickSort):\n");
    for (int i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Speculative Execution Time: %lf seconds\n", end - start);
    return 0;
}
