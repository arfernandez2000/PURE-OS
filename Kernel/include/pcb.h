#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <defs.h>

#define MAX_PROCESSES 64

typedef struct
{
      uint64_t pid;
      uint64_t ppid;
      int foreground;
      char *name;
      void *rsp;
      void *rbp;
      int priority;
      int argc;
      char **argv;
      unsigned int state;
      int pipes[2];

} PCB;
PCB* createPCB(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);
void* scheduler(void * lastRSP);
uint64_t loadProcess(uint64_t rsp, void (*fn), uint64_t rbp);
uint64_t preserveStack(uint64_t rsp);
void newProcessStack(void (*fn));
void newStack(uint64_t rsp);
void cleanProcesses();

void * _initialize_stack_frame(void * rip, const void * rsp);

void saveSampleRSP(uint64_t rsp);
uint64_t getSampleRSP();

#endif