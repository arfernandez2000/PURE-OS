#include "libc.h"
#include "shell.h"

static char * info[] = 
    {"clear: Clear the shell window", 
    "excdiv: Throw exception 0 (zero division)", 
    "excop: Throw exception 6 (opcode error)", 
    "help: Print information about commands", 
    "inforeg: Print values of registers",
    "printmem: Prints 32 bytes of memory, starting at a given direction", 
    "time: Prints date and time in UTC", 
    "ps: Prints a list of processes with relevant information", 
    "loop: Prints the process id with a message every few second", //Cambiar few por la cant de segundos
    "kill: Kills a process given its id",
    "nice: Changes process priority",
    "block: Changes process state between ready and block",
    "sem: Prints list of semaphores with each of their properties",
    "cat: Prints stdin",
    "wc: Prints amount of lines of stdin",
    "filter: Prints amount of vowels from stdin",
    "pipe: Prints list of pipes with each of their properties",
    "phylo: Starts diner philosophers dilemma",
    "testMM: Tests the selected memory manager", };
static const int len = 19;

void help() {
    for (int i = 0; i < len; i++) {
        addText(info[i]);
        substractLine();
    }
    printWindow();
}