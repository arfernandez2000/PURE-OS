// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include "libc.h"
#include "shell/include/shell.h"

#define NULL (void* )0


int main() {
    winClear();

    char *argv[] = {"Shell"};
    shell(1,argv);

	return 1; 
}