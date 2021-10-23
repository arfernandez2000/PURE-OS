#include "pcb.h"
#include "defs.h"
#include <memorymanager.h>

static const uint8_t * firstProcessAddress = (uint8_t *) 0x18000000;
static const long stackSize = 0x4000000; // 2^26
static const uint8_t * lastProcessAddress = (uint8_t *) 0xC8000000; //50 stack procesess

int activeProcesses = 0, currentProcess = -1;


uint64_t processesStack[MAX_PROCESSES];
PCB processQueue[MAX_PROCESSES];
int stopInantion = 3;
PCB* currentPCB = NULL;
int processID = 0;

void initProcesses(){


}


void* scheduler(void * lastRSP){
    if(currentPCB){

        if(currentPCB->state == READY && stopInantion > 0){
            stopInantion--;
            return lastRSP;
        }

        currentPCB->rsp = lastRSP;
    }
    stopInantion = currentPCB->priority;
        
    return currentPCB->rsp;



}
void createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    PCB* newProcess = mallocMM(sizeof(PCB));

    newProcess->foreground = fg;
    newProcess->foreground = fg;
    newProcess->argc = argc;
    newProcess->argv = argv;
    newProcess->pipes[0] = fd[0];
    newProcess->pipes[1] = fd[1];

    newProcess->ppid = processID;
    newProcess->pid = ++processID;

    strcpy(newProcess->name, name);
         //nombre, priority, state
}


void newProcessStack(void (*fn)) {
    if (firstProcessAddress - activeProcesses * stackSize + stackSize <= lastProcessAddress){
        return;
    }
    _initialize_stack_frame(fn, firstProcessAddress - activeProcesses * stackSize);
}


void newStack(uint64_t rsp) {
    processesStack[activeProcesses++] = rsp;
}

uint64_t preserveStack(uint64_t rsp) {
    if (currentProcess != -1) {
        processesStack[currentProcess] = rsp;
    }
    if (++currentProcess >= activeProcesses) currentProcess = 0;
    if (activeProcesses == 0){
        return 0;
    }
    return processesStack[currentProcess];
}

static uint64_t sampleRSP;

void saveSampleRSP(uint64_t rsp) {
    sampleRSP = rsp;
}

uint64_t getSampleRSP() {
    return sampleRSP;
}


void cleanProcesses() {
    activeProcesses = 0;
    currentProcess = -1;
}