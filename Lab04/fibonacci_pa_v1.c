#include <stdio.h>
#include <omp.h>


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
    int n = 10;

    // Set number of threads
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < n; i++) {
                #pragma omp task
                {
                    int result = fib(i);
                    #pragma omp critical
                    {
                        printf("%d ", result);
                    }
                }
            }
            // printf("\n");
        }
    }
    printf("\n");

    return 0;
}
