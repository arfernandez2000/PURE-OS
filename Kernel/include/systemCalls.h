#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define MALLOC 4
#define FREE 5
#define GET_PPID 6
#define INFOREG 7
#define KILL 10
#define BLOCK 11
#define UNBLOCK 12
#define GET_PID 13
#define PS 14
#define NICE 15
#define DUMP_MM 16
#define TICKS_ELAPSED 17
#define SEM_OPEN 18
#define SEM_POST 19
#define SEM_WAIT 20
#define SEM_CLOSE 21
#define YIELD 22
#define DUMP_SEM 23
#define PIPE_OPEN 24
#define PIPE_READ 25
#define PIPE_WRITE 26
#define PIPE_CLOSE 27
#define GET_PIPES 28
#define PIPE_DUMP 29
#define SECONDS_ELAPSED 30
#define PROCESS_COUNT 31
#define SEM_DUMP 32
#define DUMP_LINES 33
#define DUMP_LINES_PIPE 34
#define SHM_OPEN 35

uint64_t write(uint64_t, uint64_t, uint64_t);
uint64_t read(uint64_t, uint64_t, uint64_t);
int createProcess(void (*entryPoint)(int, char **), int argc, char **argv,
                  int fg, int fd[2]);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int unBlockProcess(uint64_t pid);
void ps();
int nice(uint64_t, uint64_t);

#endif