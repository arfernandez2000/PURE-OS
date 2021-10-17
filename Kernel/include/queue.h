#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct Queue *QueueADT;

QueueADT createQueue();
int enqueueProcess(QueueADT queue, PCB process);
void freeQueue(QueueADT queue);
PCB* dequeueProcess(QueueADT queue);
int isEmpty(QueueADT queue);





#endif