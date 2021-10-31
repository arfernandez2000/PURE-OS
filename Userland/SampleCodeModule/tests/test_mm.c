#include "test_mm.h"
#include "system.h"
#include <string.h>

#include "shell.h"
#include "libc.h"


#define MAX_BLOCKS 1
#define MAX_MEMORY 1024 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

// void test_mm(){
//   mm_rq mm_rqs[MAX_BLOCKS];
//   uint8_t rq;
//   uint32_t total;
//    int i = 0;

//   while (i < 1){
//     i++;
//     rq = 0;
//     total = 0;

//     // Request as many blocks as we can
//     while(rq < MAX_BLOCKS && total < MAX_MEMORY){
     
//       mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
//       mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size); // TODO: Port this call as required

//       //TODO: check if NULL
      
      
//       if (mm_rqs[rq].address == NULL)
//       {
//         printString("No memory!\n");
        
//       }
//       total += mm_rqs[rq].size;
//       rq++;
//     }

//     // Set
//     uint32_t i;
//     for (i = 0; i < rq; i++)
//       if (mm_rqs[i].address != NULL)
//         memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

//     // Check
//     for (i = 0; i < rq; i++)
//       if (mm_rqs[i].address != NULL)
//         if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
//           printString("ERROR!\n"); // TODO: Port this call as required

//     // Free
//     for (i = 0; i < rq; i++)
//       if (mm_rqs[i].address != NULL)
//         sys_free(mm_rqs[i].address);  // TODO: Port this call as required

//   } 
  
// }
void test_mm(){\
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  // int contador = 50;
  int contador = 100000;

  // addText("Testing...",0,10);
  // printWindow(0);

  //No se por que pero el printString mueve todo a la derecha
  // printString("Testing...");

  while (contador){
    rq = 0;
    total = 0;
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size); // TODO: Port this call as required
//TODO: check if NULL
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printString("Error!");


    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        sys_free(mm_rqs[i].address);  // TODO: Port this call as required
    contador--;
  } 
  addText("Memory manager test OK");
  substractLine();
  printWindow();
}
