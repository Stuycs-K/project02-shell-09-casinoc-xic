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

static void sighandler(int signo){
  printf("SIGHANDLER");
  if(signo == SIGQUIT){
    printf("SIGHANDLER_IF");
    exit(0);
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
    input = malloc(200);
    fgets(input, 200, stdin);
    input[strlen(input)-1] = '\0';

    // Parse the input.
    char *parsed_input[20];
    parse_args(input, parsed_input);

    // Handle exiting.
    if(strcmp(parsed_input[0],"exit") == 0){
      exit(0);
    }
    signal(SIGQUIT, sighandler);

  
    //int semicolon = semicolon_counter(input) + 1;
    //char *arr[semicolon];
    for(int i = 0; i<1; i++){
        //arr[i] = strsep(&input, ";");
        //printf("%lu\n", strlen(arr[i]));
        //printf("%s\n", arr[i]);
        pid_t command = fork();
        if(command < 0){
          perror("fork fail");
          exit(1);
        }
        else if(command == 0){ //child command
          //char *arg_ary[20];
          //parse_args(arr[i], arg_ary);
          //printf("Command: %s", arg_ary[0]);
          //printf("%s\n", arg_ary[1]);
          execvp(parsed_input[0], parsed_input);
          free(input);
          exit(0);
        }

        else{ //parent
          int *status;
          wait(status);
        }
    }
  }       
  return 0;
}
