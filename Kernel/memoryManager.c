
#include "video.h"

// //habria que hacer un nodo grande e ir fragmentando y de-fragmentando, 
// //ya que estamos ubicados en el kernel

// #include "video.h"
// #include "memorymanager.h"
// #include "defs.h"

// #define NALLOC 1024

// typedef long Align;

// union header {
//     struct {
//         union header * ptr; /* next block if on free list*/
//         unsigned size; /*size of this block */
//     } data;
//     Align x; /* force alignment of blocks*/
// };

// typedef union header Header;


// static Header base; /* empty list to get started*/
// static Header *freep = NULL; /*start of free list*/

// unsigned long totalUnits;

// void * mallocMM(unsigned nbytes){

//     if (nbytes == 0) 
//         return NULL;

//     Header *p, *prevp;
//     unsigned nunits;

//     nunits = (nbytes + sizeof(Header)-1)/sizeof(Header) + 1;

//     if((prevp = freep) == NULL){ /*no free list*/
//         base.s.ptr = freep = prevp = &base;
//         base.s.size = 0;
//     }

//     for(p= prevp->s.ptr;; prevp = p, p= p->s.ptr) {
//         if(p->s.size >= nunits){ /* big enough */
//             if(p->s.size == nunits) {
//                 prevp->s.ptr = p->s.ptr;
//             }
//             else{ /*allocate tail end*/
//                 p->s.size -= nunits;
//                 p += p->s.size;
//                 p->s.size = nunits;
//             }
//             freep = prevp;
//             return (void*) (p+1);
//         }
//         if(p == freep){ /* wrapped around free list */
//             new_line();
//             new_line();
//             return NULL; /* none left */
//         }
//     }


// }



// /* free: put block ap in free list */
// void freeMM(void *ap) {
//     Header *bp, *p;
    
//     bp = ( Header *) ap -1; /* point to block header */
//     for(p = freep; !(bp >  p && bp < p->data.ptr); p = p->data.ptr){
//         if(p >= p->data.ptr && (bp > p || bp < p->data.ptr))
//             break; /* freed block at start or end of arena*/
//     }

//     if( bp + bp->data.size == p->data.ptr){ /* join to upper nbr*/
//         bp->data.size += p->data.ptr->data.size;
//         bp->data.ptr = p->data.ptr->data.ptr;
//     }
//     else{
//         bp->data.ptr =  p->data.ptr;
//     } 
    
//     if( p + p->data.size == bp){ /* join to lower nbr*/
//         p->data.size += bp->data.size;
//         p->data.ptr = bp->data.ptr;
//     }
//     else{
//         p->data.ptr = bp;
//     }
    // freep = p;
// }










// #include "memorymanager.h"
// #include "video.h"

// char * ptr = 0x60000000;

// void memInit(void * startingPoint){

//     ptr = (char *) startingPoint;
// }

// void * mallocMM(unsigned bytes){
//     char * aux = ptr;
//     ptr += bytes;
//     return aux;
// }

// void freeMM(void *ap) {
//     ;
// }








// void initializeMemory(char *memBase, unsigned long memSize)
// {
//       // Initially its all a very large block
//       totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
//       freep = base = (Header *)memBase;
//       freep->data.size = totalUnits;
//       freep->data.ptr = freep;
// }





// void *mallocMM(unsigned nbytes)
// {
//       if (nbytes == 0)
//             return NULL;
            
//       unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

//       Header *currNode, *prevNode;
//       prevNode = freep;
        
//       for (currNode = prevNode->data.ptr;; prevNode = currNode, currNode = currNode->data.ptr)
//       {
          
//             if (currNode->data.size >= nunits) {      
//                   if (currNode->data.size == nunits) // Equal just use
//                         prevNode->data.ptr = currNode->data.ptr;
//                   else
//                   {
//                         currNode->data.size -= nunits;
//                         currNode += currNode->data.size;
//                         currNode->data.size = nunits;
//                   }
//                   freep = prevNode;
                  
//                   return (void *)(currNode + 1); //Return new memspace WITHOUT header
//             }  
//             if (currNode == freep)
//                   return NULL;
//       }
// }


