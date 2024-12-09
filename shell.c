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

void handle_pipe(char *cmd1, char *cmd2) {
    char ** command1;
    command1 = parse_command(cmd1);

    char ** command2;
    command2 = parse_command(cmd2);

    int temp_fd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (temp_fd == -1) {
        printf("%s\n", strerror(perror));
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First command: Redirect stdout to temp file
        dup2(temp_fd, STDOUT_FILENO);
        close(temp_fd);
        execvp(command1[0], command1);
        exit(0); // Execute cmd1
    }

    int status1;
    waitpid(pid1, &status1, 0);

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second command: Redirect stdin from temp file
        int input_fd = open("temp.txt", O_RDONLY, 0);
        if (input_fd == -1) {
            printf("%s\n", strerror(perror));
            return;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
        close(temp_fd);
        execvp(command2[0], command2);
    }

    int status2;
    waitpid(pid2, &status2, 0);

    close(temp_fd);
    remove("temp.txt");
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

char ** parse_command(char * stripped_command){
     char ** parsed_command;
     parsed_command = calloc(200, 1);
     int args_num = parse_args(stripped_command, parsed_command);
     return parsed_command;
}