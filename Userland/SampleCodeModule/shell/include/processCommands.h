#ifndef PROCESS_COM_H
#define PROCESS_COM_H
#include <stdint.h>

void ps();
void kill(uint64_t);
void block(uint64_t);
void unblock(uint64_t);

#endif