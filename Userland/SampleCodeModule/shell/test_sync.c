// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include "system.h"
#include "shell.h"
#include "sem.h"
#include "test_sync.h"
#include "libc.h"
#include "processCommands.h" 

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 101
#define SEM_SHELL 102
#define NULL (void*) 0

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int argc, char *argv[]){
  uint64_t i;
  int sem = atoi(argv[0], 1);
  int64_t value = atoi(argv[1], Stringlen(argv[1]));
  int N = atoi(argv[2], Stringlen(argv[2]));

  if (sem && sOpen(SEM_ID, 1) == -1){
    addText("ERROR OPENING SEM\n");
    printWindow();
    return;
  }
  
  for (i = 0; i < N; i++){
    if (sem){
      if(sWait(SEM_ID) == -1){
        return;
      };
    } 
    slowInc(&global, value);
    if (sem) {
      if(sPost(SEM_ID) == -1){
        return;
      };
    }

  }

  if (sem) {
    if(sClose(SEM_ID) == -1){
      return;
    }
  } 
  
  addText("Final value: ");
  char* buff;
  addText(itoa(global, buff, 10));
  substractLine();
  printWindow();
  sPost(SEM_SHELL);
  exit();
}

void test_sync(){
  uint64_t i;

  global = 0;

  if(sOpen(SEM_SHELL, -TOTAL_PAIR_PROCESSES*2) == -1){
    return;
  }

  addText("CREATING PROCESSES...(WITH SEM)\n");
  printWindow();
  int error;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char **argv1 = sys_malloc(4*sizeof(char));
    argv1[0] = "inc";
    argv1[1] = "1";
    argv1[2] = "1";
    argv1[3] = "1000";
    error = sys_loadProcess(&inc, 4, argv1, 0, NULL);
    if (error == -1)
    {
        addText("Error al crear el proceso");
        printWindow();
        return ;
    }
   
    
    char **argv2 = sys_malloc(4*sizeof(char));
    argv2[0] = "inc";
    argv2[1] = "1";
    argv2[2] = "-1";
    argv2[3] = "1000";
    error = sys_loadProcess(&inc, 4, argv2, 0, NULL);
    if (error == -1)
    {
        addText("Error al crear el proceso");
        printWindow();
        return ;
    }
  }

  for (int i = 0; i < TOTAL_PAIR_PROCESSES*2; i++)
  {
    if(sWait(SEM_SHELL) == -1){
      return;
    };
  }

  if(sClose(SEM_SHELL) == -1){
    return;
  };
  
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  addText("CREATING PROCESSES...(WITHOUT SEM)\n");
  printWindow();
  int error;

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[4] = {"inc", "1", "1", "100"};
    error = sys_loadProcess(&inc, 4, argv1, 0, NULL);
    if(error == -1){
      addText("Error loading Process");
      printWindow();
      return;
    }
    char *argv2[4] = {"inc", "0", "-1", "100"};
    error = sys_loadProcess(&inc, 4, argv2, 0, NULL);
    if(error == -1){
      addText("Error loading Process");
      printWindow();
      return;
    }
  }
}

