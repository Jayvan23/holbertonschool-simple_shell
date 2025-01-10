#include "shell.h"

void execute_command(char *line)
{
    char *args[MAX_ARGS];
    int i = 0;
    pid_t pid;

    args[i] = strtok(line, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1)
    {
        i++;
        args[i] = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL)
        return;

    pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, environ) == -1)
        {
            perror("./hsh");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        wait(NULL);
    }
}
