#include "queue.h"
#include "defs.h"
static const uint8_t * pure64FirstFreeSpaceDir = (uint8_t *) 0x0000000000200000; //arbitrary
static const uint8_t * pure64LastDir = (uint8_t *) 0xFFFFFFFFFFFFFFFF; 

int activeProcesses = 0, currentProcess = -1;
uint64_t processesStack[MAX_PROCESSES];
//------Process variables------
static uint64_t newPID = 0;
static PCB* currentProcessPCB = NULL;
static PCB* idleProcess = NULL;
//-----------------------------
QueueADT processQueue = NULL;
uint64_t stopInanition;

void initScheduler(){

//cargar un proceso vacio que este READY

}

void * scheduler(void *rsp){

    if(currentProcess->state == READY && stopInanition > 0){
        stopInanition--;
        return rsp;
    }

    stopinanition = currentProcess->priority;



    

}


//----------------------------------------------

static uint64_t sampleRSP;

static uint64_t getNewPID()
{
      return newPID++;
}

void saveSampleRSP(uint64_t rsp) {
    sampleRSP = rsp;
}


uint64_t getSampleRSP() {
    return sampleRSP;
}


//---------------------------------

void initProcessQueue(){

    processQueue = malloc(QueueADT);
     if (processesQueue == NULL)
            return;
    processQueue->head = NULL;
    processQueue->tail = processQueue->head;
    processesQueue->size = 0;

}

void _createProcess(void (*fn)(int, char **), int foreground, char* name, int priority, int argc, char **argv, unsigned int state
,int* fd) {
    
    PCB* newProcess;

    newProcess->pid = getNewPID();

    if(currentProcessPCB == NULL){
        newProcess->ppid = 0;
    }
    else{
        newProcess->ppid = currentProcessPCB->pid;
    }
    
    newProcess->foreground = foreground;
    newProcess->name = name;

    newProcess->rbp = malloc(STACK_SIZE);
    newProcess->rbp = (void *)((char *)newProcess->rbp + STACK_SIZE - 1);
    newProcess->rsp = (void *)((char *)newProcess->rbp - 1);

    newProcess->priority = priority;
    newProcess->fd[0] = fd ? fd[0] : 0;
    newProcess->fd[1] = fd ? fd[1] : 1;

    newProcess->state = state;
    newProcess->argc = argc;
    newProcess->argv = argv;
  
    _initialize_stack_frame(fn, newProcess->rsp);
}

void newStack(uint64_t rsp) {
    processesStack[activeProcesses++] = rsp;
}

uint64_t preserveStack(uint64_t rsp) {
    if (currentProcess != -1) {
        processesStack[currentProcess] = rsp;
    }
    if (++currentProcess >= activeProcesses) currentProcess = 0;
    if (activeProcesses == 0) return 0;
    return processesStack[currentProcess];
}


void cleanProcesses() {
    activeProcesses = 0;
    currentProcess = -1;
}

