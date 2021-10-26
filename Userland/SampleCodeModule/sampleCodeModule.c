/* sampleCodeModule.c */
#include "libc.h"
#include "shell/include/shell.h"

#define NULL (void* )0

void sys_loadProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);
void sys_switchContext();

int main() {
    winClear();

    char *argv[] = {"Shell"};
    sys_loadProcess(&shell, 2, argv, 1, NULL, "Shell");
    
    sys_switchContext();

	return 1; 
}
