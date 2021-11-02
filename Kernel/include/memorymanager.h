#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

char * mallocMM(unsigned int qnbytes);
void memInit(void * startingPoint);
//void memInit(char *memBase, unsigned long memSize);
void freeMM(void *ap);

#endif