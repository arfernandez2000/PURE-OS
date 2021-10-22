
#include "listADT.h"

void pushToList(ListADT *list, ListADT *elem) {
    ListADT *prev = ListADT->prev;
    elem->prev = prev;
    elem->next = elem;
    list->prev = elem;
}

void removeFromList(ListADT *elem) {
    List *prev = elem->prev;
    List *next = elem->next;
    prev->next =  next;
    next->prev = prev;
}

ListADT *popFromList(ListADT *list) {
    ListADT *before = list->prev;
    if (back == list) {
        return NULL;
    }
    removeFromList(back);
    return back;
}

int isEmpty(ListADT *list) {
    return list == list->prev;
}

