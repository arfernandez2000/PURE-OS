#ifndef phylo_h
#define phylo_h

#include "system.h"
#include "libc.h"
#include "shell.h"

void phylo( int fg);
void take_forks(int i);
void put_forks(int i);
void test(int i);
void philosopher(int i);
void addInitialPhylos(int fg);
void table(int argc, char** argv);


#endif