// void freeMM(void *freeMem)
// {
//       if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0)
//             return;

//       Header *freeBlock, *currNode;
//       freeBlock = (Header *)freeMem - 1; //Add header to mem to free

//       if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
//             return;

//       char isExternal = 0;

//       for (currNode = freep; !(freeBlock > currNode && freeBlock < currNode->data.ptr); currNode = currNode->data.ptr)
//       {

//             if (freeBlock == currNode || freeBlock == currNode->data.ptr)
//                   return;

//             if (currNode >= currNode->data.ptr && (freeBlock > currNode || freeBlock < currNode->data.ptr))
//             {
//                   isExternal = 1;
//                   break;
//             }
//       }

//       if (!isExternal && (currNode + currNode->data.size > freeBlock || freeBlock + freeBlock->data.size > currNode->data.ptr)) //Absurd!!
//             return;

//       if (freeBlock + freeBlock->data.size == currNode->data.ptr) //Join right
//       {
//             freeBlock->data.size += currNode->data.ptr->data.size;
//             freeBlock->data.ptr = currNode->data.ptr->data.ptr;
//       }
//       else
//             freeBlock->data.ptr = currNode->data.ptr;

//       if (currNode + currNode->data.size == freeBlock) //Join left
//       {
//             currNode->data.size += freeBlock->data.size;
//             currNode->data.ptr = freeBlock->data.ptr;
//       }
//       else
//             currNode->data.ptr = freeBlock;

//       freep = currNode;
// }





#define NULL 0

typedef long Align;
typedef union header Header;

union header
{
      struct
      {
            union header *ptr;
            unsigned size;
      } data;
      Align x;
};

static Header *base;
static Header *startingNode = NULL;

unsigned long totalUnits;

void memInit(char *memBase, unsigned long memSize)
{
      // Initially its all a very large block
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      startingNode = base = (Header *)memBase;
      startingNode->data.size = totalUnits;
      startingNode->data.ptr = startingNode;
}

// Ref for malloc/free : The C Programming Language  - K&R
void *mallocMM(unsigned nbytes)
{
      if (nbytes == 0)
            return NULL;

      unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

      Header *currNode, *prevNode;
      prevNode = startingNode;

      for (currNode = prevNode->data.ptr;; prevNode = currNode, currNode = currNode->data.ptr)
      {
          
            if (currNode->data.size >= nunits)
            {
                  if (currNode->data.size == nunits) // Equal just use
                        prevNode->data.ptr = currNode->data.ptr;
                  else
                  {
                        currNode->data.size -= nunits;
                        currNode += currNode->data.size;
                        currNode->data.size = nunits;
                  }
                  startingNode = prevNode;
                  return (void *)(currNode + 1); //Return new memspace WITHOUT header
            }
            if (currNode == startingNode)
                  return NULL;
      }
}

void freeMM(void *freeMem)
{
      if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0)
            return;

      Header *freeBlock, *currNode;
      freeBlock = (Header *)freeMem - 1; //Add header to mem to free

      if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
            return;

      char isExternal = 0;

      for (currNode = startingNode; !(freeBlock > currNode && freeBlock < currNode->data.ptr); currNode = currNode->data.ptr)
      {

            if (freeBlock == currNode || freeBlock == currNode->data.ptr)
                  return;

            if (currNode >= currNode->data.ptr && (freeBlock > currNode || freeBlock < currNode->data.ptr))
            {
                  isExternal = 1;
                  break;
            }
      }

      if (!isExternal && (currNode + currNode->data.size > freeBlock || freeBlock + freeBlock->data.size > currNode->data.ptr)) //Absurd!!
            return;

      if (freeBlock + freeBlock->data.size == currNode->data.ptr) //Join right
      {
            freeBlock->data.size += currNode->data.ptr->data.size;
            freeBlock->data.ptr = currNode->data.ptr->data.ptr;
      }
      else
            freeBlock->data.ptr = currNode->data.ptr;

      if (currNode + currNode->data.size == freeBlock) //Join left
      {
            currNode->data.size += freeBlock->data.size;
            currNode->data.ptr = freeBlock->data.ptr;
      }
      else
            currNode->data.ptr = freeBlock;

      startingNode = currNode;
}
