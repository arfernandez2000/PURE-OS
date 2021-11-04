#ifndef PIPESLIB_H
#define PIPESLIB_H

#include "system.h"
#include <stdint.h>

uint64_t pOpen(uint64_t id);
char *pRead(uint64_t id);
int pWrite(uint64_t id, char *str);
int pClose(uint64_t id);

#endif