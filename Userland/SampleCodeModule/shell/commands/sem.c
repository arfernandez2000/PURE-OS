// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "system.h"
#include "shell.h"

uint64_t sOpen(uint64_t id, int initValue){
    return syscall(SEM_OPEN, id, initValue, 0, 0, 0, 0);
}
int sWait(uint32_t id){
    return syscall(SEM_WAIT, id, 0, 0, 0, 0, 0);
}
int sPost(uint32_t id){
    return syscall(SEM_POST, id, 0, 0, 0, 0, 0);
}
int sClose(uint32_t id){
    return syscall(SEM_CLOSE, id, 0, 0, 0, 0, 0);
}
void sem(){
    char** result =  (char**)syscall(SEM_DUMP,0,0,0,0,0,0);
    int linesDump = syscall(DUMP_LINES,0,0,0,0,0,0);
    if(linesDump == 1){
        addText("No active semaphores");
        substractLine();
        printWindow();
        return;
    }
    int i=0;
    while(i < linesDump){
        addText(result[i]);
        substractLine();
        printWindow();
        i++;
    }
    return;

}
