#ifndef SHELL_H
#define SHELL_H
void pipe(char *cmd1, char *cmd2);
int parse_args(char * line, char ** arg_ary);
char ** parse_command(char * stripped_command);
#endif