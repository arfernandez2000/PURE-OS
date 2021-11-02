#include "files.h"
#include "libc.h"
#include "shell.h"
#include "processCommands.h"
#include "system.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 1024

#define NULL (void *)0
#define SIZE 100
#define MAX_ARGS 5

#define COLS 80
#define ROWS 25

int openFile(char *file);

void catProc(int argc, char** argv);
void wcProc(int argc, char** argv);
void filterProc (int argc, char** argv);
void scanning(char* buffer, int filterVow);
int isVow(char c);

void cat (){
    char* argv[] = {"cat"};
    sys_loadProcess(&catProc, 1, argv, 0, NULL);
}

void catProc(int argc, char** argv){
    printWindow();
    char buffer[BUFF_SIZE] = {0};
        scanning(buffer, 0);
        substractLine();
        addText(buffer);
        substractLine();
        addText("$> ");
        printWindow();
    unblock(0);
    exit();
}

void scanning(char* buffer, int filterVow){
    char c;
        int i = 0;
        char* window = getWindow();
        int offset = getOffset();
        while ((c = getChar()) != '\t' && i < BUFF_SIZE - 1) {
            if (c != -1) {
                if (c == '\b' && i > 0) {
                    buffer[--i] = ' ';
                    window[--offset] = ' ';
                    setOffset(offset);
                    printWindow();
                }
                if (c == '\n'){
                    buffer[i++] = c;
                    substractLine();
                    offset = getOffset();
                    printWindow();
                }
                else if (c != 0 && c != '\b') { 
                    if(!filterVow || !isVow(c)){
                        buffer[i++] = c;
                    }
                    if (offset == ROWS * COLS - 1) {
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
        if (offset == ROWS * COLS - 1) substractLine();
        window[offset] = ' ';
}

void wc () {
    char* argv[] = {"wc"};
    sys_loadProcess(&wcProc, 1, argv, 0, NULL);
}

void wcProc(int argc, char** argv){
    printWindow();
    substractLine();
    char buffer[BUFF_SIZE] = {0};
    block(0);
        scanning(buffer, 0);
        substractLine();
        int lines = 1;
        for (size_t i = 0; buffer[i] != '\0'; i++)
        {
            if(buffer[i] == '\n'){
                lines++;
            }
        }
        char ret[BUFF_SIZE] = {0};
        addText(itoa(lines, ret, 10));
        substractLine();
        addText("$> ");
        printWindow();
    unblock(0);
    exit();
}

void filter(){
    char* argv[] = {"filter"};
    sys_loadProcess(&filterProc, 1, argv, 0, NULL);
}

void filterProc (int argc, char** argv) {
    printWindow();
    char buffer[BUFF_SIZE] = {0};
    block(0);
        scanning(buffer, 1);
        substractLine();
        addText(buffer);
        substractLine();
        addText("$> ");
        printWindow();
    unblock(0);
    exit();
}

int isVow(char c){
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}
