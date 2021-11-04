// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "libc.h"
#include "shell.h"

static char * info[] = 
    {"clear: Clear the shell window", 
    "help: Print information about commands",
    "ps: Prints a list of processes with relevant information", 
    "loop: Prints the process id with a message every few second", //Cambiar few por la cant de segundos
    "kill: Kills a process given its id",
    "nice: Changes process priority. Receives PID and Priority",
    "block: Changes process state between ready and block. Receives pid",
    "sem: Prints list of semaphores with each of their properties",
    "cat: Prints stdin",
    "wc: Prints amount of lines of stdin",
    "filter: Filters vowels from input string",
    "pipe: Prints list of pipes with each of their properties",
    "phylo: Starts diner philosophers dilemma",
    "test_mm: Tests the selected memory manager",
    "test_priority: Tests processes' priority",
    "test_processes: Test block, unblock and kill processes. You'll only be able to run it once",
    "test_sync: Test semaphores and process synchronization",
    "test_no_sync: Test semaphores and process synchronization",
    "To run processes in background, write command&. Commands available for background are:",
    "loop&, cat&, wc&, filter& , phylo&"};
static const int len = 20;

void help() {
    for (int i = 0; i < len; i++) {
        addText(info[i]);
        substractLine();
    }
    printWindow();
}