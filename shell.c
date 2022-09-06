#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/wait.h>

//Custom Headers
#include<constants.h>
#include<token.h>
#include<history.h>
#include<built-in.h>
#include<commands.h>

HISTORY_STORE *history_store;

void handler(TOKEN *token_list, size_t token_list_length);
void init_path_file(void);

char *PATH_FILE = "./PATH";

int main(int argc, char* argv[]) {
    if(argc > 2) {
        //Print an error
        fprintf(stderr, "More number of arguments than required\n");
        exit(1);
    }

    //Get the absolute pathname of the file PATH
    init_path_file();

    bool is_interactive_mode = (argc == 1);

    FILE* file_ptr = stdin;
    if(!is_interactive_mode) {
        char *batch_file = strdup(argv[1]);

        if((file_ptr = fopen(batch_file, "r")) == NULL) {
            fprintf(stderr, error_msg);
            exit(1);
        }
    }

    history_store = create_history_store(); 

    char *buffer = NULL;
    TOKEN *token_list = NULL;
    size_t token_list_length, buffer_size, cmd_len;
    token_list_length = buffer_size = 0;
    bool is_valid_cmd;

    while(true) {
        if(is_interactive_mode)
            printf("csh> ");

        if((cmd_len = getline(&buffer, &buffer_size, file_ptr)) == EOF)
            break;

        //Skip empty lines
        if(cmd_len == 1)
            continue;

        buffer[cmd_len - 1] = '\0';

        is_valid_cmd = tokenize(strdup(buffer), &token_list, &token_list_length);
       
        if(is_valid_cmd && token_list_length)
            handler(token_list, token_list_length);

        add_history(history_store, buffer);

        free(token_list);
    }

    //Close the file
    if(!is_interactive_mode)
        fclose(file_ptr);

    free(buffer);
    free(history_store);

    return 0;
}

void handler(TOKEN *token_list, size_t token_list_length) {
    for(size_t pos = 0, cmd_len; pos < token_list_length; pos += cmd_len) {
        char *curr_cmd = token_list[pos].val;
        cmd_len = token_list[pos].length;
        TOKEN* curr_cmd_start = token_list + pos;

        int built_in_code = get_builtin(curr_cmd);

        if(built_in_code == -1) {
            char *program_path = check_cmd(curr_cmd);

            if(strlen(program_path) == 0) {
                printf("Command '%s' not found\n", curr_cmd);
                return;
            }

            execute_command(program_path, curr_cmd_start, cmd_len);
        }
        else
            builtin_handler(built_in_code, curr_cmd_start, cmd_len);        
    }

    return;
}

//Determine the canonical path name i.e. absolute pathname
//of the file path
void init_path_file(void) {
    char *file = strdup(PATH_FILE);

    file = realpath(file, NULL);
    if(file == NULL)
        fprintf(stderr, "The shell will not work beyond the current directory and non-bultin commands will not work\n");
    else
        PATH_FILE = file;

    return;
}