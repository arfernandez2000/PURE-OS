// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "semaphores.h"
#include "memorymanager.h"
#include "naiveConsole.h"
#include "pcb.h"
#include "lib.h"
#define MAX_SEM 10000


typedef struct Semaphore
{
    uint32_t id;
    int value;
    uint16_t listeners;
    uint32_t blockedPIDs[MAX_BLOCKED_PIDS];
    uint16_t blockedPIDsSize;
    int mutex;
    struct Semaphore *next;

} Semaphore;

Semaphore *semaphores = NULL;
static Semaphore *findSem(uint32_t id);

uint32_t sOpen(uint32_t id, int initValue)
{

    Semaphore *sem = findSem(id);
    if (sem == NULL)
    {
        sem = (Semaphore*) mallocMM(sizeof(Semaphore));
        if (sem == NULL)
            return -1;

        sem->value = initValue;
        sem->listeners = 0;
        sem->blockedPIDsSize = 0;
        sem->id = id;
        sem->next = NULL;
        sem->mutex = 0;

        Semaphore *lastSem = semaphores;

        if (lastSem == NULL)
            semaphores = sem;
        else
        {
            while (lastSem->next != NULL)
                lastSem = lastSem->next;
            lastSem->next = sem;
        }
    }

    if (sem->listeners >= MAX_BLOCKED_PIDS)
    {
        ncPrint("No space for this listener");
        return -1;
    }

    sem->listeners++;
    return id;
}

int sWait(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return -1;

    acquire(&(sem->mutex));
    if (sem->value > 0)
    {
        sem->value--;
        release(&(sem->mutex));
        return 0;
    }
    else
    {
        int currPid = getPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = currPid;
        release(&(sem->mutex));
        if(blockProcess(currPid) == -1){
            return -1;
        };
    }

    return 0;
}

int sPost(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return -1;

    acquire(&(sem->mutex));
    if (sem->blockedPIDsSize > 0)
    {
        int nextPid = sem->blockedPIDs[0];
        for (int i = 0; i < sem->blockedPIDsSize - 1; i++)
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        sem->blockedPIDsSize--;
        if(unBlockProcess(nextPid) == -1){
            return -1;
        };
        release(&(sem->mutex));
        return 0;
    }
    else
        sem->value++;

    release(&(sem->mutex));
    return 0;
}

int sClose(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return -1;

    sem->listeners--;
    if (sem->listeners > 0)
        return 0;

    Semaphore *aux = semaphores;
    if (aux == sem)
        semaphores = aux->next;
    else
    {
        while (aux->next != sem)
            aux = aux->next;
        aux->next = sem->next;
    }

    freeMM(sem);
    return 0;
}

int dumpBlockedPIDs(uint32_t *blockedPIDs, uint16_t blockedPIDsSize, char *** result, int line)
{
    for (int i = 0; i < blockedPIDsSize; i++)
    {
        *result[line] = "         PID: ";
        char buffer[10];
        strcat(*result[line++], itoa(blockedPIDs[i],buffer,10,10));
    }
    return line;
}

int lines;
int getLinesDump(){
    return lines;
}

char** semDisplay()
{
    char** result = mallocMM(10000);

    Semaphore *sem = semaphores;
    int i = 1;
    int line =0;
    while (sem)
    {   
        result[line++] = "-------------------------------";
        result[line++]= "Semaphore";
        strcat(result[line], itoa(i++,result[line++],10,10));

        result[line] = "     ID: ";
        char buffer[10];
        strcat(result[line++], itoa(sem->id,buffer,10,10));

        result[line] = "     Value: ";
        strcat(result[line++], itoa(sem->value,buffer,10,10));

        result[line] = "     Number of attached processes: ";

        strcat(result[line++], itoa(sem->listeners,buffer,10,10));
  
        result[line] ="     Number of blocked processes: ";
        strcat(result[line++], itoa(sem->blockedPIDsSize,buffer,10,10));

        result[line] = "     Blocked processes:";
        line = dumpBlockedPIDs(sem->blockedPIDs, sem->blockedPIDsSize, &result,line);
        sem = sem->next;
    }
    result[line++] = "-------------------------------------" ;
    lines = line;
    return result;
}


static Semaphore *findSem(uint32_t id)
{
    Semaphore *sem = semaphores;
    while (sem)
    {
        if (sem->id == id)
            return sem;
        sem = sem->next;
    }
    return NULL;
}