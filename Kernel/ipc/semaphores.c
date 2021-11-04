#include "semaphores.h"
#include "memorymanager.h"
#include "naiveConsole.h"
#include "pcb.h"
#include "lib.h"


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

static void dumpBlockedPIDs(uint32_t *blockedPIDs, uint16_t blockedPIDsSize);
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

void sStatus()
{
    ncPrint("\nSEMAPHORE DUMP\n");
    ncPrint("------------------------------------------------\n");
    ncPrint("Active semaphores:\n");
    Semaphore *sem = semaphores;
    int i = 1;
    while (sem)
    {
        ncPrint("-------------------------------\n");
        ncPrint("Semaphore");
        ncPrintDec(i++);
        ncPrint("\n");
        ncPrint("     ID: ");
        ncPrintDec(sem->id);
        ncPrint("\n");
        ncPrint("     Value: ");
        ncPrintDec(sem->value);
        ncPrint("\n");
        ncPrint("     Number of attached processes: ");
        ncPrintDec(sem->listeners);
        ncPrint("\n");
        ncPrint("     Number of blocked processes: ");
        ncPrintDec(sem->blockedPIDsSize);
        ncPrint("\n");
        ncPrint("     Blocked processes:\n");
        dumpBlockedPIDs(sem->blockedPIDs, sem->blockedPIDsSize);
        sem = sem->next;
    }
    ncPrint("-------------------------------\n");
}

static void dumpBlockedPIDs(uint32_t *blockedPIDs, uint16_t blockedPIDsSize)
{
    for (int i = 0; i < blockedPIDsSize; i++)
    {
        ncPrint("         PID: ");
        ncPrintDec(blockedPIDs[i]);
        ncPrint("\n");
    }
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