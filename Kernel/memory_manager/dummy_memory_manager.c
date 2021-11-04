// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "memorymanager.h"
char * ptr = 0x600000;

void memInit(void * startingPoint){

     ptr = (char *) startingPoint;
}

char * mallocMM(unsigned int bytes){
     char * aux = ptr;
     ptr += bytes;
     return aux;
}
int freeMM(void *freeMem){
       return 0;
}