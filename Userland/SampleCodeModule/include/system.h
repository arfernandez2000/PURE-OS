#ifndef SYSTEM_H
#define SYSTEM_H

void sys_switchContext();
void sys_loadProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int fd[2], char* name);
int sys_time(char);
void sys_write(int, char *, int);
char sys_read(int, char *, int);
void * sys_malloc(unsigned);
void sys_free(void *);
void sys_ps(void *);

#endif