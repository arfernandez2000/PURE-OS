#ifndef PROCESS_COM_H
#define PROCESS_COM_H

#include <stdint.h>
#include "shell.h"

void ps();
void yield();
int kill(uint64_t);
int block(uint64_t);
int unblock(uint64_t);
int nice(uint64_t,uint64_t);
void exit();

#endif