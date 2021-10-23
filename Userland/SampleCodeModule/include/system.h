#ifndef SYSTEM_H
#define SYSTEM_H

void sys_switchContext();
void sys_loadProcess();
int sys_time(char);
void sys_write(int, char *, int);
char sys_read(int, char *, int);
void * sys_malloc(unsigned);
void sys_free(void *);
void sys_ps(void *);

#endif