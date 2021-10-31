#include "pcb.h"
#include "defs.h"
#include <memorymanager.h>
#include "lib.h"
#include "video.h"
#include "naiveConsole.h"

int activeProcesses = 0;
int currentProcess = 0;

uint64_t processesStack[MAX_PROCESSES];
PCB* processQueue[MAX_PROCESSES];
int stopInanition = 0;
PCB* currentPCB = NULL;
int processID = 0;


uint64_t entre1era = 0;

void initScheduler(uint64_t rsp){
    entre1era = rsp;
}

uint64_t scheduler(uint64_t lastRSP){
    if(!entre1era){
        entre1era = 1;
        return processesStack[0];
    }
    processesStack[currentProcess] = lastRSP;
   

    do {
        
        if(processQueue[currentProcess]->state!= READY){
            stopInanition = 0;
        }

        if(stopInanition == 0){
            currentProcess = (++currentProcess) % activeProcesses;
            if(processQueue[currentProcess]->state == READY)
                stopInanition = processQueue[currentProcess]->priority;
           
        }
        if(stopInanition > 0 && processQueue[currentProcess]->state == READY){
            stopInanition--;
         }
    }while(processQueue[currentProcess]->state != READY);
    
    return processesStack[currentProcess]; 
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){

    processQueue[activeProcesses] = createPCB(entryPoint, argc,argv,fg,fd,name);
    currentPCB = processQueue[currentProcess];
    if(currentPCB == NULL){
        return -1;
    }
    return 1;
}


PCB* createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    
    PCB* newProcess = mallocMM(sizeof(PCB));
    if(newProcess == NULL){
        return NULL; 
    }

    newProcess->foreground = fg;
    newProcess->argc = argc;
    newProcess->argv = &argv[1];
    newProcess->pipes[0] = fd[0];
    newProcess->pipes[1] = fd[1];
    newProcess->ppid = 0;
    newProcess->pid = processID++;
    newProcess->state = READY;
    newProcess->priority = 1;

    newProcess->name = mallocMM(20);
    strcpy(name,newProcess->name);
     
    newProcessStack(entryPoint, argc, argv, newProcess);
    return newProcess;
}


void newProcessStack(void (*fn), int argc, char** argv, PCB* newProcess) {

    uint64_t newStack = mallocMM(STACK_SIZE);
    
    if(newStack == NULL){
       while(1){
           printStringLen(0x30, "Error",5);
       }; 
    }
    newProcess->rbp =  newStack + STACK_SIZE;
    newProcess->rsp = (uint64_t) _initialize_stack_frame(fn, newStack + STACK_SIZE, argc, &argv[1]);
    processesStack[activeProcesses++] =  newProcess->rsp;
    
    
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
int getProcessCount(){
    return activeProcesses;
}

uint64_t getPID(){
    return processQueue[currentProcess]->pid;
}
//TODO Imprimir name y rbp
char** psDisplay() {
    char** processString = mallocMM(1000);

  
    //strcpy(processQueue[0]->name ,processString[0])
    for (int i = 0; i < activeProcesses; i++) {
        processString[i] =  mallocMM(1024);
        char* auxName = NULL;
        char buff[10]={0};
        //strcpy(processQueue[i]->name, auxName);
        int j;
        int aux;
        for (j = 0; processQueue[i]->name[j] != 0; j++) {
            auxName[j] = processQueue[i]->name[j];
        }
        for (aux = j; aux < MAX_NAME_LENGHT; aux++) {
            auxName[aux] = ' ';
        }
        auxName[aux] = 0;
        strcat(processString[i],auxName);
        strcat(processString[i],"    ");

        
        char * messi = itoa(processQueue[i]->pid, buff, 10,10);
        strcat(processString[i], messi);
        strcat(processString[i],"         ");
        strcat(processString[i], itoa(processQueue[i]->priority, buff, 10,10));
        strcat(processString[i],"            ");
        strcat(processString[i], itoa(processQueue[i]->foreground, buff, 10,10));
        strcat(processString[i],"       ");
        strcat(processString[i],"0x");
        //TODO: ARREGLAR ESTO
        strcat(processString[i], itoa((uint64_t)processQueue[i]->rbp, buff, 10,10));
        strcat(processString[i],"   ");
        strcat(processString[i],"0x");
        strcat(processString[i], itoa(processesStack[i], buff, 10,10));
        strcat(processString[i],"        ");
        strcat(processString[i], itoa(processQueue[i]->state, buff, 10,10));
    };
    return processString;
    }


static uint64_t sampleRSP;

void saveSampleRSP(uint64_t rsp) {
    sampleRSP = rsp;
}

uint64_t getSampleRSP() {
    return sampleRSP;
}
static int changeState(uint64_t pid, int state)
{
    if(activeProcesses > pid && processQueue[pid]->state !=KILLED){
        processQueue[pid]->state = state;
        return 1;
    }
    if (processQueue[pid]->state == KILLED && state == KILLED)
        return 1;
    return -1;
    
        
}
void cleanProcesses() {
    activeProcesses = 0;
    currentProcess = -1;
}

int  killProcess(uint64_t pid) {
    return changeState(pid,KILLED);
}

int blockProcess(uint64_t pid) {
    return changeState(pid, BLOCKED);
}

int unBlockProcess(uint64_t pid) {
    return changeState(pid,READY);
}
int nice(uint64_t pid, uint64_t priority){
    if(activeProcesses < pid || priority <= 0){
        return -1;
    }
    processQueue[pid]->priority = priority;
    return 1;
}

