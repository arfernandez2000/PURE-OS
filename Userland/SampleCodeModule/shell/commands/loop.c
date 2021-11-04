// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "loop.h"
#include "system.h"
#include "shell.h"
#include "libc.h"
#include "processCommands.h"
#include "sem.h"

#define NULL (void *)0
#define SEM_SHELL 104

int ticksElapsed()
{
    return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}

int waitCycles(int cycles, int fg)
{
    int goal = ticksElapsed() + cycles;
    char c;
    while (ticksElapsed() < goal) {
        if (fg == 1 && (c = getChar()) == '\t')
            return 1;
    }
    return 0;
}

void loopProc(int argc, char **argv)
{
    int fg = atoi(argv[0], 1);
    uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);
    int loop = 1;
    while (loop) {
        if (waitCycles(10, fg) == 1) {
            break;
        }
        char buff[20];
        addText("Hello Nigerian Prince :) ");
        addText(itoa(pid, buff, 10));
        printWindow();
        substractLine();
    }
    if(fg){
        if(sPost(SEM_SHELL) == -1){
            return;
        };
        if(sClose(SEM_SHELL) == -1){
            return;
        }   
    }
    exit();
}

void loop(int fg)
{
    char buffer[10];
    if(fg){
        if(sOpen(SEM_SHELL, -1) == -1)
            return;
    }
    char *argv[] = {"loop", itoa(fg, buffer, 10)};
    int error = sys_loadProcess(&loopProc, 2, argv, fg, NULL);
    if (error == -1)
    {
        addText("Error al crear el proceso");
    }
    if(fg)
        if(sWait(SEM_SHELL) == -1){
            return;
        };
}
