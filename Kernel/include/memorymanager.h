#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

// void * mallocMM(unsigned nbytes);
// void memInit(char *memBase, unsigned long memSize);
// int freeMM(void *ap);
void memInit(void *startingPoint);
char *mallocMM(unsigned int bytes);
int freeMM(void *freeMem);
#endif