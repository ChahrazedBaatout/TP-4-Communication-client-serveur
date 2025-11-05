#include "segdef2.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_rand() {
    srand(time(NULL));
}
long getrand() {
    return rand();
}

void wait_sem(int id, int sem){

    struct sembuf sop;
    sop.sem_flg = SEM_UNDO;
    sop.sem_num = sem;
    sop.sem_op = 0;

    if(semop(id, &sop, 1) == -1){
        perror("semop");
        exit(1);
    }
}
void acq_sem(int id, int sem) {
    struct sembuf sop;
    sop.sem_flg = SEM_UNDO;
    sop.sem_num = sem;
    sop.sem_op = -1;

    if(semop(id, &sop, 1) == -1){
        perror("semop");
        exit(1);
    }
}
void lib_sem(int id, int sem){

    struct sembuf sop;
    sop.sem_flg = SEM_UNDO;
    sop.sem_num = sem;
    sop.sem_op = 1;

    if(semop(id, &sop, 1) == -1){
        perror("semop");
        exit(1);
    }
}