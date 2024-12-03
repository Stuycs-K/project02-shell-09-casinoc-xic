#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "main.h"

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
  char ** commands;
  printf("buffer: %s semicolon: %d\n", ptr, test);
  return 0;
}