// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "shell.h"
#include "system.h"
#include <stdint.h>

void ps() {
  addText("Name      PID     priority    foreground     rbp          rsp       "
          "   state");
  substractLine();
  printWindow();
  char **result = (char **)syscall(PS, 0, 0, 0, 0, 0, 0);
  int processCount = syscall(PROCESS_COUNT, 0, 0, 0, 0, 0, 0);
  for (int i = 0; i < processCount; i++) {
    addText(result[i]);
    substractLine();
    printWindow();
    sys_free(result[i]);
  }
  sys_free(result);
}
int kill(uint64_t pid) { return syscall(KILL, pid, 0, 0, 0, 0, 0); }
int block(uint64_t pid) { return syscall(BLOCK, pid, 0, 0, 0, 0, 0); }
int unblock(uint64_t pid) { return syscall(UNBLOCK, pid, 0, 0, 0, 0, 0); }

int nice(uint64_t pid, int priority) {
  return syscall(NICE, pid, priority, 0, 0, 0, 0);
}
void exit() {
  uint64_t pid = syscall(GET_PID, 0, 0, 0, 0, 0, 0);
  if (kill(pid) == -1) {
    addText("Error killing");
    printWindow();
  };
  while (1)
    ;
}

void yield() { syscall(YIELD, 0, 0, 0, 0, 0, 0); }
