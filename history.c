#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Custom Headers
#include<history.h>
#include<constants.h>

const size_t HISTORY_BUFFER = 1000;

HISTORY_STORE* create_history_store(void) {
    HISTORY_STORE *history_store = (HISTORY_STORE *)malloc(sizeof(HISTORY_STORE));

    if(history_store == NULL) {
        fprintf(stderr, error_msg);
        exit(1);
    }

    history_store->store = (char **)malloc(sizeof(char *)*HISTORY_BUFFER);
    history_store->curr = 0;
    history_store->max = HISTORY_BUFFER;
    
    if(history_store->store == NULL) {
        fprintf(stderr, error_msg);
        exit(1);
    }

    return history_store; 
}

char* get_history(HISTORY_STORE *history_store, size_t n) {
    if(n < 0 || n > history_store->curr) {
        fprintf(stderr, error_msg);
        exit(1);
    }

    return history_store->store[n];
}

void add_history(HISTORY_STORE *history_store, char *cmd) {
    //Reallocate to a bigger size
    if(history_store->curr + 1 == history_store->max) {
        history_store->max += HISTORY_BUFFER;
        history_store->store = (char **)realloc(history_store->store, sizeof(char *)*(history_store->max));
    }

    (history_store->store)[(history_store->curr)++] = strdup(cmd);

    return;
}