#include "../include/builtins.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int cae_builtin(char** args){
    if(strcmp(args[0], "exit") == 0){
        return -1;
    }
    else if(strcmp(args[0], "cd") == 0){
        char* sec_arg = args[1];

        if(sec_arg == NULL) sec_arg = getenv("HOME");
        
        if(sec_arg != NULL && chdir(sec_arg) != 0)
            perror("Cd failed!");

        return 1;
    }
    return 0;
}