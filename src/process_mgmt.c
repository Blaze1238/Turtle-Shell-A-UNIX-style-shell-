#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "../include/process_mgmt.h"

static int handle_redirection(char **args){
    int i = 0;
    while(args[i] != NULL){

        if(strcmp(args[i], ">") == 0){
            char* filename = args[i+1];
            if(filename == NULL){
                perror("No filename specified");
                return 1;
            }
            int fd = open(filename,O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if(fd < 0){
                perror("File failed to open");
                return -1;
            }

            if(dup2(fd,STDOUT_FILENO) < 0){
                perror("dup2 failed");
                close(fd);
                return -1;
            }
            close(fd);
            args[i] = NULL;
            break;
        }

        else if(strcmp(args[i], ">>") == 0){
            char* filename = args[i+1];
            if(filename == NULL){
                perror("No filename specified");
                return 1;
            }
            int fd = open(filename,O_WRONLY | O_APPEND | O_CREAT, 0644);
            if(fd < 0){
                perror("File failed to open");
                return -1;
            }

            if(dup2(fd,STDOUT_FILENO) < 0){
                perror("dup2 failed");
                close(fd);
                return -1;
            }
            close(fd);
            args[i] = NULL;
            break;
        }

        else if(strcmp(args[i], "<") == 0){
            char* filename = args[i+1];
            if(filename == NULL){
                perror("No filename specified");
                return 1;
            }
            int fd = open(filename,O_RDONLY, 0644);
            if(fd < 0){
                perror("File failed to open");
                return -1;
            }

            if(dup2(fd,STDIN_FILENO) < 0){
                perror("dup2 failed");
                close(fd);
                return -1;
            }
            close(fd);
            args[i] = NULL;
            break;
        }

        i++;
    }
    return 0;
}

int launch_process(char** args){
    pid_t pid = fork();

    if(pid < 0){
        perror("Failed to fork process");
        return -1; 
    }
    else if(pid == 0){
        //Child process 
        int handle_redir_ret = handle_redirection(args);
        if(handle_redir_ret < 0){
            exit(EXIT_FAILURE);
        }else if(handle_redir_ret > 0){
            return 0;
        }

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