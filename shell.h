#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stddeath>

#define MAX_ARGS 100

extern char **environ;

void execute_command(char *line);
int handle_builtins(char **argv);
int _strcmp(const char *s1, const char *s2);

#endif
