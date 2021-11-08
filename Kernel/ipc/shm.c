#include "memorymanager.h"


#define CANT_SHM 10
#define SHM_SIZE 1024
#define NULL (void* ) 0 

void* sharedMems[CANT_SHM] = {0};


void* shmOpen(int shm){
    if(shm >= CANT_SHM){
        return NULL;
    }
    if (sharedMems[shm] == 0)
        sharedMems[shm] =  mallocMM(SHM_SIZE);
    return sharedMems[shm];
}