#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - simple shell main loop
 *
 * Return: 0 on success, or the exit status code.
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *command;

    while (1)
    {
        printf("$ ");

        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            free(line);
            exit(0);
        }

        line[nread - 1] = '\0';

        command = strtok(line, " ");

        if (command != NULL && strcmp(command, "exit") == 0)
        {
            free(line);
            exit(0);
        }

    }

    free(line);
    return (0);
}
