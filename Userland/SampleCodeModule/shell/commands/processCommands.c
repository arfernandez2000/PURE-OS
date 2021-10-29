#include "system.h"


void ps() {
    syscall(PS, 0, 0, 0, 0, 0, 0);
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