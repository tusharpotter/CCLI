#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<stddef.h>
#include<stdbool.h>

typedef struct TOKEN {
    char *val;
    bool is_option, is_background, is_command;
    int length;
} TOKEN;

TOKEN* new_token(void);

bool tokenize(char *str, TOKEN **token_list, size_t *token_list_length);

#endif