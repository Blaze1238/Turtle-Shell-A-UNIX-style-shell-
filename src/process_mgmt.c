#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "../include/process_mgmt.h"

int launch_process(char** args){
    pid_t pid = fork();

    if(pid < 0){
        perror("Failed to fork process");
        return -1; 
    }
    else if(pid == 0){
        //Child process 
        if(execvp(args[0],args) == -1){
            perror("Exec failed!");
            exit(EXIT_FAILURE); 
        } //This statement returns to this scope only if exec fails 

        exit(EXIT_FAILURE);
    }
    else{
        int status; 
        if(waitpid(pid,&status,0) == -1){
            perror("Wait failed!");
            return -1;
        }
        return 0;
    }
}