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
    printf("Compare results (diff): ");
    seg.result - averageArray(seg.tab, maxval) == 0 ?printf("TRUE") : printf("FALSE");
    printf("\n--------------------------------------------------------\n");
}


void displaySegment2(segment seg) {
    if (seg.result - averageArray(seg.tab, maxval) != 0) {
        printf("Segment PID: %d\n", seg.pid);
        printf("Segment REQ: %d\n", seg.req);
        printf("Segment TAB: ");
        displayArray(seg.tab, maxval);
        printf("Segment RESULT (client): %ld\n", averageArray(seg.tab, maxval));
        printf("Segment RESULT (server): %ld\n", seg.result);
        printf("\n--------------------------------------------------------\n");
    }
}

void initialisations(int* semid, int* shmid, segment** seg ) {
    if ((*semid=semget(cle,3,0))==-1) {
        perror("semget");
        exit(1);
    }

    if ((*shmid = shmget(cle, segsize, 0)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((*seg=shmat(*shmid,NULL,0))== (void *)-1) {
        perror("shmat");
        exit(1);
    }
    init_rand();
}

void sendOneSegment(int semid, segment* seg) {
    acq_sem(semid, seg_dispo);
    *seg = preparedSegment();
    acq_sem(semid, seg_init);
    wait_sem(semid, res_ok);
    lib_sem(semid, seg_init);
    acq_sem(semid, res_ok);
    lib_sem(semid,res_ok);
    lib_sem(semid,seg_dispo);
    displaySegment2(*seg);
}

void sendSegments(int semid, segment* seg, int n) {
    for (int i = 0; i < n; i++) {
        sendOneSegment(semid, seg);
    }
}

void detachSegment(segment* seg) {
    if (shmdt(seg) == -1) {
        perror("shmdt");
        exit(1);
    }
}

int main(void) {
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        fork();
        int semid;
        int shmid;
        segment *seg;
        initialisations(&semid, &shmid, &seg );
        sendSegments(semid, seg, NUMBER_OF_SEGMENTS_PER_CLIENT);
        detachSegment(seg);
    }
    return 0;
}