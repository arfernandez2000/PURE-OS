#include "phylo.h"

#define INITIAL_PHYLOS 5 /* number of philosophers */
#define MAX_PHYLOS 62
#define LEFT (i+N−1)%N /* number of i’s left neighbor */ //TODO hacer esto dinamicamente
#define RIGHT (i+1)%N /* number of i’s right neighbor */ //TODO hacer esto dinamicamente

#define THINKING 0 
#define HUNGRY 1  
#define EATING 2 


typedef int semaphore; /* semaphores are a special kind of int */
int state[MAX_PHYLOS]; /* array to keep track of everyone’s state */
semaphore mutex = 1; /* mutual exclusion for critical regions */
semaphore s[MAX_PHYLOS]; /* one semaphore per philosopher */

int phylosCount = INITIAL_PHYLOS;

static void think(){
  return;
}
static void eat(){
    return;
}

void philosopher(int i) /* i: philosopher number, from 0 to N−1 */
{
    while (1) { /* repeat forever */
        think(); /* philosopher is thinking */
        //take_forks(i); /* acquire two for ks or block */
        eat(); /* yum-yum, spaghetti */
        //put_forks(i); /* put both for ks back on table */
    }
}
void addInitialPhylos(int fg){
    char buffer[10];
    char *argv[] = { "phylosopher", itoa(fg,buffer,10)};
    for(int i=0; i< INITIAL_PHYLOS; i++){
        int error = sys_loadProcess(&philosopher,2,argv,fg,0);
        if(error == -1){
        addText("Error Loading Phylosopher");
        substractLine();
        printWindow();
        return;
        }
    }
}

void table(int argc, char** argv){
    addInitialPhylos(atoi(argv[0],2));
    while(1){
        
    }

}


// void take_forks(int i) /* i: philosopher number, from 0 to N−1 */
// {

//     down(&mutex); /* enter critical region */
//     state[i] = HUNGRY; /* record fact that philosopher i is hungry */
//     test(i); /* tr y to acquire 2 for ks */
//     up(&mutex); /* exit critical region */
//     down(&s[i]); /* block if for ks were not acquired */
// }

// void put_forks(i) /* i: philosopher number, from 0 to N−1 */
// {
//     down(&mutex); /* enter critical region */
//     state[i] = THINKING; /* philosopher has finished eating */
//     test(LEFT); /* see if left neighbor can now eat */
//     test(RIGHT); /* see if right neighbor can now eat */
//     up(&mutex); /* exit critical region */
// }
// void test(i) /* i: philosopher number, from 0 to N−1 */
// {
//     if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
//         state[i] = EATING;
//         up(&s[i]);
//     }
// }

void phylo(int fg){
    //proceso para la mesa
    //proceso para cada uno de los filosofos iniciales
    //que cada filosofo imprima su estado?
    //ver en fg y en bg
    char buffer[10];
    char * argv[] = {"Phylosophers table", itoa(fg, buffer,10)};
    int error = sys_loadProcess(&table,2,argv,0,0);
    if(error == -1){
        addText("Error Loading Table");
        substractLine();
        printWindow();
        return;
    }



}
