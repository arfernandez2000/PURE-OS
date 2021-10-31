#include "../include/test_prio.h"

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

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
    bussy_wait_prio(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;
  int res, resNice, resNice2, resNice3, resNice4, resNice5, resNice6, res_block, res_unblock, res_kill = -1;

  for(i = 0; i < TOTAL_PROCESSES; i++){
    res =  my_create_process_prio("endless_loop");
    if(res == -1){
      addText("Error creating process");
      substractLine();
      printWindow();
      return;
    }
    pids[i] = res;
    
  }
    

  bussy_wait_prio(WAIT);
  addText("CHANGING PRIORITIES...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        resNice = my_nice_prio(pids[i], 0); 
        if(resNice == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }//lowest priority 
        break;

      case 1:
        resNice2 = my_nice_prio(pids[i], 1); //medium priority
        if(resNice2 == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 2:
        resNice3 = my_nice_prio(pids[i], 2); //highest priority
        if(resNice3 == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
    }
  }

  bussy_wait_prio(WAIT);
  addText("BLOCKING...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
      res_block = my_block_prio(pids[i]);
       if(res_block == -1){
          addText("Error blocking process");
          substractLine();
          printWindow();
          return;
        }

  }
   

  addText("CHANGING PRIORITIES WHILE BLOCKED...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        resNice4 = my_nice_prio(pids[i], 1); //medium priority
        if(resNice4 == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 1:
        resNice5 = my_nice_prio(pids[i], 1); //medium priority
        if(resNice5 == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
      case 2:
        resNice6 = my_nice_prio(pids[i], 1); //medium priority
        if(resNice6 == -1){
          addText("Error updating priority");
          substractLine();
          printWindow();
          return;
        }
        break;
    }
  }

  addText("UNBLOCKING...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++){
    res_unblock = my_unblock_prio(pids[i]);
    if(res_unblock == -1){
          addText("Error unblocking process");
          substractLine();
          printWindow();
          return;
        }
  }
   

  bussy_wait_prio(WAIT);
  addText("KILLING...");
  substractLine();
  printWindow();

  for(i = 0; i < TOTAL_PROCESSES; i++) {
    res_kill = my_kill_prio(pids[i]);
    if (res_kill == -1) {
      addText("Error killing process");
      substractLine();
      printWindow();
      return;
    }
  }
}

