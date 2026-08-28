#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/pipeline_parse.h"

char*** parse_pipe(char **args){
    int i = 0;

    if(strcmp(args[0],"|") == 0){
        perror("Inalid pipe");
        return NULL;
    }


    char*** pipe_args = (char***)malloc(sizeof(char**) * PIPE_COMM_CAP);
    
    if(pipe_args == NULL){
        perror("Cant initialize Pipe Arguments");
        return NULL;
    }
    
    int pipe_args_num = 0;
    int pipe_args_cap = PIPE_COMM_CAP;

    pipe_args[pipe_args_num++] = &args[0];

    while(args[i] != NULL){
        if(strcmp(args[i],"|") == 0){
            if(args[i+1] == NULL || (strcmp(args[i+1],"|") == 0)){
                perror("Invalid Pipe");
                free(pipe_args);
                return NULL;
            }

            pipe_args[pipe_args_num++] = &args[i+1];
            args[i] = NULL;
            if(pipe_args_num >= pipe_args_cap - 1){
                pipe_args_cap += PIPE_COMM_CAP; 
                char*** temp = (char***)realloc(pipe_args, sizeof(char**) * pipe_args_cap);
                if(temp == NULL){
                    perror("Pipe Arguments realloc failed");
                    free(pipe_args);
                    return NULL;
                }
                pipe_args = temp;
            }
        }
        i++;
    }
    pipe_args[pipe_args_num] = NULL;
    return pipe_args;
}