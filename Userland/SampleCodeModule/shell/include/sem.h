#ifndef SEM_H
#define SEM_H

uint64_t sOpen(uint64_t id, uint64_t initValue);
int sWait(uint32_t id);
int sPost(uint32_t id);
int sClose(uint32_t id);

#endif