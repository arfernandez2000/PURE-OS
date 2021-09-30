#include "memorymanager.h"
char * ptr = 0x60000000;

void memInit(void * startingPoint){

    ptr = (char *) startingPoint;
}

char * alloc(unsigned int bytes){
    char * aux = ptr;
    ptr += bytes;
    return aux;
}

