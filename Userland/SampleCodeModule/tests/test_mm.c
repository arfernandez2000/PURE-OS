#include "test_mm.h"
#include "system.h"
#include <string.h>

#include "shell.h"
#include "libc.h"


#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 //Should be around 80% of memory managed by the MM
#define LOOPS 100000

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  int contador = LOOPS;

   addText("Testing...");
   substractLine();
   printWindow();



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

    if(LOOPS >= 100000 && contador == LOOPS/2 ){
      addText("Yeah, it takes a while ;)");
      substractLine();
      printWindow();
    }
  } 
  addText("Memory manager test OK");
  substractLine();
  printWindow();
}
