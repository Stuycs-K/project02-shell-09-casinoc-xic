#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "main.h"

int parse_args(char * line, char ** arg_ary){
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
  return i;
}

int main(int argc, char *argv[]){
  while(1){

    // Get user input.
    char cwd[512];
    strcpy(cwd, getcwd(cwd, 200));
    printf("%s/$ ", cwd);
    fflush(stdout);
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

      // Create backup for stdout and command.
      int stdout = 1;
      int backup_stdout = dup(stdout);
      char command_copy[100];
      strcpy(command_copy, command);

      // Redirect output to file with appending.
      char * stripped_command = strsep(&command_copy, ">>");
      char * filename = strsep(&command_copy, ">>");
      char output_redirected = 0;
      if (filename != NULL){
        output_redirected = 1;
        int fd1 = open(++filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
        dup2(fd1, stdout);
      }

      // Redirect output to file with truncating.
      if(!output_redirected){
        char * stripped_command = strsep(&command, ">");
        char * filename = strsep(&command, ">");
        if (filename != NULL){
          int fd1 = open(++filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(fd1, stdout);
        }
      }
      
      // Parse the command.
      char ** parsed_command;
      parsed_command = calloc(200, 1);
      int args_num = parse_args(stripped_command, parsed_command);

      // Handle exiting.
      if(strcmp(parsed_command[0],"exit") == 0){
        exit(0);
      }

      // Switch directories.
      if(strcmp(parsed_command[0], "cd") == 0){ //cd
        printf("cd called\n");
        chdir(parsed_command[1]);
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
        int status;
        wait(&status);
        dup2(backup_stdout, stdout);
      }
      free(parsed_command);
    }
  }
  return 0;
}
