#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define CLE 		(key_t)3
#define CLE_VALUE    3
#define SEG_DISPO 	0
#define SEG_INIT  	1
#define RES_OK    	2
#define MAXVAL   	100
#define NUMBER_OF_SEGMENTS_PER_CLIENT 5
#define NUMBER_OF_CLIENTS 10
typedef struct shmseg
{
   int pid;
   int req;
   long tab[MAXVAL];
   long result;
} segment;

#define segsize sizeof(segment)

void init_rand();
long getrand();

void wait_sem(int,int);
void acq_sem(int,int);
void lib_sem(int,int);
