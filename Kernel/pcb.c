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

void addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){

    processQueue[activeProcesses] = createPCB(entryPoint, argc,argv,fg,fd,name);
    currentPCB = processQueue[currentProcess];
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
     
    newProcessStack(entryPoint, argc, argv);
    return newProcess;
}


void newProcessStack(void (*fn), int argc, char** argv) {

    uint64_t newStack = mallocMM(STACK_SIZE);
    
    if(newStack == NULL){
       while(1){
           printStringLen(0x30, "Error",5);
       }; 
    }
    processesStack[activeProcesses++] = (uint64_t) _initialize_stack_frame(fn, newStack + STACK_SIZE, argc, &argv[1]);
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

uint64_t getPID(){
    return processQueue[currentProcess]->pid;
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

//TODO Imprimir name y rbp
char** psDisplay() {
    char *processString[100];
    strcpy(processQueue[0]->name ,processString[0]);
    char buff[10];
    for (int i = 0; i < activeProcesses; i++) {
        // strcat(processString[i],processQueue[i]->name);
        // strcat(processString[i],"   ");
        strcat(processString[i], itoa(processQueue[i]->pid, buff, 10,2));
        strcat(processString[i],"       ");
        strcat(processString[i], itoa(processQueue[i]->priority, buff, 10,3));
        strcat(processString[i],"       ");
        strcat(processString[i], itoa(processQueue[i]->foreground, buff, 10,3));
        strcat(processString[i],"       ");
        strcat(processString[i],"0x");
        strcat(processString[i], itoa(processesStack[i], buff, 10,8));
        strcat(processString[i],"       ");
        strcat(processString[i], itoa(processQueue[i]->state, buff, 10,3));
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
static void changeState(uint64_t pid, int state)
{
    if(processQueue[pid]->state != KILLED)
        processQueue[pid]->state = state;
}
void cleanProcesses() {
    activeProcesses = 0;
    currentProcess = -1;
}

void killProcess(uint64_t pid) {
    changeState(pid,KILLED);
}

void blockProcess(uint64_t pid) {
    changeState(pid, BLOCKED);
}

void unBlockProcess(uint64_t pid) {
    changeState(pid,READY);
}
void nice(uint64_t pid, uint64_t priority){
    processQueue[pid]->priority = priority;
}
