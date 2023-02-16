#include <stdio.h>
#include <omp.h>


#define NUM_THREADS 4

int fib(int n) {
    if (n < 2) {
        return n;
    } else {
        int x, y;
        #pragma omp task shared(x)
        x = fib(n-1);
        #pragma omp task shared(y)
        y = fib(n-2);
        #pragma omp taskwait
        return x + y;
    }
}

int main() {
    int n = 30;

    // Set number of threads
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        #pragma omp for ordered
        // #pragma omp for
        for (int i = 0; i < n; i++) {
            #pragma omp ordered
            {
                int result = fib(i);
                printf("%d ", result);
            }
            // int result = fib(i);
            // printf("%d ", result);
            
        }
        // printf("\n");
    }
    printf("\n");

    return 0;
}
