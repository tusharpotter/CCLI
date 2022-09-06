#ifndef BUILT_IN_H
#define BUILT_IN_H

#include<stddef.h>

extern const size_t built_in;
extern const char* built_ins[];

int get_builtin(char *cmd);

void builtin_handler(int code, TOKEN *token_list, size_t token_list_length);

void help(TOKEN *token_list, size_t token_list_length);
void history_cmd(TOKEN *token_list, size_t token_list_length);
void exit_shell(TOKEN *token_list, size_t token_list_length);
void path(TOKEN *token_list, size_t token_list_length);
void change_dir(TOKEN *token_list, size_t token_list_length);
void pwd(TOKEN *token_list, size_t token_list_length);

#endif