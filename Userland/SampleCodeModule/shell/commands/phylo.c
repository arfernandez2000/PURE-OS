#include "phylo.h"


#define INITIAL_PHYLOS 4 /* number of philosophers */
#define MAX_PHYLOS 9

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define GONE 3

#define SEM_SHELL 105
#define SEM_MUTEX 106
#define SEM_PHYLO 107

int state[MAX_PHYLOS]; /* array to keep track of everyone’s state */
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

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING)
    {
        state[i] = EATING;
        sPost(SEM_PHYLO + i);
    }
}

void take_forks(int i)
{

    sWait(SEM_MUTEX);      /* enter critical region */
    state[i] = HUNGRY; /* record fact that philosopher i is hungry */
    test(i);           /* try to acquire 2 for ks */
    sPost(SEM_MUTEX);        /* exit critical region */
    sWait(SEM_PHYLO + i);       /* block if for ks were not acquired */
}

void put_forks(int i)
{
    sWait(SEM_MUTEX);        /* enter critical region */
    state[i] = THINKING; /* philosopher has finished eating */
    test(LEFT(i));       /* see if left neighbor can now eat */
    test(RIGHT(i));      /* see if right neighbor can now eat */
    sPost(SEM_MUTEX);          /* exit critical region */
}

void philosopher(int argc, char **argv)
{
    int i = atoi(argv[2], 1); 
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
    sWait(SEM_MUTEX);
    if (phylosCount == INITIAL_PHYLOS)
    {
        return -1;
    }
  
    phylosCount--;
    kill(phylosPid[phylosCount]);
    state[phylosCount] = GONE;
    sPost(SEM_MUTEX);
    sClose(SEM_PHYLO + phylosCount);
    return 1;
}
int addPhylo(int fg)
{
    if (phylosCount == MAX_PHYLOS)
        return -1;
  
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
    sWait(SEM_MUTEX);
    state[phylosCount] = THINKING;
    if(!sOpen(SEM_PHYLO + phylosCount, 1))
        return -1;
        
    phylosPid[phylosCount++] = pid;
    sPost(SEM_MUTEX);
    return 1;
}

void killAllPhylos()
{
    
    for (int i = 0; i < phylosCount; i++){
        kill(phylosPid[i]);
        // phylosPid[i] = 0;
        state[i] = GONE;
        sClose(SEM_PHYLO + i);
    }
}

void tablePrint(int argc, char** argv) {
    while (1) {
        sleep(10);
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
    }
}

void table(int argc, char **argv)
{
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
            sPost(SEM_MUTEX);
            break;
        default:
            break;
        }
    }
    addText("Finished");
    substractLine();
    printWindow();
    kill(tablePrintID);
    killAllPhylos();
    sClose(SEM_MUTEX);
    sPost(SEM_SHELL);
    sClose(SEM_SHELL);
    exit();
}

void initialize(){
    if(!sOpen(SEM_MUTEX, 1))
        return;
    sWait(SEM_MUTEX);    
    phylosCount = 0;
    sPost(SEM_MUTEX);
}

//---------------------------------------------------

void phylo(int fg)
{
    int run = 1;
    char buffer[10];
    if(fg){
        if(!sOpen(SEM_SHELL, -1))
            return;
    }
    
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
    if(fg)
        sWait(SEM_SHELL);
}
