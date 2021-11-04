// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "loop.h"
#include "libc.h"
#include "processCommands.h"
#include "sem.h"
#include "shell.h"
#include "system.h"
#include "pipesLib.h"

#define NULL (void *)0
#define SEM_SHELL 104
#define SEM_SHELL_FILES 103
#define BUFF_SIZE 1024

int ticksElapsed() { return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0); }

int waitCycles(int cycles, int fg) {
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

    int *pipes = (int*)syscall(GET_PIPES, 0, 0, 0, 0, 0, 0);

    char message[BUFF_SIZE];

    if (pipes[0] == -1 && pipes[1] >= 0) {
        int i;
        char* c;
        strcpy2(pRead(pipes[1]), c);
        while (*c != '\0')
        {
            message[i++] = c[0];
            strcpy2(pRead(pipes[1]), c);
        }
        
        message[i] = '\0';
    }
    else{
        strcpy2("Hello Nigerian Prince :) ", message);
    }
    int loop = 1;
    while (loop) {
        if (waitCycles(10, fg) == 1) {
            break;
        }
        char buff[20];
        addText(message);
        addText(itoa(pid, buff, 10));
        printWindow();
        substractLine();
        
    }
    if(fg && pipes[0] == -1 && pipes[1] == -1){
        if (sPost(SEM_SHELL) == -1) {
          return;
        };
        if (sClose(SEM_SHELL) == -1) {
          return;
        }
    }
    if(fg && pipes[1] >= 0){
      if (sPost(SEM_SHELL_FILES) == -1) {
        return;
      }
      if (sClose(SEM_SHELL_FILES) == -1) {
        return;
      }
    }
    exit();
}

void loop(int fg, int *pipes)
{
    char buffer[10];
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1))){
        if(sOpen(SEM_SHELL, -1) == -1)
            return;
    }
    char *argv[] = {"loop", itoa(fg, buffer, 10)};
    int error = sys_loadProcess(&loopProc, 2, argv, fg, pipes);
    if (error == -1)
    {
        addText("Error al crear el proceso");
    }
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1)))
        if(sWait(SEM_SHELL) == -1){
          return;
        }
}
