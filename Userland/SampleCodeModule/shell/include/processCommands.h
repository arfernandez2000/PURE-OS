#ifndef PROCESS_COM_H
#define PROCESS_COM_H

#include <stdint.h>
#include "shell.h"

void ps();
int kill(uint64_t);
int block(uint64_t);
int unblock(uint64_t);
int nice(uint64_t,uint64_t);

#endif