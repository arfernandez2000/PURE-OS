#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <defs.h>

#define MAX_PROCESSES 64
#define MAX_NAME_LENGHT 7

typedef struct
{
      uint64_t pid;
      uint64_t ppid;
      int foreground;
      char *name;
      uint64_t rsp;
      uint64_t rbp;
      int priority;
      int argc;
      char **argv;
      unsigned int state;
      int pipes[2];

} PCB;
PCB* createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);
uint64_t scheduler(uint64_t lastRSP);
uint64_t preserveStack(uint64_t rsp);
void newProcessStack(void (*fn), int argc, char ** argv, PCB* newProcess);
void newStack(uint64_t rsp);
void cleanProcesses();
int getProcessCount();
void initScheduler();
int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);
uint64_t _initialize_stack_frame(void * rip, const void * rsp, int argc, char** argv);
char** psDisplay();
void saveSampleRSP(uint64_t rsp);
uint64_t getSampleRSP();
uint64_t getPID();
int* getPipes();

void yield();
static int argsCopy(char **buffer, char **argv, int argc);

#endif