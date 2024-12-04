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
  if(signo == SIGQUIT){
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
    printf(">>>");
    char buffer[200];
    char * ptr = buffer;
    fgets(ptr, 200, stdin);
    char *initial[20];
    parse_args(ptr, initial);
    if(strcmp(initial[0],"exit") == 0){
      exit(0);
    }
    signal(SIGQUIT, sighandler);
    int semicolon = semicolon_counter(ptr) + 1;
    char *arr[semicolon];
    ptr[strlen(ptr)-1] = '\0';
    for(int i = 0; i<1; i++){
        arr[i] = strsep(&ptr, ";");
        //printf("%lu\n", strlen(arr[i]));
        //printf("%s\n", arr[i]);
        pid_t command = fork();
        if(command < 0){
          perror("fork fail");
          exit(1);
        }
        else if(command == 0){ //child command
          char *arg_ary[20];
          parse_args(arr[i], arg_ary);
          //printf("Command: %s", arg_ary[0]);
          //printf("%s\n", arg_ary[1]);
          execvp(arg_ary[0], arg_ary);
        }

        else{ //parent
          int *status;
          wait(status);
        }
    }
  }
}
       
  return 0;
}
