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
    if (atoi(argv[0],1)){ 
       block(0);
    }
    uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);
    int loop = 1;
    while (loop)
    {
        waitCycles(10);
        char buff[20];
        addText("Hello Nigerian Prince :) ");
        addText(itoa(pid, buff, 10));
        printWindow();
        substractLine();
        //TODO Ari scanearlo y sacarlo del loop
    }
    exit();
}

void loop(int fg){
    char buffer[10];
    char* argv[] = {"loop", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&loopProc, 2, argv, fg, NULL);
    if(error == -1){
        addText("Error al crear el proceso");
    }
}

