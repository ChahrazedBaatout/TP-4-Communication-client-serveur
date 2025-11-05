#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "segdef2.h"
#include "magic_values.h"

static int count = INITIAL_VALUE;

int longeurPid(int pid) {
    int length = INITIAL_VALUE;
    while (pid != INITIAL_VALUE) {
        pid /= DECIMAL_BASE;
        length++;
    }
    return length;
}

int power(int base, int exp) {
    int result = INITIAL_VALUE+1;
    for (int i = INITIAL_VALUE; i < exp; i++) {
        result *= base;
    }
    return result;
}

int generateRequestId(int pid) {
    return pid +(++count)*power(DECIMAL_BASE,(longeurPid(pid)));
}

long* generateRandomArray(long* arr, int size) {
    for (int i = INITIAL_VALUE; i < size; i++) {
        arr[i] = getrand();
    }
    return arr;
}

void displayArray(long* arr, int size) {
    for (int i = INITIAL_VALUE; i < size; i++) {
        printf("%ld ", arr[i]);
    }
    printf(REUTRN_TO_THE_LINE);
}

long averageArray(long* arr, int size) {
    long sum = INITIAL_VALUE;
    for (int i = INITIAL_VALUE; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

segment preparedSegment() {
    segment seg;
    seg.pid = getpid();
    seg.req = generateRequestId(getpid());
    generateRandomArray(seg.tab, MAXVAL);
    seg.result = INITIAL_VALUE;
    return seg;
}

void displaySegment(segment seg) {
    printf("Segment PID: %d\n", seg.pid);
    printf("Segment REQ: %d\n", seg.req);
    printf("Segment TAB: ");
    displayArray(seg.tab, MAXVAL);
    printf("Segment RESULT (client): %ld\n", averageArray(seg.tab, MAXVAL));
    printf("Segment RESULT (server): %ld\n", seg.result);
    printf("Compare results (diff): ");
    seg.result - averageArray(seg.tab, MAXVAL) == INITIAL_VALUE ?printf("TRUE") : printf("FALSE");
    printf("\n--------------------------------------------------------\n");
}

void displaySegment2(segment seg) {
    if (seg.result - averageArray(seg.tab, MAXVAL) != INITIAL_VALUE) {
        printf("Segment PID: %d\n", seg.pid);
        printf("Segment REQ: %d\n", seg.req);
        printf("Segment TAB: ");
        displayArray(seg.tab, MAXVAL);
        printf("Segment RESULT (client): %ld\n", averageArray(seg.tab, MAXVAL));
        printf("Segment RESULT (server): %ld\n", seg.result);
        printf("\n--------------------------------------------------------\n");
    }
}

void initialisations(int* semid, int* shmid, segment** seg ) {
    if ((*semid=semget(CLE,CLE_VALUE,INITIAL_VALUE))==ERRROR_VALUE) {
        perror(SEMGET_ERROR);
        exit(ERRROR_VALUE);
    }

    if ((*shmid = shmget(CLE, segsize, INITIAL_VALUE)) == ERRROR_VALUE) {
        perror(SHMGET_ERROR);
        exit(ERRROR_VALUE);
    }

    if ((*seg=shmat(*shmid,NULL,INITIAL_VALUE))== (void *)ERRROR_VALUE) {
        perror(SHMAT_ERROR);
        exit(ERRROR_VALUE);
    }
    init_rand();
}

void sendOneSegment(int semid, segment* seg) {
    acq_sem(semid, SEG_DISPO);
    *seg = preparedSegment();
    acq_sem(semid, SEG_INIT);
    wait_sem(semid, RES_OK);
    lib_sem(semid, SEG_INIT);
    acq_sem(semid, RES_OK);
    lib_sem(semid,RES_OK);
    lib_sem(semid,SEG_DISPO);
    displaySegment2(*seg);
}

void sendSegments(int semid, segment* seg, int n) {
    for (int i = INITIAL_VALUE; i < n; i++) {
        sendOneSegment(semid, seg);
    }
}

void detachSegment(segment* seg) {
    if (shmdt(seg) == ERRROR_VALUE) {
        perror(SHMD_ERROR);
        exit(ERRROR_VALUE);
    }
}

int main(void) {
    for (int i = INITIAL_VALUE; i < NUMBER_OF_CLIENTS; i++) {
        fork();
        int semid;
        int shmid;
        segment *seg;
        initialisations(&semid, &shmid, &seg );
        sendSegments(semid, seg, NUMBER_OF_SEGMENTS_PER_CLIENT);
        detachSegment(seg);
    }
    return INITIAL_VALUE;
}