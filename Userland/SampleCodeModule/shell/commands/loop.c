#include "loop.h"
#include "system.h"
#include "shell.h"
#include "libc.h"


int ticksElapsed()
{
      return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}

void waitCycles(int cycles)
{
      int goal = ticksElapsed() + cycles;
      while (ticksElapsed() < goal);
}
void loop(char* window, int * offset){

    uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);

    while (1)
    {
        waitCycles(10);
        char buff[20];
        addText("Hello Nigerian Prince :)", window, offset);
        addText(itoa(pid, buff, 10), window, offset);
        printWindow(window);
        substractLine(window, offset);
    }
}
