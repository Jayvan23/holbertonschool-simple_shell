#include "shell.h"

int handle_builtins(char **argv)
{
    if (_strcmp(argv[0], "cd") == 0)
    {
        if (argv[1] == NULL)
            fprintf(stderr, "cd: expected argument\n");
        else if (chdir(argv[1]) != 0)
            perror("cd");
        return 0;
    }
    else if (_strcmp(argv[0], "pwd") == 0)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 0;
    }
    else if (_strcmp(argv[0], "echo") == 0)
    {
        int i = 1;
        for (; argv[i] != NULL; i++)
        {
            printf("%s", argv[i]);
            if (argv[i + 1] != NULL)
                printf(" ");
        }
        printf("\n");
        return 0;
    }
    else if (_strcmp(argv[0], "env") == 0)
    {   int i = 0;
        for (; environ[i] != NULL; i++)
            printf("%s\n", environ[i]);
        return 0;
    }
    else if (_strcmp(argv[0], "export") == 0)
    {
        if (argv[1] != NULL)
        {
            putenv(argv[1]);
        }
        else
        {
            fprintf(stderr, "export: expected argument\n");
        }
        return 0;
    }
    return -1;
}
