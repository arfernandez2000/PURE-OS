#include "loop.h"
#include "system.h"
#include "shell.h"
#include "libc.h"
#include "processCommands.h"

#define NULL (void*) 0 


int ticksElapsed()
{
      return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}

void waitCycles(int cycles)
{
      int goal = ticksElapsed() + cycles;
      while (ticksElapsed() < goal);
}

void loopProc(int argc, char** argv){

    uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);
    while (1)
    {
        waitCycles(10);
        char buff[20];
        addText("Hello Nigerian Prince :) ");
        addText(itoa(pid, buff, 10));
        printWindow();
        substractLine();
       
    }
}

void loop(int fg){
    char* argv[] = {"loop"};
    if(fg){
        block(0);
    }
    int error = sys_loadProcess(&loopProc, 1, argv, fg, NULL);
    if(error == -1){
        addText("Error al crear el proceso");
    }
    exit();
    unblock(0);
}

