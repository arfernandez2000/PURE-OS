


#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "semaphores.h"
#include "memorymanager.h"


uint32_t pOpen(uint32_t pipeId);
int pClose(uint32_t pipeId);
int pRead(uint32_t pipeId);
uint32_t pWrite(uint32_t pipeId, char *str);
uint32_t putCharPipe(uint32_t pipeId, char c);

#endif