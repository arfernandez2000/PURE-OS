#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdint.h>
#include "naiveConsole.h"

void * mallocMM(unsigned nbytes);
void memInit(char *memBase, unsigned long memSize);
void freeMM(void *ap);

#endif