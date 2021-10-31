#ifndef test_prio_h
#define test_prio_h

#include <stdint.h>
#include "../include/system.h"
#include "../shell/include/processCommands.h"
#include "../include/libc.h"
#include "../shell/include/shell.h"
void endless_loop_prio();
uint64_t my_create_process_prio(char * name);
uint64_t my_nice_prio(uint64_t pid, uint64_t newPrio);
uint64_t my_kill_prio(uint64_t pid);
uint64_t my_block_prio(uint64_t pid);
uint64_t my_unblock_pr(uint64_t pid);
void bussy_wait_prio(uint64_t n);
void endless_loop_prio();
void test_prio();


#endif