#include "../include/test_prio.h"

#define MINOR_WAIT 10000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                            // TODO: Long enough to see theese processes beeing run at least twice

uint64_t my_getpid_prio(){
  return syscall(GET_PID,0,0,0,0,0,0);
}

uint64_t my_create_process_prio(char * name){
  char * argv[]= {name};
  return sys_loadProcess(&endless_loop_prio, 1, argv,0,0);
}

uint64_t my_nice_prio(uint64_t pid, uint64_t newPrio){
  return nice(pid,newPrio);
}

uint64_t my_kill_prio(uint64_t pid){
  return kill(pid);
}

uint64_t my_block_prio(uint64_t pid){
  return block(pid);
}

uint64_t my_unblock_prio(uint64_t pid){
  return unblock(pid);
}

void bussy_wait_prio(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop_prio(){
  uint64_t pid = my_getpid_prio();

  while(1){
    char buffer[2];
    addText(itoa(pid,buffer,10));
    printWindow();
    bussy_wait_prio(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;
  int res;
  int currentPID = syscall(PROCESS_COUNT, 0,0,0,0,0,0);

  for(i = 0; i < TOTAL_PROCESSES; i++){
    res =  my_create_process_prio("inf_loop");
    if(res == -1){
      addText("Error creating process");
      substractLine();
      printWindow();
      return;
    }
    pids[i] = currentPID++;
    
  }
    

  bussy_wait_prio(WAIT);
  substractLine();
  addText("CHANGING PRIORITIES...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        res = my_nice_prio(pids[i], 1); 
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }//lowest priority 
        break;

      case 1:
        res = my_nice_prio(pids[i], 2); //medium priority
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 2:
        res = my_nice_prio(pids[i], 3); //highest priority
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
    }
  }

  bussy_wait_prio(WAIT);
  substractLine();
  addText("BLOCKING...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
      res = my_block_prio(pids[i]);
       if(res == -1){
          addText("Error blocking process");
          substractLine();
          printWindow();
          return;
        }

  }
  substractLine();
  addText("CHANGING PRIORITIES WHILE BLOCKED...");
  substractLine();
  printWindow();
  

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        res = my_nice_prio(pids[i], 2); //medium priority
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 1:
        res = my_nice_prio(pids[i], 2); //medium priority
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 2:
        res = my_nice_prio(pids[i], 2); //medium priority
        if(res == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
    }
  }
  substractLine();
  addText("UNBLOCKING...");
  substractLine();
  printWindow();
  

  for(i = 0; i < TOTAL_PROCESSES; i++){
    res = my_unblock_prio(pids[i]);
    if(res == -1){
          addText("Error unblocking process");
          substractLine();
          printWindow();
          return;
        }
  }
  bussy_wait_prio(WAIT);
  substractLine();
  addText("KILLING...");
  substractLine();
  printWindow();
  

  for(i = 0; i < TOTAL_PROCESSES; i++) {
    res = my_kill_prio(pids[i]);
    if (res == -1) {
      addText("Error killing process");
      substractLine();
      printWindow();
      return;
    }
  }
  substractLine();
  addText("Test Successful");
  substractLine();
  printWindow();
}

