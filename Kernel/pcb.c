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
   
    int phylo = 0;
    do {
        phylo = 0;
        if(processQueue[currentProcess]->state!= READY){
            stopInanition = 0;
        }

        if(stopInanition == 0){
            currentProcess = (++currentProcess) % activeProcesses;
            if(processQueue[currentProcess]->ppid != 0 && processQueue[processQueue[currentProcess]->ppid]->state !=READY){
                phylo = 1; //awful fix but we are running out of time . Works temporarily
                continue;
            }
         
            if(processQueue[currentProcess]->state == READY)
                stopInanition = processQueue[currentProcess]->priority;
           
        }
        if(stopInanition > 0 && processQueue[currentProcess]->state == READY){
            stopInanition--;
        }
       

    }while(phylo || processQueue[currentProcess]->state != READY);
    
    return processesStack[currentProcess]; 
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    processQueue[activeProcesses] = createPCB(entryPoint, argc,argv,fg,fd,name);
    return processID++;
}
int argsCopy(char **buffer, char **argv, int argc)
{
      for (int i = 0; i < argc; i++)
      {
            buffer[i] = mallocMM(sizeof(char) * (Stringlen(argv[i]) + 1));
            strcpy(argv[i], buffer[i]);
      }
      return 1;
}

PCB* createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name){
    PCB* newProcess = (PCB*) mallocMM(sizeof(PCB));
    if(newProcess == NULL){
        return NULL; 
    }

    newProcess->foreground = fg;
    newProcess->argc = argc;

    char **argvCopy = (char**)mallocMM(sizeof(char *) * argc);
      if (argvCopy == 0)
            return NULL;
    argsCopy(argvCopy, &argv[1], argc - 1);

    newProcess->argv = argvCopy;

    newProcess->pipes[0] = fd[0];
    newProcess->pipes[1] = fd[1];

    if(strcmp(name,"phylo") == 0){
          newProcess->ppid = atoi(newProcess->argv[1],Stringlen(newProcess->argv[1]));
    }else{
         newProcess->ppid = 0;
    }

    newProcess->pid = processID;
    newProcess->state = READY;
    newProcess->priority = 1;

    newProcess->name = mallocMM(20);
    strcpy(name,newProcess->name);
     
    newProcessStack(entryPoint, argc, argvCopy, newProcess);
    return newProcess;
}

void newProcessStack(void (*fn), int argc, char** argv, PCB* newProcess) {

    uint64_t newStack = (uint64_t) mallocMM(STACK_SIZE);
    newProcess->rbp =  newStack + STACK_SIZE;
    newProcess->rsp = (uint64_t) _initialize_stack_frame(fn, newStack + STACK_SIZE, argc, argv);
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

uint64_t getPPID(){
    return processQueue[currentProcess]->ppid;
}

int* getPipes() {
    return processQueue[currentProcess]->pipes;
}

char** psDisplay() {
    char** processString = (char**) mallocMM(1000);
    for(int i=0; i< activeProcesses; i++){
        processString[i] = " ";
    }
    

    for (int i = 0; i < activeProcesses; i++) {
        processString[i] =  mallocMM(1024);
        char* auxName = NULL;
        char buff[10]={0};
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
    if(activeProcesses > pid){
        if(processQueue[pid]->state !=KILLED){
                processQueue[pid]->state = state;
                return 1;
        }
        else if(processQueue[pid]->state == KILLED && state == KILLED){
            return 1;
        }
    }
    return -1;
    
        
}
void cleanProcesses() {
    activeProcesses = 0;
    currentProcess = -1;
}

int  killProcess(uint64_t pid) {
    int res = changeState(pid,KILLED);
    return res;
}

int blockProcess(uint64_t pid) {
    int error = changeState(pid, BLOCKED);
    if(error == -1){
        return -1;
    }
    if(getPID() == pid)
        yield();
    return 1;
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
void yield(){
    stopInanition = 0;
    _callTick();
}
