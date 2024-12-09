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
#include "shell.h"

void pipe(char *cmd1, char *cmd2) {
    int temp_fd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (temp_fd == -1) {
        printf("%s\n", strerror(error));
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First command: Redirect stdout to temp file
        dup2(temp_fd, STDOUT_FILENO);
        close(temp_fd);
        execvp(cmd1, cmd1);
        exit(0); // Execute cmd1
    }

    int status1;
    waitpid(pid1, &status1, 0);

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second command: Redirect stdin from temp file
        int input_fd = open("temp.txt", O_RDONLY, 0777);
        if (input_fd == -1) {
            printf("%s\n", strerror(error));
            return;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
        close(temp_fd);
        execvp(cmd2, cmd2); // Execute cmd2
    }

    int status2;
    waitpid(pid2, &status2, 0);

    close(temp_fd);
    unlink(temp_file); // Remove temp file
}

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

      int stdin = 0;
      // Redirect output to file with appending.
      
      char * stripped_command = command;
      
      if(strstr(command, ">>") != NULL){
        stripped_command = strsep(&command, ">");
        stripped_command[strlen(stripped_command)-1] = '\0';
        printf("%s\n", stripped_command);
        strsep(&command, " "); //goes past operators
        char * stripped_filename = command;
        int fd1 = open(stripped_filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
        dup2(fd1, stdout);
      }

      // Redirect output to file with truncating. 
      else if(strstr(command, ">") != NULL){
        stripped_command = strsep(&command, ">");
        stripped_command[strlen(stripped_command)-1] = '\0';
        printf("%s\n", stripped_command);
        strsep(&command, " ");
        char * stripped_filename = command;
        printf("%s\n", stripped_filename);
        if (stripped_filename != NULL){
          int fd1 = open(stripped_filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(fd1, stdout);
        }
      } // return stripped_command
      //pipe redirection
      else if(strstr(command, "|") != NULL){
        stripped_command = strsep(&command, "|");
        stripped_command[strlen(stripped_command)-1] = '\0';
        printf("%s\n", stripped_command);
        strsep(&command, " "); 
        char * stripped_filename = command; //right side command
        pipe(stripped_command, stripped_filename);
      }
      
      
      // Parse the command.
      char ** parsed_command;
      parsed_command = parse_command(stripped_command);

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
