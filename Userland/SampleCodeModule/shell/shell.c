#include "libc.h"
#include "help.h"
#include "clear.h"
#include "time.h"
#include "shell.h"
#include "inforeg.h"
#include "printmem.h"
#include "excDiv.h"
#include "excOP.h"
#include "change.h"
#include "files.h"
#include "test_mm.h"
#include "processCommands.h"
#include "loop.h"


#define NULL (void *)0
#define SIZE 100
#define MAX_ARGS 5

#define COLS 80
#define ROWS 25

const int len_void = 10;
const int len_files = 3;
char *commands_void[] = {"help", "time", "inforeg", "excdiv", "excop", "clear", "prueba","testMM","ps","loop"};
void (*func []) (char *, int *) = {help, time, inforeg, excdiv, excop, clear,  prueba, test_mm, ps, loop};
char *commands_files[] = {"cat", "wc", "filter"};
void (*func_files []) (char *, int *, char *) = {cat, wc, filter};


void waitMF(int argc, char** argv){
    while(1){
        printString("messi");
    }
   
}
void prueba(char * window, int * offset){
    sys_loadProcess(&waitMF, 1, 0 , 1, NULL, "Prueba");
    printWindow(window);
}
void substractLine(char * window, int * offset) {
    for (int i = 0; i < ROWS - 1; i++) {
        for (int j = 0; j < COLS; j++) {
            window[i * COLS + j] = window[(i + 1) * COLS + j];
        }
    }
    for (int i = 0; i < COLS; i++) {
        window[(ROWS - 1) * COLS + i - 1] = ' ';
        window[ROWS * COLS] = 0;
    }
    *offset = (ROWS - 1) * COLS;
}

void addText(char * buffer, char * window, int * offset) {
    while (*buffer != 0) {
        if (*offset == ROWS * COLS - 1) substractLine(window, offset);
        window[(*offset)++] = *buffer++;
    }
}

void printWindow(char * window) {
    printString(window);
}

void scanfNoPrint(char * buffer, int maxSize, char * window, int * offset) {
    char c;
    int i = 0;
    while ((c = getChar()) != '\n' && i < maxSize - 1) {
        if (c != -1) {
            // if (c == '\v')
            //     sys_switchContext();
            // else 
            if (c == '\b' && i > 0) {
                buffer[--i] = ' ';
                window[--(*offset)] = ' ';
                printWindow(window);
            }
            else if (c != 0 && c != '\b') { 
                buffer[i++] = c;
                if (*offset == ROWS * COLS - 1) substractLine(window, offset);
                window[(*offset)++] = c;
                printWindow(window);
            } 
        }
    }
    buffer[i] = '\0';
    if (*offset == ROWS * COLS - 1) substractLine(window, offset);
    window[*offset] = ' ';
}

void clearWindow(char * window, int * offset) {
    for (int i = 0; i <= ROWS * COLS; i++) {
        window[i] = ' ';
    }
    window[ROWS * COLS] = 0;
    *offset = (ROWS - 1) * COLS;
    printWindow(window);
}

void shell(int argc, char** argv) {
    char window[ROWS * COLS + 1] = {[0 ... ROWS * COLS - 1] = ' ', 0};
    int offset = (ROWS - 1) * COLS;

    printWindow(window);
    while (1) {
        int comm_flag = 0;
        addText("$> ", window, &offset);
        printWindow(window);
        char buffer[SIZE] = {0};
        scanfNoPrint(buffer, SIZE, window, &offset);
        substractLine(window, &offset);
        char* tokens[SIZE] = {0};
        int file_comm = 1;
        tokens[0] = strstrip(buffer, ' ');

        tokens[1] = strtokLib(tokens[0], ' ');
        tokens[2] = strtokLib(tokens[1], '\n');
        
        for (int i = 0; i < len_files; i++) {  
            if (!strcmp(tokens[0], commands_files[i])) {
                (*func_files[i])(window, &offset, tokens[1]);
                file_comm = 0;
                comm_flag = 1;
            }
        }
        
        if(file_comm){
            for (int i = 1; i < MAX_ARGS; i++) {
                tokens[i] = strtokLib(tokens[i - 1], ' ');
            }
        
            for (int i = 0; i < len_void; i++) {
                if (!strcmp(tokens[0], commands_void[i])) {
                    if (*tokens[1] != 0)
                        incorrect_arg(tokens[0], window, &offset);
                    else
                        (*func[i])(window, &offset);
                    comm_flag = 1;
                }
            }
        }
        if (!strcmp(tokens[0], "printmem")) {
            if (*tokens[2] != 0 || *tokens[1] == 0)
                incorrect_arg(tokens[0], window, &offset);
            else {
                int length = Stringlen(tokens[1]);
                printmem(window, &offset, atoi(tokens[1], length));
            }
            comm_flag = 1;
        }

        if (!comm_flag) {
            if (*tokens[0] != 0)
                incorrect_comm(tokens[0], window, &offset);
        }
    }
}

void incorrect_comm(char * buffer, char* window, int * offset) {
    addText(buffer, window, offset);
    addText(" is not a BottlerShell command", window, offset);
    printWindow(window);
    substractLine(window, offset);
}

void incorrect_arg(char * command, char* window, int * offset) {
    addText("Incorrect arguments for command ", window, offset);
    addText(command, window, offset);
    printWindow(window);
    substractLine(window, offset);
}