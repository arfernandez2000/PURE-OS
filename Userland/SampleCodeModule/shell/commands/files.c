#include "files.h"
#include "libc.h"
#include "shell.h"
#include "processCommands.h"
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
void scanning(char* buffer);

void cat (){
    char* argv[] = {"cat"};
    int error = sys_loadProcess(&catProc, 1, argv, 0, NULL);
    if(error == -1){
        addText("Error al crear el proceso");
    }
}

void catProc(int argc, char** argv){
    printWindow();
    substractLine();
    char buffer[BUFF_SIZE] = {0};
    block(0);
        scanning(buffer);
    unblock(0);
    addText(buffer);
    printWindow();
    substractLine();
}

void scanning(char* buffer){
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
                else if (c != 0 && c != '\b') { 
                    buffer[i++] = c;
                    if (offset == ROWS * COLS - 1) substractLine();
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
    // int lines = ((Stringlen(input) + 6) / COLS) + 1; //agrego lo que ocupa el $> wx
    // char ret[BUFF_SIZE] = {0};
    // addText(itoa(lines, ret, 10));
    printWindow();
    substractLine();
}

void filter () {
    int vocals = 0;
    // while(*input != '\0'){
    //     if(*input == 'a' || *input == 'e' || *input == 'i' || *input == 'o' || *input == 'u' ||
    //         *input == 'A' || *input == 'E' || *input == 'I' || *input == 'O' || *input == 'U'){
    //         vocals++;
    //     }
    //     *input++;
    // }
    char ret[BUFF_SIZE] = {0};
    addText(itoa(vocals, ret, 10));
    printWindow();
    substractLine();
}
