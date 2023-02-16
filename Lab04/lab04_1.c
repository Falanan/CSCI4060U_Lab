#include <stdio.h>

int fib(int n) {
    if (n < 2) {
        return n;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

int main() {
    int n = 30;

    for (int i = 0; i < n; i++) {
        int result = fib(i);
        printf("%d ", result);
    }
    printf("\n");

    return 0;
}


