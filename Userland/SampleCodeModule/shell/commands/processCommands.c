#include "shell.h"
#include "libc.h"
#include "system.h"

void ps(char* window, int* offset) {
    syscall(PS, 0, 0, 0, 0, 0, 0);
}