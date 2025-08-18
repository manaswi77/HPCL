#include <stdio.h>
#include <omp.h>

#define SIZE 10   
#define ITEMS 20  

int buffer[SIZE];
int in = 0, out = 0, count = 0;

int main() {
    #pragma omp parallel sections
    {
        // Producer Section
        #pragma omp section
        {
            for (int i = 1; i <= ITEMS; i++) {
                int produced = 0;
                while (!produced) {
                    #pragma omp critical
                    {
                        if (count < SIZE) {   // buffer not full
                            buffer[in] = i;
                            in = (in + 1) % SIZE;
                            count++;
                            printf("Produced: %d | Buffer Count: %d\n", i, count);
                            produced = 1;  // mark item as produced
                        }
                    }
                }
            }
        }

        // Consumer Section
        #pragma omp section
        {
            for (int i = 1; i <= ITEMS; i++) {
                int consumed = 0;
                while (!consumed) {
                    #pragma omp critical
                    {
                        if (count > 0) {   // buffer not empty
                            int item = buffer[out];
                            out = (out + 1) % SIZE;
                            count--;
                            printf("Consumed: %d | Buffer Count: %d\n", item, count);
                            consumed = 1;  // mark item as consumed
                        }
                    }
                }
            }
        }
    }
    return 0;
}
