#include <stdio.h>
#include <stdlib.h>

#include "../include/reader.h"
#include "../include/parser.h"
#include "../include/process_mgmt.h"
#include "../include/builtins.h"

int main(){
    int running = 1;
    int input_buf_pos = 0;

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

            int is_built_in = -1;
            if(command_args != NULL && command_args[0] != NULL){
                is_built_in = cae_builtin(command_args); 

                //forking and execution of child process
                int return_code = -1;
                if(!is_built_in)
                    return_code = launch_process(command_args);
            
            }

            //Freeing and reset
            input_buf_pos = 0;
            free(command_args);
            free(input_buf);

            if(is_built_in == -1) return 0;
        }
    }
    return 0;
}

