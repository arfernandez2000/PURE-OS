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
int kill(uint64_t pid){
    return syscall(KILL, pid, 0, 0, 0, 0, 0);

}
int block(uint64_t pid){
    return syscall(BLOCK, pid, 0, 0, 0, 0, 0);
}
int unblock(uint64_t pid){
    return syscall(UNBLOCK, pid, 0, 0, 0, 0, 0);
}
int nice(uint64_t pid, int priority){
    return syscall(NICE, pid, priority,0,0,0,0);
}