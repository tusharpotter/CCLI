#ifndef HISTORY_H
#define HISTORY_H

#include<stddef.h>

typedef struct HISTORY_STORE {
    char **store;
    size_t curr, max;
} HISTORY_STORE; 

HISTORY_STORE* create_history_store(void);

char* get_history(HISTORY_STORE *history_store, size_t n);

void add_history(HISTORY_STORE *history_store, char *cmd);

#endif