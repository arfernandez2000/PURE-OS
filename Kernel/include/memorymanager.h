#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "naiveConsole.h"
#include <stdint.h>

void *mallocMM(unsigned nbytes);
void memInit(char *memBase, unsigned long memSize);
void freeMM(void *ap);

#endif