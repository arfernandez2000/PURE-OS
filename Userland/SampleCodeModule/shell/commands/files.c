// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "files.h"
#include "libc.h"
#include "shell.h"
#include "processCommands.h"
#include "system.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sem.h"
#include "pipesLib.h"

#define BUFF_SIZE 1024

#define NULL (void *)0
#define SIZE 100
#define MAX_ARGS 5
#define SEM_SHELL 103

#define COLS 80
#define ROWS 25

//pipe 0 write
//pipe 1 read

int openFile(char *file);

void catProc(int argc, char **argv);
void wcProc(int argc, char **argv);
void filterProc(int argc, char **argv);
void scanning(char *buffer, int filterVow);
int isVow(char c);

void cat(int fg, int *pipes)
{
    char buffer[10];
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1))){
        if(sOpen(SEM_SHELL, -1) == -1)
            return;
    }
    char* argv[] = {"cat", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&catProc, 2, argv, fg, pipes);
    if(error == -1){
        addText("Error al crear el proceso");
    }
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1)))
        if(sWait(SEM_SHELL) == -1){
          return ;
        };
}

void catProc(int argc, char **argv)
{
    if (!atoi(argv[0], 1))
    {
        while (1);
    }
    int *pipes =  (int*) syscall(GET_PIPES, 0, 0, 0, 0, 0, 0);
    
    char buffer[BUFF_SIZE] = {0};

    if (pipes[0] >= 0 && pipes[1] == -1) {
        scanning(buffer, 0);
        substractLine();
        printWindow();
        pWrite(pipes[0], buffer);
        exit();
    }
    else if (pipes[0] == -1 && pipes[1] >= 0) {
        int i;
        char* c;
        strcpy2(pRead(pipes[1]), c);
        while (*c != '\0')
        {
            buffer[i++] = c[0];
            strcpy2(pRead(pipes[1]), c);
        }
        buffer[i] = '\0';
    }
    else {
        scanning(buffer, 0);
    }
    substractLine();
    addText(buffer);
    substractLine();
    printWindow();
    if(sPost(SEM_SHELL)==-1){
      return;
    };
    if(sClose(SEM_SHELL) == -1){
      return;
    };
    exit();
}

void scanning(char *buffer, int filterVow)
{
    char c;
    int i = 0;
    char *window = getWindow();
    int offset = getOffset();
    while ((c = getChar()) != '\t' && i < BUFF_SIZE - 1)
    {
        if (c != -1)
        {
            if (c == '\b' && i > 0)
            {
                buffer[--i] = ' ';
                window[--offset] = ' ';
                setOffset(offset);
                printWindow();
            }
            if (c == '\n')
            {
                buffer[i++] = c;
                substractLine();
                offset = getOffset();
                printWindow();
            }
            else if (c != 0 && c != '\b')
            {
                if (!filterVow || !isVow(c))
                {
                    buffer[i++] = c;
                }
                if (offset == ROWS * COLS - 1)
                {
                    substractLine();
                    offset = getOffset();
                }
                window[offset++] = c;
                setOffset(offset);
                printWindow();
            }
        }
    }
    buffer[i] = '\0';
    if (offset == ROWS * COLS - 1)
        substractLine();
    window[offset] = ' ';
}

void wc(int fg, int *pipes)
{
    char buffer[10];
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1))){
        if(sOpen(SEM_SHELL, -1) == -1)
            return;
    }
    char* argv[] = {"wc", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&wcProc, 2, argv, fg, pipes);
    if(error == -1){
      return;
    }
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1)))
        if(sWait(SEM_SHELL) == -1){
          return;
        };
}

void wcProc(int argc, char **argv)
{
    if (!atoi(argv[0], 1)) {
        while (1);
    }

    int *pipes = (int*) syscall(GET_PIPES, 0, 0, 0, 0, 0, 0);

    char buffer[BUFF_SIZE] = {0};

    if (pipes[0] == -1 && pipes[1] >= 0) {
        int i;
        char* c;
        strcpy2(pRead(pipes[1]), c);
        while (*c != '\0')
        {
            buffer[i++] = c[0];
            strcpy2(pRead(pipes[1]), c);
        }
        
        buffer[i] = '\0';
    }
    else {
        scanning(buffer, 0);
    }
    substractLine();
    int lines = 1;
    for (size_t i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\n')
        {
            lines++;
        }
    }
    char ret[BUFF_SIZE] = {0};
    if(pipes[0] >= 0){
       pWrite(pipes[0], itoa(lines, ret, 10));
        exit(); 
    }
    addText(itoa(lines, ret, 10));
    substractLine();
    printWindow();
    if(sPost(SEM_SHELL) == -1){
        return;
    }
    if(sClose(SEM_SHELL) == -1){
        return;
    }
    exit();
}

void filter(int fg, int *pipes)
{
    char buffer[10];
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1))){
        if(sOpen(SEM_SHELL, -1) == -1)
            return;
    }
    char* argv[] = {"filter", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&filterProc, 2, argv, fg, pipes);
    if(error == -1){
      return;
    }
    if(fg && (pipes[0] >= 0 || (pipes[0] == -1 && pipes[1] == -1)))
        if(sWait(SEM_SHELL) == -1){
        return;
    }

}

void filterProc(int argc, char **argv)
{
    if (!atoi(argv[0], 1))
    {
        while (1);
    }
    char buffer[BUFF_SIZE] = {0};

    int *pipes = (int*)syscall(GET_PIPES, 0, 0, 0, 0, 0, 0);

    if (pipes[0] == -1 && pipes[1] >= 0) {
        int i;
        char* c;
        strcpy2(pRead(pipes[1]), c);
        while (*c != '\0')
        {
            if(!isVow(c[0]))
                buffer[i++] = c[0];
            strcpy2(pRead(pipes[1]), c);
        }
        
        buffer[i] = '\0';
    }
    else {
        scanning(buffer, 1);
    }
    substractLine();
    if(pipes[0] >= 0){
       pWrite(pipes[0], buffer);
        exit(); 
    }
    addText(buffer);
    substractLine();
    printWindow();
    if(sPost(SEM_SHELL) == -1){
        return;
    }
    if(sClose(SEM_SHELL) == -1){
        return;
    }
    exit();
}

int isVow(char c)
{
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}