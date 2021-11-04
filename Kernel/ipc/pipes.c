// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <pipes.h>

#define BUFF_SIZE 1024
#define PIPE_COUNT 12

#define IN_USE 1
#define EMPTY 0

typedef struct
{
    uint32_t id;
    char buffer[BUFF_SIZE];
    int readIndex;
    int writeIndex;
    long totalProcesses;
    int lockW;
    int lockR;
    int state;
} Pipe;

typedef struct
{
    Pipe pipes[PIPE_COUNT];
} PipeArray;

uint32_t semId = 200;
static PipeArray pipesArray;

static int putCharPipeByIdx(int pipeIndex, char c);
static int getPipeIdx(uint32_t pipeId);
static int getFreePipe();
static uint32_t newPipe(uint32_t pipeId);

uint32_t pOpen(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
    {
        pipeIndex = newPipe(pipeId);
        if (pipeIndex == -1)
            return -1;
    }

    pipesArray.pipes[pipeIndex].totalProcesses++;

    return pipeId;
}

int pClose(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    pipe->totalProcesses--;

    //Depleted pipe?
    if (pipe->totalProcesses > 0)
        return 1;

    if(sClose(pipe->lockR) == -1){
        return -1;
    };
     if(sClose(pipe->lockW) == -1){
         return -1;
     };
    pipe->state = EMPTY;

    return 1;
}

char* pRead(uint32_t pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return NULL;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    if(sWait(pipe->lockR) == -1){
        return NULL;
    };

    char c = pipe->buffer[pipe->readIndex];
    pipe->readIndex = (pipe->readIndex + 1) % BUFF_SIZE;

    if(sPost(pipe->lockW) == -1){
        return NULL;
    };

    return &c;
}

uint32_t pWrite(uint32_t pipeId, char *str)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    while (*str != 0)
        putCharPipeByIdx(pipeIndex, *str++);
    putCharPipeByIdx(pipeIndex, 0);
    return pipeId;
}

static int putCharPipeByIdx(int pipeIndex, char c)
{
    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    if(sWait(pipe->lockW) == -1){
        return -1;
    };

    pipe->buffer[pipe->writeIndex] = c;
    pipe->writeIndex = (pipe->writeIndex + 1) % BUFF_SIZE;

    if(sPost(pipe->lockR) == -1){
        return -1;
    };

    return 0;
}

uint32_t putCharPipe(uint32_t pipeId, char c)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    putCharPipeByIdx(pipeIndex, c);

    return pipeId;
}

static uint32_t newPipe(uint32_t pipeId)
{
    int newIdx = getFreePipe();

    if (newIdx == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[newIdx];

    pipe->id = pipeId;
    pipe->state = IN_USE;
    pipe->readIndex = pipe->writeIndex = pipe->totalProcesses = 0;

    if ((pipe->lockR = sOpen(semId++, 0)) == -1)
        return -1;

    if ((pipe->lockW = sOpen(semId++, BUFF_SIZE)) == -1)
        return -1;

    return pipeId;
}

static int getPipeIdx(uint32_t pipeId)
{
    for (int i = 0; i < PIPE_COUNT; i++)
        if (pipesArray.pipes[i].state == IN_USE && pipesArray.pipes[i].id == pipeId)
            return i;
    return -1;
}

static int getFreePipe()
{
    for (int i = 0; i < PIPE_COUNT; i++)
        if (pipesArray.pipes[i].state == EMPTY)
            return i;
    return -1;
}