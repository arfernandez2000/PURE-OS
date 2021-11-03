#ifndef test_processes_h
#define test_processes_h

#include "test_util.h"
#include "processCommands.h"
#include "system.h"
#include "shell.h"

void endless_loop_proc();
uint32_t my_create_process_proc(char * name);
uint32_t my_kill_proc(uint32_t pid);
uint32_t my_block_proc(uint32_t pid);
uint32_t my_unblock_proc(uint32_t pid);
void test_processes();




#endif
