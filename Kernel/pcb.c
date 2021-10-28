#include "pcb.h"
#include "defs.h"
#include <memorymanager.h>
#include "lib.h"
#include "video.h"
#include "naiveConsole.h"


int activeProcesses = 0;
int currentProcess = -1;

uint64_t processesStack[MAX_PROCESSES];
PCB* processQueue[MAX_PROCESSES];
int stopInantion = 0;
PCB* currentPCB = NULL;
int processID = 0;


uint64_t entre1era = 0;

void initScheduler(uint64_t rsp){
    entre1era = rsp;
}

uint64_t scheduler(uint64_t lastRSP){
//    printStringLen(0x20,"a",1);
    if(!entre1era){
        entre1era = 1;
        //printStringLen(0x20,"aaaa",4);
        return processesStack[0];
    }
    if(currentProcess < getProcessCount() && currentProcess > 1)
        return processesStack[currentProcess++]; //current vamos a usar para iterar
    return lastRSP;
////    if(currentPCB){
////        if(currentPCB->state == READY && stopInantion > 0){
////            stopInantion--;
////            return lastRSP;
////        }
////        //Cambiar de proceso
////        currentPCB->rsp = lastRSP;
////    }
////    //setear la prioridad y crear el proceso, y devolver el nuevo rsp
////    stopInantion = currentPCB->priority;
////
//    return currentPCB->rsp;
}

void addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    //printStringLen(0x01,"B",1);
    //printStringLen(0x01,"C",1);
    processQueue[activeProcesses] = createPCB(entryPoint, argc,argv,fg,fd,name);
    processQueue[currentProcess]->priority = 1;
    processQueue[currentProcess]->state = READY;
    currentPCB = processQueue[currentProcess];
    // entryPoint(argc, argv);
}


PCB* createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    
    PCB* newProcess = mallocMM(sizeof(PCB));
    if(newProcess == NULL){
        return NULL; 
    }

    newProcess->foreground = fg;
    newProcess->argc = argc;
    newProcess->argv = argv;
    newProcess->pipes[0] = fd[0];
    newProcess->pipes[1] = fd[1];
    if(processID == 0){
        newProcess->pid = processID;
        newProcess->ppid = processID++;
    }
    else{
        newProcess->ppid = processID;
        newProcess->pid = ++processID;
    }
    

    strcpy(newProcess->name, name);
    //arrastre error 
    newProcessStack(entryPoint);
    return newProcess;
}


void newProcessStack(void (*fn)) {

    uint64_t newStack = mallocMM(STACK_SIZE);
    
    if(newStack == NULL){
       while(1){
           printStringLen(0x30, "Error",5);
       }; 
    }

    processesStack[activeProcesses++] = (uint64_t) _initialize_stack_frame(fn, newStack + STACK_SIZE);
}


// void newStack(uint64_t rsp) {
//     processesStack[activeProcesses++] = rsp;
// }

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
int getProcessCount(){
    return activeProcesses;
}
void printProcess(PCB *process)
{
        //TODO> numbers to string ;
      if (process != NULL){
            char * pid = process->pid;
            char * foreground = process->foreground;
            char * rsp = process->rsp;
            char * state = process->state;
            char * name = process->name;
      }
              
}

void psDisplay()
{
    //   printStringLen(0x02,"PID      FG       RSP              RBP              STATE        NAME", Stringlen("PID      FG       RSP              RBP              STATE        NAME") );

    
    //   for(int i = 0; i< activeProcesses; i++)
    //   {
    //         printProcess(processQueue[i]);
    //   }

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

void killProcess(uint64_t pid) {
    ;
}

void blockProcess(uint64_t pid) {
    ;
}

void unBlockProcess(uint64_t pid) {

}
