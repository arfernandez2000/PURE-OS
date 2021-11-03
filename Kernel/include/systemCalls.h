#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

uint64_t write(uint64_t, uint64_t, uint64_t);
uint64_t read(uint64_t, uint64_t, uint64_t);
int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2]);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int unBlockProcess(uint64_t pid);
void ps();
int nice(uint64_t,uint64_t);

#endif