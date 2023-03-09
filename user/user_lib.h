/*
 * header file to be used by applications.
 */

// # added @ lab2_challenge2
typedef unsigned long long uint64;

int printu(const char *s, ...);
int exit(int code);
void *better_malloc(int n);
void better_free(void *va);

// # added @ lab2_challenge2
uint64 naive_sbrk(uint64 n);