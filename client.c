#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "segdef.h"

static int count = 0;

int longeurPid(int pid) {
    int length = 0;
    while (pid != 0) {
        pid /= 10;
        length++;
    }
    return length;
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int generateRequestId(int pid) {
    return pid +(++count)*power(10,(longeurPid(pid)));
}

long* generateRandomArray(long* arr, int size) {
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

segment preparedSegment() {
    segment seg;
    seg.pid = getpid();
    seg.req = generateRequestId(getpid());
    generateRandomArray(seg.tab, maxval);
    seg.result = 0;
    return seg;
}

void displaySegment(segment seg) {
    printf("Segment PID: %d\n", seg.pid);
    printf("Segment REQ: %d\n", seg.req);
    printf("Segment TAB: ");
    displayArray(seg.tab, maxval);
    printf("Segment RESULT (client): %ld\n", averageArray(seg.tab, maxval));
    printf("Segment RESULT (server): %ld\n", seg.result);
    printf("Compare results (diff): %ld\n", seg.result - averageArray(seg.tab, maxval));
    printf("--------------------------------------------------------\n");
}

int main(void) {
    init_rand();
    segment seg = preparedSegment();
    displaySegment(seg);
    segment seg2 = preparedSegment();
    displaySegment(seg2);

    segment seg3 = preparedSegment();
    displaySegment(seg3);
    return 0;
}