#include <stdio.h>
#include <stdlib.h>
#include "segdef.h"
#define ARRAY_SIZE 10

long* generateRandomArray(int size) {
    long* arr = (long*)malloc(size * sizeof(long));
    for (int i = 0; i < size; i++) {
        arr[i] = getrand();
    }
    return arr;
}

void displayArray(long* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}

long averageArray(long* arr, int size) {
    long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

int main(void) {
    init_rand();
    int size = ARRAY_SIZE;
    long* randomArray = generateRandomArray(size);
    printf("Generated array: ");
    displayArray(randomArray, size);
    printf("Average: %ld\n",  averageArray(randomArray, size));
    return 0;
}