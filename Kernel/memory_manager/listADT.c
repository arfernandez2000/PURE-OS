// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

