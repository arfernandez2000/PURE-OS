#include "files.h"
#include "libc.h"
#include "shell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 1040
#define COLS 40

int openFile(char *file);

void cat (char * window, int * offset, char * input){
    addText(input, window, offset);
    printWindow(window);
    substractLine(window, offset);
}

void wc (char * window, int * offset, char *input) {
    int lines = ((Stringlen(input) + 6) / COLS) + 1; //agrego lo que ocupa el $> wx
    char ret[BUFF_SIZE] = {0};
    addText(itoa(lines, ret, 10), window, offset);
    printWindow(window);
    substractLine(window, offset);
}

void filter (char * window, int * offset, char *input) {
    int vocals = 0;
    while(*input != '\0'){
        if(*input == 'a' || *input == 'e' || *input == 'i' || *input == 'o' || *input == 'u' ||
            *input == 'A' || *input == 'E' || *input == 'I' || *input == 'O' || *input == 'U'){
            vocals++;
        }
        *input++;
    }
    char ret[BUFF_SIZE] = {0};
    addText(itoa(vocals, ret, 10), window, offset);
    printWindow(window);
    substractLine(window, offset);
}
