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

      // Create backup for stdout and stdin.
      int stdout = 1;
      int backup_stdout = dup(stdout);
      int stdin = 0;
      int backup_stdin = dup(stdin);

      // Check if there is a pipe.
      int is_piped = 0;
      if(strstr(command, "|") != NULL){
        is_piped = 1;
      }
      
      for(int i = 0; i < is_piped + 1; i++){

        // Redirection is there is a pipe.
        if(is_piped){
          char * command_ptr = command;
          char * sub_command;
          if(i == 0){
            sub_command = strsep(&command_ptr, "|");
            sub_command[strlen(sub_command) - 1] = '\0';
            printf("piped0\n");
            int fd3 = open("pipe_temp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
            dup2(fd3, stdout);
          }
          if(i == 1){   
            strsep(&command_ptr, " ");
            sub_command = command_ptr;
            printf("piped1\n");
            int fd4 = open("pipe_temp", O_RDONLY, 0);
            dup2(fd4, stdin);
          }
        }
 
        // Redirect output to file with appending.
        printf("piped2\n");
        char * stripped_command = command;
        if(strstr(command, ">>") != NULL){
          stripped_command = strsep(&command, ">");
          stripped_command[strlen(stripped_command)-1] = '\0';
          strsep(&command, " ");
          char * stripped_filename = command;
          int fd1 = open(stripped_filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(fd1, stdout);
        }

        // Redirect output to file with truncating. 
        else if(strstr(sub_command, ">") != NULL){
          printf("piped3\n");
          stripped_command = strsep(&sub_command, ">");
          stripped_command[strlen(stripped_command)-1] = '\0';
          strsep(&sub_command, " ");
          char * stripped_filename = sub_command;
          int fd1 = open(stripped_filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(fd1, stdout);
        }

        // Redirect input from file. 
        else if(strstr(sub_command, "<") != NULL){
          printf("piped4\n");
          stripped_command = strsep(&sub_command, "<");
          stripped_command[strlen(stripped_command)-1] = '\0';
          strsep(&sub_command, " ");
          char * stripped_filename = sub_command;
          int fd1 = open(stripped_filename, O_RDONLY, 0);
          dup2(fd1, stdin);
        }
    
        // Parse the command.
        printf("piped5\n");
        char ** parsed_command;
        parsed_command = calloc(200, 1);
        int args_num = parse_args(stripped_command, parsed_command);

        // Handle exiting.
        if(strcmp(parsed_command[0],"exit") == 0){
          exit(0);
        }

        // Switch directories.
        printf("piped6\n");
        if(strcmp(parsed_command[0], "cd") == 0){ //cd
          printf("cd called\n");
          chdir(parsed_command[1]);
        }

        // Run the command with a fork.
        printf("piped7\n");
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
        }
        free(parsed_command);
        printf("piped8\n");
        dup2(backup_stdout, stdout);
        dup2(backup_stdin, stdin);
      }
      //remove("pipe_temp");
    }
  }
  return 0;
}
