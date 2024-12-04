#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "main.h"

void parse_args(char * line, char ** arg_ary){
  char * token;
  char * curr = line;
  token = strsep(&curr, " ");
  arg_ary[0] = token;
  int i = 1;
  while(i < 99) {
    token = strsep(&curr, " ");
    if(token == NULL){
      break;
    }
    arg_ary[i] = token;
    i++;
  }
}

int semicolon_counter(char * str){
  int num = 0;
  for(int i = 0; i < strlen(str); i++){
    if(str[i] == ';'){
      num++;
    }
  }
  return num;
}

int main(int argc, char *argv[]){
  while(1){

    // Get user input.
    printf(">>>");
    char input_buffer[200];
    char * input = input_buffer;
    if(fgets(input, 200, stdin) == NULL){
      printf("\n");
      exit(0);
    }
    input[strlen(input)-1] = '\0';
    char * command;

    // Loop through commands and break if command is NULL.
    while(1){

      // Get the next command.
      command = strsep(&input, ";");
      if(command == NULL){
        break;
      }
      // Break if command is the last command.

      // Parse the command.
      char ** parsed_command;
      parsed_command = calloc(200, 1);
      parse_args(command, parsed_command);

      // Handle exiting.
      if(strcmp(parsed_command[0],"exit") == 0){
        exit(0);
      }
      
      // Run the command with a fork.
      pid_t child = fork();
      if(child < 0){
        perror("fork fail");
        exit(1);
      }
      else if(child == 0){ //child command
        execvp(parsed_command[0], parsed_command);
        exit(0);
      }

      else{ //parent
        int *status;
        wait(status);
      }
      free(parsed_command);
    }
  }
  return 0;
}
