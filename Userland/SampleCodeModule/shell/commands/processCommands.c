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

// void ps() {
//     int proccessCount =  syscall(PROCCESS_COUNT, 0, 0, 0, 0, 0, 0);
//     PCB ** array = syscall(PS, 0, 0, 0, 0, 0, 0);
//     addText("Name PID PPID priority foreground rbp rsp state");
//     substractLine();
//     for(int i =0 ; i< proccessCount; i++){ 
//         addText(array[i]->name);
//         addText(' ');
//         addText(array[i]->pid);   
//         addText(' ');
//         addText(array[i]->ppid); 
//         addText(' ');
//         addText(array[i]->priority); 
//         addText(' ');
//         addText(array[i]->foreground); 
//         addText(' ');
//         addText(array[i]->rbp);
//         addText(' ');
//         addText(array[i]->rsp); 
//         addText(' ');
//         char pid[20];
//         addText(itoa(array[i]->pid, pid, 10));
//         addText(' ');
//         char state[20];
//         addText(itoa(array[i]->state, state, 10));
//         substractLine();
//     }
// }

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