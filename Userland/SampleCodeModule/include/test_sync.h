#ifndef test_sync_h
#define test_sync_h

#include "shell.h"

uint64_t my_create_process_sync(char *name);
uint64_t my_sem_open_sync(char *sem_id, uint64_t initialValue);
uint64_t my_sem_wait_sync(char *sem_id);
uint64_t my_sem_post_sync(char *sem_id);
uint64_t my_sem_close_sync(char *sem_id);
void slowInc_sync(int64_t *p, int64_t inc);
void inc_sync(uint64_t sem, int64_t value, uint64_t N);
void test_sync();
void test_no_sync();

#endif