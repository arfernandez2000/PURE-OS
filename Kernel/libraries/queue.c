#include "queue.h"
#include "defs.h"


typedef struct {
    PCB* process;
    struct Node* next;
}Node;

typedef struct {
    struct Node* head;
    struct Node* tail;
    unsigned int size;
}Queue;


static void freeQueueRec(Node *first);

void createQueue(){
    QueueADT queue;

    if ((queue = malloc(sizeof(Queue))) == NULL) {
        return NULL;
    }
    return queue;
}

int enqueueProcess(QueueADT queue, PCB* process){
    
    Node* newNode;
    if((newNode= malloc(sizeof(Node))) == NULL){
        return -1;
    }
    newNode->process = process;
    newNode->next = NULL;

    if(isEmpty(queue)){
            queue->head = newNode;
            queue->tail = newNode;
    }
    if (queue->tail != NULL) {
        queue->tail->next = newNode;
    }
    
    queue->size++;
    return 1;
}
void freeQueue(QueueADT queue){
    if(queue == NULL){
        return;
    }
    freeQueueRec(queue->head);
    free(queue);
}
void freeQueueRec(Node *first) {
    if (first == NULL) {
        return;
    }
    freeQueueRec(first->next);
    freeNode(first);
}

void freeProcess(Node *process)
{
      for (int i = 0; i < process->pcb.argc; i++)
            free(process->pcb.argv[i]);
      free(process->pcb.argv);
      free((void *)((char *)process->pcb.rbp - STACK_SIZE + 1));
      free((void *)process);
}

PCB* dequeueProcess(QueueADT queue){
    if(isEmpty(queue)){
        return NULL;
    }
    Node *aux = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    return aux->process;
}


int isEmpty(QueueADT queue){
    return queue->size == 0;
}