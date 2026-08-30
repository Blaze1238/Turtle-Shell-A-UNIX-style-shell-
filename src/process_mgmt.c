#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "../include/process_mgmt.h"
#include "../include/pipeline_parse.h"

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

        if(execvp(args[0],args) < 0){
            perror("Exec failed!");
            exit(EXIT_FAILURE); 
        } //This statement returns to this scope only if exec fails 

        exit(EXIT_FAILURE);
    }
    else{
        int status; 
        if(waitpid(pid,&status,0) < 0){
            perror("Wait failed!");
            return -1;
        }
        return 0;
    }
}

int launch_pipeline(char*** pipe_args){
    int i = 0;
    int pno = 0;
    int prev_pipe_read; 
    int curr_pipe[2];

    while(pipe_args[i] != NULL){
        pno++;
        i++;
    }

    pid_t p[pno];
    pno = 0;
    i = 0;

    while(pipe_args[i] != NULL){

        if(pipe_args[i+1] != NULL)
            if(pipe(curr_pipe) < 0){
                perror("pipe creation error");
                return 1;
            }

        p[pno] = fork();

        if(p[pno] < 0 ){
            perror("Child process creation error");
            return -1;
        }
        else if(p[pno] == 0){
            if(i > 0)
                if(dup2(prev_pipe_read, STDIN_FILENO) < 0){
                    perror("Falied to get prev pipe read");
                    exit(EXIT_FAILURE);
                }
            if(pipe_args[i+1] != NULL)
                if(dup2(curr_pipe[1], STDOUT_FILENO) < 0){
                    perror("Falied to create write pipe end");
                    exit(EXIT_FAILURE);
                }
            
            if(pipe_args[i+1] != NULL){
                close(curr_pipe[0]);
                close(curr_pipe[1]);
            }
            if(i>0)
                close(prev_pipe_read);

            if(execvp(pipe_args[i][0],pipe_args[i]) < 0){
                perror("Falied to create write pipe end");
                exit(EXIT_FAILURE);
            }
        }
        else if(p[pno] > 0){
            if(i>0) close(prev_pipe_read);
            if(pipe_args[i+1] != NULL){
                close(curr_pipe[1]);
                prev_pipe_read = curr_pipe[0];
            }
        }
        pno++;
        i++;
    }

    for(int j=0; j<pno; j++){
        int status; 
        if(waitpid(p[j],&status,0) < 0){
            perror("Wait multi-pipe error");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}