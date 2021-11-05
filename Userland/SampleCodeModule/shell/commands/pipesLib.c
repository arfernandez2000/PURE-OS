// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "pipesLib.h"

uint64_t pOpen(uint64_t id) { return syscall(PIPE_OPEN, id, 0, 0, 0, 0, 0); }

char *pRead(uint64_t id) {
  return (char *)syscall(PIPE_READ, id, 0, 0, 0, 0, 0);
}

int pWrite(uint64_t id, char *str) {
  return syscall(PIPE_WRITE, id, (uint64_t)str, 0, 0, 0, 0);
}

int pClose(uint64_t id) { return syscall(PIPE_CLOSE, id, 0, 0, 0, 0, 0); }

void pipe() {

  char **result = (char **)syscall(PIPE_DUMP, 0, 0, 0, 0, 0, 0);
  int linesDump = syscall(DUMP_LINES_PIPE, 0, 0, 0, 0, 0, 0);
  if (linesDump == 1) {
    addText("No active pipes");
    substractLine();
    printWindow();
    return;
  }
  int i = 0;
  while (i < linesDump) {
    addText(result[i]);
    substractLine();
    printWindow();
    i++;
  }
  sys_free(result);
  return;
}