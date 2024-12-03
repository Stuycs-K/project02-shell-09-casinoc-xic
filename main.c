#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
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
  char buffer[200];
  char * ptr = buffer;
  fgets(ptr, 200, stdin);
  int semicolon = semicolon_counter(ptr);
  char *arr[semicolon];
  for(int i = 0; i<=semicolon; i++){
    arr[i] = strsep(&ptr, ";");
    printf("%s", arr[i]);
    pid_t command = fork();
    if(command < 0){
      perror("fork fail");
      exit(1);
    }
    else if(command == 0){ //child command

    }

    else{ //parent
      int *status;
      wait(status);
    }
  }
  return 0;
}
