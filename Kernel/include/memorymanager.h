#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

void * mallocMM(unsigned nbytes);
void memInit(void * startingPoint);
void freeMM(void *ap);

#endif