
/*
 * week4_1_dynamic_array.c
 * Author: Arda Kocaman
 * Student ID: 251ADB187
 * Description: Read and process a dynamically allocated integer array.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size.\n");
        return 1;
    }

    // Allocate memory only after validating the size.
    int *arr = malloc((size_t)n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            // Prevent leaks if input fails midway.
            free(arr);
            printf("Invalid input.\n");
            return 1;
        }
    }

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    printf("Sum = %lld\n", sum);
    printf("Average = %.2f\n", (double)sum / n);

    free(arr);
    return 0;
}
