#include "phylo.h"

#define INITIAL_PHYLOS 5 /* number of philosophers */
#define MAX_PHYLOS 62


#define THINKING 0 
#define HUNGRY 1  
#define EATING 2 


typedef int semaphore; /* semaphores are a special kind of int */
int state[MAX_PHYLOS]; /* array to keep track of everyone’s state */
semaphore mutex = 1; /* mutual exclusion for critical regions */
semaphore s[MAX_PHYLOS]; /* one semaphore per philosopher */
int phylosPid[MAX_PHYLOS];

int phylosCount = 0;

#define RIGHT(i) ((i) + 1) % (phylosCount)                         /* number of i’s right neighbor */
#define LEFT(i) ((i) + phylosCount - 1) % (phylosCount) /* number of i’s left neighbor */

void think(){
    return;
}

void eat(){
    return;
}
static void up(int * sem){
    (*sem)++;
}
static void down(int * sem){
    (*sem)--;
}
void test(int i){
    if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING) {
        state[i] = EATING;
        up(&s[i]);
    }
}

void take_forks(int i) 
{

    down(&mutex); /* enter critical region */
    state[i] = HUNGRY; /* record fact that philosopher i is hungry */
    test(i); /* try to acquire 2 for ks */
    up(&mutex); /* exit critical region */
    down(&s[i]); /* block if for ks were not acquired */
}

void put_forks(int i) 
{
    down(&mutex); /* enter critical region */
    state[i] = THINKING; /* philosopher has finished eating */
    test(LEFT(i)); /* see if left neighbor can now eat */
    test(RIGHT(i)); /* see if right neighbor can now eat */
    up(&mutex); /* exit critical region */
}

void philosopher(int argc, char ** argv)
{
    int i = atoi(argv[1],10);
    while (1) { /* repeat forever */
        //think(); /* philosopher is thinking */
        take_forks(i); /* acquire two for ks or block */
        //eat(); /* yum-yum, spaghetti */
        put_forks(i); /* put both for ks back on table */
    }
}
int removePhylo(){
    down(&mutex);
    if(phylosCount == INITIAL_PHYLOS){
        return -1;
    }
    phylosCount--;
    //kill(phylosPid[phylosCount]);
    kill(phylosCount + 1); 
    up(&mutex);
    return 1;

}
int addPhylo(int fg){
    if(phylosCount == MAX_PHYLOS)
        return -1;
    down(&mutex);
    char buffer[10];
    char *argv[] = { "phylosopher", itoa(fg, buffer,10), itoa(phylosCount,buffer,10)};
    int pid = sys_loadProcess(&philosopher,2, argv, fg, 0); //tenemos que devolver el pid
    
    if(pid == -1){
        addText("Error Loading Phylosopher");
        substractLine();
        printWindow();
        return -1;
    }
    state[phylosCount] = THINKING;
    s[phylosCount] = 1; 
    phylosPid[phylosCount++] = pid;
    up(&mutex);
    return 1;
}



void table(int argc, char** argv){
    if(atoi(argv[0],10) == 1){
        block(0);
    }
    int run = 1;
    while(run){
        for(int i=0; i< phylosCount; i++){
            if(state[i] == EATING){
                addText(" E ");
            }
            else{
                addText(" . ");
            }
        }
        substractLine();
        printWindow();

    }
    unblock(0);
    exit();
}


//---------------------------------------------------
void phylo(int fg){
    int run = 1;
    char buffer[10];
    char * argv[] = {"Phylosophers table", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&table,2,argv,0,0);
    if(error == -1){
        addText("Error Loading Table");
        substractLine();
        printWindow();
        return;
    }
    block(1);
    for(int i=0; i< INITIAL_PHYLOS; i++){
       addPhylo(fg);
   }
   unblock(2);
   while(run){
       char key = getChar();
        switch (key)
        {
        case 'a':
            if (addPhylo(fg) == -1){
                addText("Can\'t add another philosopher. Maximum 8 philosophers.");
                substractLine();
                printWindow();
            }else{
                addText("A new philosopher joined!");
                substractLine();
                printWindow();
            }
            break;  
        case 'd':
            if (removePhylo() == -1){
                addText("Can\'t remove another philosopher. Minimum 4 philosophers.");
                substractLine();
                printWindow();
            }
            else{
                addText("One philosopher has left!");
                substractLine();
                printWindow();
            }
            break;
        case 'q':
            run = 0;
            break;
        default:
            break;
        }



   }
   addText("Finished.");
   substractLine();
   printWindow();
}
