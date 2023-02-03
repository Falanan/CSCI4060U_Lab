#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = (int *) malloc(10 * sizeof(int));

    printf("size: %lu\n", sizeof(*ptr) / sizeof(int));

    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Initialize the memory
    for (int i = 0; i < 10; i++) {
        ptr[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");
    // Increase the size of the memory block
    int *new_ptr = (int *) realloc(ptr, 20 * sizeof(int));
    if (new_ptr == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }

    // Use the newly allocated memory block
    for (int i = 10; i < 20; i++) {
        new_ptr[i] = i;
    }

    for (int i = 0; i < 20; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Free the memory
    free(new_ptr);

    return 0;
}
