#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

void * mallocMM(unsigned nbytes);
// void memInit(void * startingPoint);
void memInit(char *memBase, unsigned long memSize);
void freeMM(void *ap);

#endif