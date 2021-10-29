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