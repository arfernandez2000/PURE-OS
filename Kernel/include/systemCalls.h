#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

uint64_t write(uint64_t, uint64_t, uint64_t);
uint64_t read(uint64_t, uint64_t, uint64_t);
uint64_t getTime(uint64_t, uint64_t, uint64_t);
void createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);

#endif