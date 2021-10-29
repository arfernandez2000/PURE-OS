#ifndef SHELL
#define SHELL

#include "system.h"

void prueba();
void shell(int, char**);
void printWindow();
void addText(char * buffer);
void incorrect_comm(char * buffer);
void incorrect_arg(char * command);
void clearWindow();
void substractLine();
char * getWindow();
int getOffset();
void setOffset(int of);

#endif