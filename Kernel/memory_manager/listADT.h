#include <stdlib.h>
#include <stdint.h>

typedef struct ListADT {
    struct ListADT *prev;
    struct ListADT *next;
    int free;
    int level;
} ListADT;

void pushToList(ListADT *list, ListADT *elem);

void removeFromList(ListADT *elem);

ListADT *popFromList(ListADT *list);

int isEmpty(ListADT *list);
