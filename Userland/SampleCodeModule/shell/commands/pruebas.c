#include "system.h"
#include "files.h"

#define NAME 1
#define NULL ((void *)0)
void proceso1Proc(int argc, char ** argv){

    int* ptr = (int*) shmOpen(NAME);
    if (ptr == NULL) {
        addText("Error opening shared Memory\n");
        printWindow();
        return;
    }
    while (1) {
        waitCycles(50);
        ptr[0] += 1;
    }
}


void proceso1(int fg){
    char * argv[] =  {"Proceso 1", fg};
    int fd[2];
    fd[0] = -1;
    fd[1] = -1;
    sys_loadProcess(&proceso1Proc, 2, argv, fg, fd);


}
void proceso2Proc(int argc, char** argv){
   int * ptr = (int*) shmOpen(NAME);
   if(ptr ==  NULL){
       addText("Error opening shared Memory \n");
       printWindow();
       return ;
   }
   char buffer[10];
   addText(itoa(*ptr,buffer,10));
   substractLine();
   printWindow();
   exit();
}

void proceso2(int fg){
    char * argv[] =  {"Proceso 2", fg};
    int fd[2];
    fd[0] = -1;
    fd[1] = -1;
    sys_loadProcess(&proceso2Proc, 2, argv, fg, fd);
}