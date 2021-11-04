// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "pipesLib.h"

uint64_t pOpen(uint64_t id)
{
    return syscall(PIPE_OPEN, id, 0, 0, 0, 0, 0);
}

int pRead(uint64_t id)
{
    return syscall(PIPE_READ, id, 0, 0, 0, 0, 0);
}

int pWrite(uint64_t id, char *str)
{
    return syscall(PIPE_WRITE, id, (uint64_t)str, 0, 0, 0, 0);
}

int pClose(uint64_t id)
{
    return syscall(PIPE_CLOSE, id, 0, 0, 0, 0, 0);
}