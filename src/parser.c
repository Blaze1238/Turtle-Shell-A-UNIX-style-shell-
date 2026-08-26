#include <stdio.h>
#include <stdlib.h>

#include "../include/parser.h"

int is_whitespace(char c){
    if(c == ' ' || c == '\t' || c == '\n' || c =='\r') return 1;
    return 0;
}

char** tokenize_command(char* input_buf, int buf_length){
    char** command_args = (char**) malloc(sizeof(char*) * COMMAND_ARGS_BUFF);
    if(command_args == NULL) exit(EXIT_FAILURE);

    int args_cap = COMMAND_ARGS_BUFF;
    int args_pos = 0;
    int in_word = 0;

    for(int i = 0; i <= buf_length; i++){
        if(is_whitespace(input_buf[i])){
            input_buf[i] = '\0';
            in_word = 0;
        }

        else if(in_word == 0){
            in_word = 1; 
            command_args[args_pos++] = &input_buf[i];
    
            if(args_pos >= args_cap - 1){
                args_cap += COMMAND_ARGS_BUFF;
                char** temp = (char**) realloc(command_args, args_cap * sizeof(char*));
                if(temp == NULL){
                    free(command_args);
                    exit(EXIT_FAILURE);
                }
                else{
                    command_args = temp;
                }
            }
        }

    }
    command_args[args_pos] = NULL;

    return command_args;
}