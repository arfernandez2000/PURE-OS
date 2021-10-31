#include "../include/shell.h"
#include "test_processes.h"

//TO BE INCLUDED
void endless_loop_proc(){
  while(1);
}

uint32_t my_create_process_proc(char * name){
  char * argv[] = {name};
  return sys_loadProcess(&endless_loop_proc, 1, argv,0,0);
  
}

uint32_t my_kill_proc(uint32_t pid){
  return kill(pid);
  
}

uint32_t my_block_proc(uint32_t pid){
  return block(pid);
}

uint32_t my_unblock_proc(uint32_t pid){
  return unblock(pid);

}

#define MAX_PROCESSES 51//Should be around 80% of the the processes handled by the kernel  | 80% of 64 is aprox 51

enum State { RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  int error;

  while (1){

    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      error = my_create_process_proc("endless_loop");
      if(error != -1 ){
        p_rqs[rq].pid = rq;
         p_rqs[rq].state = RUNNING;
        alive++;
      }
      else{
        addText("Error creating process");
        substractLine();
        printWindow();            
        return;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (my_kill_proc(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                addText("Error killing process");  
                substractLine();
                printWindow();         // TODO: Port this as required
                return;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(my_block_proc(p_rqs[rq].pid) == -1){          
                addText("Error blocking process");
                substractLine();
                printWindow();          
                return;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(my_unblock_proc(p_rqs[rq].pid) == -1){            
            addText("Error unblocking process");  
            substractLine();
            printWindow();          
            return;
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
  addText("ProcessTest OK");
  substractLine();
  printWindow();
}

