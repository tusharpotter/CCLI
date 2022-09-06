#ifndef COMMANDS_H
#define COMMANDS_H

#include<token.h>
#include<stddef.h>

char* check_cmd(char *cmd);

void execute_command(char *program_path, TOKEN *token_list, size_t token_list_length);

#endif