#include <stdint.h>
#include <stdio.h>
#include "test_sync.h"

// uint64_t my_create_process_sync(char * name){
//   return 0;
// }

// uint64_t my_sem_open_sync(char *sem_id, uint64_t initialValue){
//   return 0;
// }

// uint64_t my_sem_wait_sync(char *sem_id){
//   return 0;
// }

// uint64_t my_sem_post_sync(char *sem_id){
//   return 0;
// }

// uint64_t my_sem_close_sync(char *sem_id){
//   return 0;
// }

// #define TOTAL_PAIR_PROCESSES 2
// #define SEM_ID "sem"

// int64_t global;  //shared memory

// void slowInc_sync(int64_t *p, int64_t inc){
//   int64_t aux = *p;
//   aux += inc;
//   yield();
//   *p = aux;
// }

// void inc_sync(uint64_t sem, int64_t value, uint64_t N){
//   uint64_t i;

//   if (sem && !my_sem_open(SEM_ID, 1)){
//     addText("ERROR OPENING SEM\n");
//     return;
//   }
  
//   for (i = 0; i < N; i++){
//     if (sem) my_sem_wait(SEM_ID);
//     slowInc_sync(&global, value);
//     if (sem) my_sem_post(SEM_ID);
//   }

//   if (sem) my_sem_close(SEM_ID);
  
//   addText("Final value: %d\n", global);
// }

void test_sync(){
  // uint64_t i;

  // global = 0;

  // addText("CREATING PROCESSES...(WITH SEM)\n");

  // for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
  //   my_create_process_sync("inc", 1, 1, 1000000);
  //   my_create_process_sync("inc", 1, -1, 1000000);
  // }
}

// void test_no_sync(){
//   uint64_t i;

//   global = 0;

//   addText("CREATING PROCESSES...(WITHOUT SEM)\n");

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     my_create_process_sync("inc", 0, 1, 1000000);
//     my_create_process_sync("inc", 0, -1, 1000000);
//   }
// }
