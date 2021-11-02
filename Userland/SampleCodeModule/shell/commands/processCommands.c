#include "system.h"
#include "shell.h"

void ps() {
    addText("Name      PID     priority    foreground     rbp          rsp          state");
    substractLine();
    printWindow();
    char** result = (char**) syscall(PS,0,0,0,0,0,0);
    int processCount =  syscall(PROCESS_COUNT, 0, 0, 0, 0, 0, 0);
    for (int i = 0; i < processCount; i++) {
        addText(result[i]);
        substractLine();
        printWindow();
    }
}
void kill(uint64_t pid){
    syscall(KILL, pid, 0, 0, 0, 0, 0);

}
void block(uint64_t pid){
    syscall(BLOCK, pid, 0, 0, 0, 0, 0);
}
void unblock(uint64_t pid){
    syscall(UNBLOCK, pid, 0, 0, 0, 0, 0);
}
void nice(uint64_t pid, int priority){
    syscall(NICE, pid, priority,0,0,0,0);
}

void exit(){
    uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);
    kill(pid);
    while (1);
}

void yield(){
    syscall(YIELD, 0, 0, 0, 0, 0, 0);
}