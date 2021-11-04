#ifndef SEM_H
#define SEM_H

uint64_t sOpen(uint64_t id, int initValue);
int sWait(uint32_t id);
int sPost(uint32_t id);
int sClose(uint32_t id);
void sem();


#endif