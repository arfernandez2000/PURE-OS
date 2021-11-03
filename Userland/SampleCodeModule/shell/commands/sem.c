#include "system.h"

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
