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

const int len_void = 11;
const int len_files = 3;
const int len_proc = 3;
char *commands_void[] = {"help", "time", "inforeg", "excdiv", "excop", "clear", "prueba","testMM","ps","loop"};
void (*func []) () = {help, time, inforeg, excdiv, excop, clear,  prueba, test_mm, ps, loop};
char *commands_files[] = {"cat", "wc", "filter"};
void (*func_files []) () = {cat, wc, filter};
char *commands_proc[] = {"kill", "block", "unblock","nice"};
void (*func_proc []) (uint64_t pid) = {kill, block, unblock};
const int len_proc_2 = 1;
char *commands_proc_2[] = {"nice"};
void (*func_proc_2 [])(uint64_t pid, int priority) = {nice};

char window[ROWS * COLS + 1] = {[0 ... ROWS * COLS - 1] = ' ', 0};
int offset = (ROWS - 1) * COLS;



void waitMF(int argc, char** argv){
    while(1);
}
void prueba(){
    char * argv[] = {"prueba"};
    sys_loadProcess(&waitMF, 1, argv , 0, 0);
    printWindow();
}
void substractLine() {
    for (int i = 0; i < ROWS - 1; i++) {
        for (int j = 0; j < COLS; j++) {
            window[i * COLS + j] = window[(i + 1) * COLS + j];
        }
    }
    for (int i = 0; i < COLS; i++) {
        window[(ROWS - 1) * COLS + i - 1] = ' ';
        window[ROWS * COLS] = 0;
    }
    offset = (ROWS - 1) * COLS;
}

void addText(char * buffer) {
    while (*buffer != 0) {
        if (offset == ROWS * COLS - 1 || *buffer == '\n') {
           substractLine(); 
           if(*buffer == '\n'){
               *buffer++;
               continue;
           }
        }
        
        window[(offset)++] = *buffer++;
    }
}

void printWindow() {
    printString(window);
}

void scanfNoPrint(char * buffer, int maxSize) {
    char c;
    int i = 0;
    while ((c = getChar()) != '\n' && i < maxSize - 1) {
        if (c != -1) {
            // if (c == '\v')
            //     sys_switchContext();
            // else 
            if (c == '\b' && i > 0) {
                buffer[--i] = ' ';
                window[--(offset)] = ' ';
                printWindow();
            }
            else if (c != 0 && c != '\b') { 
                buffer[i++] = c;
                if (offset == ROWS * COLS - 1) substractLine();
                window[(offset)++] = c;
                printWindow();
            } 
        }
    }
    buffer[i] = '\0';
    if (offset == ROWS * COLS - 1) substractLine();
    window[offset] = ' ';
}

void clearWindow() {
    for (int i = 0; i <= ROWS * COLS; i++) {
        window[i] = ' ';
    }
    window[ROWS * COLS] = 0;
    offset = (ROWS - 1) * COLS;
    printWindow();
}

void shell(int argc, char** argv) {

    printWindow();
    while (1) {
        int comm_flag = 0;
        addText("$> ");
        printWindow();
        char buffer[SIZE] = {0};
        scanfNoPrint(buffer, SIZE);
        substractLine();
        char* tokens[SIZE] = {0};
        int file_comm = 1;
        tokens[0] = strstrip(buffer, ' ');

        tokens[1] = strtokLib(tokens[0], ' ');
        tokens[2] = strtokLib(tokens[1], '\n');
        
        for (int i = 0; i < len_files; i++) {  
            if (!strcmp(tokens[0], commands_files[i])) {
                (*func_files[i])();
                file_comm = 0;
                comm_flag = 1;
                block(0);
            }
        }
        
        if(file_comm){
            for (int i = 2; i < MAX_ARGS; i++) {
                tokens[i] = strtokLib(tokens[i - 1], ' ');
            }
        
            for (int i = 0; i < len_void; i++) {
                if (!strcmp(tokens[0], commands_void[i])) {
                    if (*tokens[1] != 0)
                        incorrect_arg(tokens[0]);
                    else
                        (*func[i])();
                    comm_flag = 1;
                }
            }
        }
        if (!strcmp(tokens[0], "printmem")) {
            if (*tokens[2] != 0 || *tokens[1] == 0)
                incorrect_arg(tokens[0]);
            else {
                int length = Stringlen(tokens[1]);
                printmem(atoi(tokens[1], length));
            }
            comm_flag = 1;
        }
        for(int i = 0; i < len_proc; i++){
            if (!strcmp(tokens[0], commands_proc[i])) {
                if (*tokens[2] != 0 || *tokens[1] == 0)
                    incorrect_arg(tokens[0]);
                else {
                    int length = Stringlen(tokens[1]);
                    (*func_proc[i])(atoi(tokens[1], length));
                }
                comm_flag = 1;
            }
        }
        for(int i = 0; i < len_proc_2; i++){
            if (!strcmp(tokens[0], commands_proc_2[i])) {
                if (*tokens[2] == 0 || *tokens[1] == 0)
                    incorrect_arg(tokens[0]);
                else {
                    int length_1 = Stringlen(tokens[1]);
                    int length_2 = Stringlen(tokens[2]);
                    (*func_proc_2[i])(atoi(tokens[1], length_1), atoi(tokens[2], length_2));
                }
                comm_flag = 1;
            }
        }
        
        if (!comm_flag) {
            if (*tokens[0] != 0)
                incorrect_comm(tokens[0]);
        }
    }
}

void incorrect_comm(char * buffer) {
    addText(buffer);
    addText(" is not a BottlerShell command");
    printWindow();
    substractLine();
}

void incorrect_arg(char * command) {
    addText("Incorrect arguments for command ");
    addText(command);
    printWindow();
    substractLine();
}

char * getWindow(){
    return window;
}

int getOffset(){
    return offset;
}

void setOffset(int of){
    offset = of;
}
