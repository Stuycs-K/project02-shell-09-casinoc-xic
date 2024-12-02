#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int char_counter(char * str){
  for(int i = 0; i < strlen(str); )
}

int main(int argc, char *argv[]){
  char buffer[200];
  char * ptr = buffer;
  fgets(ptr, 200, stdin);
  printf("buffer: %s\n", ptr);
  return 0;
}