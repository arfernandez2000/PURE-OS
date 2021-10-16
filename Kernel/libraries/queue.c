#include "queue.h"


typedef struct {
    PCB process;
    struct Node* next;
}Node;

typedef struct {
    struct Node* head;
    struct Node* tail;
    unsigned int size;
}Queue;