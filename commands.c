#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stddef.h>
#include<fcntl.h> //For X_ constants
#include<sys/wait.h>

#include<token.h>
#include<constants.h>

extern char *PATH_FILE;

char* check_cmd(char *cmd) {
    FILE* file_ptr = NULL;

    //Return COMMAND NOT FOUND
    if((file_ptr = fopen(PATH_FILE, "r")) == NULL)
        return "";

    char *buffer = NULL, *program_path = "";
    size_t line_length = 0, cmd_len;
    
    while((cmd_len = getline(&buffer, &line_length, file_ptr)) != EOF) {
        //Skip empty lines
        if(cmd_len == 1)
            continue;
        
        buffer[cmd_len - 1] = '\0';

        char *curr_path = buffer;
        strcat(curr_path, "/");
        strcat(curr_path, cmd);

        if(access(curr_path, X_OK) == 0) { 
            program_path = curr_path;
            break;
        }
    }

    fclose(file_ptr);

    return program_path;
}

void execute_command(char *program_path, TOKEN *token_list, size_t token_list_length) {        
    pid_t pid = fork();
    int status;

    if(pid < 0) {
        fprintf(stderr, error_msg);
        return;
    }
    //Child Process
    else if(pid == 0) {
        char **arguments = (char **)malloc(sizeof(char *)*(token_list_length + 1));
        
        for(int i = 0; i < token_list_length; i++)
            arguments[i] = strdup(token_list[i].val);
        
        arguments[token_list_length] = NULL;

        //TODO: Add error handling
        execv(program_path, arguments);

        free(arguments);
        exit(0);
    }
    //Parent Process
    else {
        //TODO: Robust handling of background processes
        if(token_list[0].is_background)
            printf("%d\n", pid);
        else
            waitpid(pid, &status, 0);

        //TODO: Add Error Handling
    }
    
    return;
}