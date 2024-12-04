#ifndef MAIN_H
#define MAIN_H
void parse_args(char * line, char ** arg_ary);
static void sighandler(int signo);
int semicolon_counter(char * str);
#endif