#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <defs.h>

#define MAX_PROCESSES 2

#define READY 1
#define BLOCKED 2
#define KILLED 3


typedef struct
{
      uint64_t pid;
      uint64_t ppid;
      int foreground;
      char name[30];
      void *rsp;
      void *rbp;
      int priority;
      int argc;
      char **argv;
      unsigned int state;
      int fd[2];

} PCB;

uint64_t loadProcess(uint64_t rsp, void (*fn), uint64_t rbp);
uint64_t preserveStack(uint64_t rsp);
void newProcess(void (*fn),uint64_t pid, uint64_t ppid, int foreground, char name[30], void *rsp, void *rbp, int priority, int argc, char **argv,     unsigned int state
,int fd[2]);
void newStack(uint64_t rsp);
void cleanProcesses();

void * _initialize_stack_frame(void * rip, const void * rsp);

void saveSampleRSP(uint64_t rsp);
uint64_t getSampleRSP();

#endif