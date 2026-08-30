#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../include/reader.h"
#include "../include/parser.h"
#include "../include/process_mgmt.h"
#include "../include/builtins.h"
#include "../include/pipeline_parse.h"

int main(){
    int running = 1;
    int input_buf_pos = 0;
    int is_built_in = -1;
    int single_ret_code = -1;
    int pipe_ret_code = -1;

    // Setting SIGINT on shell to be ignored (Ctrl + c deosnt terminate the shell )
    signal(SIGINT,SIG_IGN);
    signal(SIGTTOU,SIG_IGN); //This diables sigttou 

    while(running){
        
        printf("$turtle : ");
        fflush(stdout);

        char* input_buf = read_command(&input_buf_pos);

        if(input_buf == NULL){
            printf("\n Shell terminated! ");
            return 0;
        }
        else{

            //Manual Tokenizer
            char** command_args = tokenize_command(input_buf, input_buf_pos);

            if(command_args != NULL && command_args[0] != NULL && command_args[0][0] != '\0'){
                is_built_in = cae_builtin(command_args); 

                if(is_built_in == -1){
                    free(command_args);
                    free(input_buf);
                    return 0;
                }
                
                //forking and execution of child process
                char*** pipe_args;
                
                if(!is_built_in){
                    //Pipe commands executed
                    pipe_args = parse_pipe(command_args);

                    if(pipe_args[0] != NULL)
                        single_ret_code = launch_process(command_args);
                    else
                        pipe_ret_code = launch_pipeline(pipe_args);
                }
            }

            //Freeing and reset
            if (command_args != NULL) {
                free(command_args);
            }
            if (input_buf != NULL) {
                free(input_buf);
            }
            input_buf_pos = 0;
        }
    }
    return 0;
}

