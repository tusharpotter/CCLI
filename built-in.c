#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>
#include<unistd.h>
#include<ctype.h>
#include<errno.h>

#include<token.h>
#include<built-in.h>
#include<history.h>
#include<constants.h>

//TODO: Improve error handling

enum BUILT_IN { CD, HELP, EXIT, PWD, HISTORY, PATH };

extern HISTORY_STORE *history_store;

const size_t built_in = 6;

extern char *PATH_FILE;

const char* built_ins[] = {
    "cd",
    "help",
    "exit",
    "pwd",
    "history",
    "path"
};

int get_builtin(char *cmd) {    
    int code = -1;

    for(size_t i = 0; i < built_in; i++) {        
        if(strcmp(cmd, built_ins[i]) == 0) {
            code = i;
            break;
        }
    }

    return code;
}

void builtin_handler(int code, TOKEN *token_list, size_t token_list_length) {
    switch(code) {
        case HELP:
            help(token_list, token_list_length);
            break;

        case EXIT:
            exit_shell(token_list, token_list_length);
            break;

        case HISTORY:
            history_cmd(token_list, token_list_length);
            break;

        case PATH:
            path(token_list, token_list_length);
            break;

        case CD:
            change_dir(token_list, token_list_length);
            break;

        case PWD:
            pwd(token_list, token_list_length);
            break;
    }
    
    return;
}

void help(TOKEN *token_list, size_t token_list_length) {
    //TODO: Print help text

    return;
}

void history_cmd(TOKEN *token_list, size_t token_list_length) {
    size_t start = 0;

    if(token_list_length == 2) {
        start = atoi(token_list[1].val);

        if(start == 0 && (strlen(token_list[0].val) == 0 || !isdigit(token_list[0].val[0]))) {
            fprintf(stdout, "A numeric argument greater than 0 is required\n");
            return;
        }

        start = ((long long int)history_store->curr - (long long int)start >= 0) ? history_store->curr - start : 0;
    }

    for(size_t i = start; i < history_store->curr; i++)
        fprintf(stdout, "%lu %s\n", i + 1, (history_store->store)[i]);

    return;
}

void exit_shell(TOKEN *token_list, size_t token_list_length) {
    if(token_list_length > 1)
        fprintf(stderr, "No arguments are allowed for exit command\n");
    else
        exit(0);

    return;
}

void path(TOKEN *token_list, size_t token_list_length) {
    FILE* file_ptr = NULL;

    if((file_ptr = fopen(PATH_FILE, "w")) == NULL) {
        fprintf(stderr, error_msg);
        return;
    }

    for(int i = 1; i < token_list_length; i++)
        fprintf(file_ptr, "%s\n", token_list[i].val);

    fclose(file_ptr);

    return;
}

void change_dir(TOKEN *token_list, size_t token_list_length) {
    if(token_list_length == 1) 
        fprintf(stderr, "Please specify a path\n");
    else if(token_list_length > 2)
        fprintf(stderr, "Too many arguments\n");
    else if(chdir(token_list[1].val) == -1)
        fprintf(stderr, "%s\n", strerror(errno));

    return;
}

void pwd(TOKEN *token_list, size_t token_list_length) {
    if(token_list_length > 1 || token_list[0].is_background) {
        fprintf(stderr, "No arguments are allowed for exit command\n");
        return;
    }

    //Determine the maximum PATH SIZE possible using
    //pathconf()
    size_t PATH_MAX = pathconf(".", _PC_PATH_MAX);
    if(PATH_MAX == -1) {
        fprintf(stderr, error_msg);
        return;
    }

    char *buffer = (char *)malloc(sizeof(char)*PATH_MAX);
    if(buffer == NULL) {
        fprintf(stderr, error_msg);
        return;
    }

    buffer = getcwd(buffer, PATH_MAX);
    if(buffer == NULL) {
        fprintf(stderr, error_msg);
        return;
    }
    else
        fprintf(stdout, "%s\n", buffer);

    free(buffer);
    
    return;
}