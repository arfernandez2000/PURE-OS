#include "phylo.h"

#define INITIAL_PHYLOS 4 /* number of philosophers */
#define MAX_PHYLOS 9

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define GONE 3

typedef int semaphore; /* semaphores are a special kind of int */
int state[MAX_PHYLOS]; /* array to keep track of everyone’s state */
semaphore mutex;   /* mutual exclusion for critical regions */
semaphore mutexTable;
semaphore s[MAX_PHYLOS]; /* one semaphore per philosopher */
int phylosPid[MAX_PHYLOS];

int phylosCount;

#define RIGHT(i) ((i) + 1) % (phylosCount)              /* number of i’s right neighbor */
#define LEFT(i) ((i) + phylosCount - 1) % (phylosCount) /* number of i’s left neighbor */

int tablePrintID = -1;

void sleep(unsigned int seconds)
{
    unsigned int limitTime = seconds + ticksElapsed();
    while (ticksElapsed() < limitTime)
        ;
}
void think()
{
    sleep(10);
    return;
}

void eat()
{
    sleep(10);
    return;
}
static void up(int *sem)
{
    (*sem)++;
}
static void down(int *sem)
{
    (*sem)--;
}
void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING)
    {
        state[i] = EATING;
        up(&s[i]);
    }
}

void take_forks(int i)
{

    down(&mutex);      /* enter critical region */
    state[i] = HUNGRY; /* record fact that philosopher i is hungry */
    test(i);           /* try to acquire 2 for ks */
    up(&mutex);        /* exit critical region */
    down(&s[i]);       /* block if for ks were not acquired */
}

void put_forks(int i)
{
    down(&mutex);        /* enter critical region */
    state[i] = THINKING; /* philosopher has finished eating */
    test(LEFT(i));       /* see if left neighbor can now eat */
    test(RIGHT(i));      /* see if right neighbor can now eat */
    up(&mutex);          /* exit critical region */
}

void philosopher(int argc, char **argv)
{
    int i = atoi(argv[2], 1); // esto cambiarlo. HAY QUE RECIBIR POR PARAMETRO EL IDX DEL phylosopher
    char buffer[10];
    while (1)
    { 
        /* repeat forever */
        //think(); /* philosopher is thinking */
        take_forks(i); /* acquire two for ks or block */
        //eat(); /* yum-yum, spaghetti */
        put_forks(i); /* put both for ks back on table */
    }
}
int removePhylo()
{
    down(&mutex);
    if (phylosCount == INITIAL_PHYLOS)
    {
        return -1;
    }
  
    phylosCount--;
    kill(phylosPid[phylosCount]);
    state[phylosCount] = GONE;
    up(&mutex);
    return 1;
}
int addPhylo(int fg)
{
    if (phylosCount == MAX_PHYLOS)
        return -1;
    down(&mutex);
    char buffer[10];
    char buffer2[10];
    char buffer3[10];
    char *argv[] = {"phylo", itoa(fg, buffer, 10), itoa(tablePrintID, buffer2, 10), itoa(phylosCount, buffer3, 10)};
    int pid = sys_loadProcess(&philosopher, 4, argv, fg, 0);

    if (pid == -1)
    {
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

void tablePrint(int argc, char** argv) {
    while (1) {
        sleep(10);
        down(&mutexTable);
        for (int i = 0; i < phylosCount; i++)
        {
            if (state[i] == EATING)
            {
                addText(" E ");
            }
            else
            {
                addText(" . ");
            }
        }
        substractLine();
        printWindow();
        up(&mutexTable);
    }
}

void table(int argc, char **argv)
{
    if (atoi(argv[0], 1) == 1)
    {
        block(0);
    }
    if(atoi(argv[0], 1) != 1) {
        while(1);
    }
    int run = 1;
    while (run)
    {
        char key = getChar();
        
        switch (key)
        {
        case 'a':
            if (addPhylo(atoi(argv[0], 1)) == -1)
            {
                addText("Can\'t add another philosopher. Maximum 9 philosophers.");
                substractLine();
                printWindow();
            }
            else
            {
                addText("A new philosopher joined!");
                substractLine();
                printWindow();
            }
            break;
        case 'd':
            if (removePhylo() == -1)
            {
                addText("Can\'t remove another philosopher. Minimum 4 philosophers.");
                substractLine();
                printWindow();
            }
            else
            {
                addText("One philosopher has left!");
                substractLine();
                printWindow();
            }
            break;
        case '\t':
            run = 0;
            break;
        default:
            break;
        }
    }
    addText("Finished");
    substractLine();
    printWindow();
    unblock(0);
    kill(tablePrintID);
    killAllPhylos();
    addText("$> ");
    printWindow();
    exit();
}
void killAllPhylos()
{
    down(&mutex);
    for (int i = 0; i < phylosCount; i++){
         kill(phylosPid[i]);
         phylosPid[i] = 0;
         s[i]= 0;
         state[i] = GONE;
    }
    phylosCount = 0;
    up(&mutex);
}
void initialize(){
    mutex =1;
    mutexTable = 1;
    phylosCount = 0;
}

//---------------------------------------------------

void phylo(int fg)
{
    int run = 1;
    char buffer[10];
    
    initialize();
    char *argv[] = {"table", itoa(fg, buffer, 10)};
    int tableID = sys_loadProcess(&table, 2, argv, fg, 0);
    char *argv2[] = {"tableP", itoa(fg, buffer,10)};
    tablePrintID = sys_loadProcess(&tablePrint, 2, argv2, fg, 0);
    if (tableID == -1)
    {
        addText("Error Loading Table");
        substractLine();
        printWindow();
        return;
    }
    block(tableID);
    block(tablePrintID);
   
    for (int i = 0; i < INITIAL_PHYLOS; i++)
    {
        addPhylo(fg);
    }
    sleep(10);
    unblock(tableID);
    unblock(tablePrintID);
}
