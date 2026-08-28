#include<stdio.h>
#include<stdlib.h>
#include "../include/reader.h"

char* read_command(int *input_buf_pos){
    int ch;
    char* input_buf = (char*)malloc(INPUT_BUFF_MIN); //buffer of 128 characters
    int input_buffer_cap = INPUT_BUFF_MIN;

    if(input_buf == NULL){
        perror("Input buff not initialized");
        exit(EXIT_FAILURE);
    }
    do{
        if(*input_buf_pos >=  input_buffer_cap - 1){
            char* temp = input_buf;
            input_buffer_cap += INPUT_BUFF_MIN;
            temp = (char*)realloc(temp, input_buffer_cap);
            if(temp == NULL){
                free(input_buf);
                perror("Couldnt reallocate input buff");
                exit(EXIT_FAILURE);
            }
            else input_buf = temp;
        }

        ch = getchar();

        if(ch == '\n')break;
        else if(ch == EOF){
            if(*input_buf_pos == 0){
                free(input_buf);
                return NULL;
            }
            else break;
        }

        input_buf[(*input_buf_pos)++] = (char)ch;
    }while(1);
    input_buf[*input_buf_pos] = '\0';
    return input_buf;
}

