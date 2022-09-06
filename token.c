#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<token.h>
#include<constants.h>

TOKEN* new_token(void) {
    TOKEN *tok = (TOKEN *)malloc(sizeof(TOKEN));

    if(tok != NULL) {
        tok->is_background = false;
        tok->is_option = false;
        tok->is_command = false;
        tok->val = "";
        tok->length = 0;
    }

    return tok;
}

//Tokenize the string
bool tokenize(char *str, TOKEN **token_list, size_t* token_list_length) {
    int n = strlen(str);

    if(str == NULL)
        return false;

    //Count the number of tokens
    *token_list_length = 1;
    for(int i = 0; i < n; i++) {
        if(str[i] == ' ')
            (*token_list_length)++;
    }

    *token_list = (TOKEN *)malloc(sizeof(TOKEN)*(*token_list_length));
    if(token_list == NULL) {
        fprintf(stderr, error_msg);
        exit(1);
    }
    
    char *token = NULL;

    (*token_list_length) = 0;
    size_t last_cmd = 0, token_len;
    bool is_sep = true;

    while((token = strsep(&str, " \n")) != NULL) {
        token_len = strlen(token);

        if(token_len == 0)
            continue;
        
        TOKEN *curr = NULL;

        if(strcmp(token, "&") == 0) {
            if((*token_list)[last_cmd].is_background) {
                fprintf(stderr, "Invalid token \"&\" detected\n");
                
                if(last_cmd != 0) 
                    *token_list_length = last_cmd;

                return false;
            }
            else
                (*token_list)[last_cmd].is_background = true;
        }
        else if(strcmp(token, "&&") == 0)
            is_sep = true;
        else {
            curr = new_token();

            if(token[0] == '-' || !is_sep) {
                curr->is_option = true;
                (*token_list)[last_cmd].length++;
            }
            else {
                curr->is_command = true;
                curr->length = 1;

                last_cmd = *token_list_length;
            }

            curr->val = token;
            
            (*token_list)[(*token_list_length)++] = *curr;

            is_sep = false;
        }
    }
    
    return true;
}