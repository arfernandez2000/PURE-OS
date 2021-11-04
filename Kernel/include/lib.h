#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

void swap(char *x, char *y);
char *reverse(char *buffer, int i, int j);
int abs(int value);
char *itoa(int value, char *buffer, int base, int length);
void strcpy(char *str1, char *str2);
int Stringlen(const char *s);
char *strcat(char *strg1, char *strg2);
int strcmp(const char *s1, const char *s2);

void acquire(int *mutex);
void release(int *mutex);
int _xchg(int *mutex, int value);
void _callTick();

#endif