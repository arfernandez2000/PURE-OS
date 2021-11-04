#ifndef PIPESLIB_H
#define PIPESLIB_H

#include <stdint.h>
#include "system.h"


uint64_t pOpen(uint64_t id);
int pRead(uint64_t id);
int pWrite(uint64_t id, char *str);
int pClose(uint64_t id);

#endif