#include "shell.h"

/**
 * _get_path - get variable PATH.
 * @env: environment local
 *
 * Return: value of PATH or NULL if not found.
 */
char *_get_path(char **env)
{
    size_t index = 0, var = 0, count = 0;
    char *path = NULL;

    for (index = 0; env[index] != NULL; index++)
    {
        if (_strncmp(env[index], "PATH=", 5) == 0)
            break;
    }

    if (env[index] == NULL)
        return (NULL);

    for (var = 5, count = 0; env[index][var]; var++, count++)
        ;

    path = malloc(sizeof(char) * (count + 1));

    if (path == NULL)
        return (NULL);

    for (var = 5, count = 0; env[index][var]; var++, count++)
        path[count] = env[index][var];

    path[count] = '\0';

    return (path);
}
