#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char *environ;  /* To access environment variables */

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *args[100];
    pid_t pid;
    int i, j;
    char cwd[1024];
    int status = 0;  /* Track the exit status of the last command */

    while (1) {
        i = 0;  /* Declare variables at the top */
        printf("simple_shell ");  /* Executable statement follows declarations */

        nread = getline(&line, &len, stdin);
        if (nread == -1) {  /* End of input (Ctrl+D) */
            printf("\n");
            break;
        }

        line[nread - 1] = '\0';  /* Remove newline character */
        args[i] = strtok(line, " ");
        while (args[i] != NULL && i < 99) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (args[0] == NULL)
            continue;  /* Ignore empty input */

        /* Built-in: exit */
        if (strcmp(args[0], "exit") == 0) {
            if (args[1] != NULL) {  /* If there are extra arguments after exit */
                fprintf(stderr, "exit: too many arguments\n");
                continue;  /* Print error and continue with the prompt */
            }
            free(line);
            exit(status);  /* Exit the shell with the current status */
        }

        /* Built-in: cd */
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: expected argument\n");
            } else if (chdir(args[1]) != 0) {
                perror("cd");
            }
            continue; /* Avoid further processing */
        }

        /* Built-in: pwd */
        if (strcmp(args[0], "pwd") == 0) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("pwd");
            }
            continue; /* Avoid further processing */
        }

        /* Built-in: echo */
        if (strcmp(args[0], "echo") == 0) {
            j = 1;
            for (; args[j] != NULL; j++) {
                printf("%s ", args[j]);
            }
            printf("\n");
            continue; /* Avoid further processing */
        }

        /* Built-in: env */
        if (strcmp(args[0], "env") == 0) {
            /* Directly use environ to access the environment variables */
            for (char **env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
            }
            continue; /* Avoid further processing */
        }

        /* Built-in: export */
        if (strcmp(args[0], "export") == 0) {
            if (args[1] == NULL || strchr(args[1], '=') == NULL) {
                fprintf(stderr, "export: usage: export VAR=value\n");
            } else {
                char *var = strtok(args[1], "=");
                char *value = strtok(NULL, "=");
                if (setenv(var, value, 1) != 0) {
                    perror("export");
                }
            }
            continue; /* Avoid further processing */
        }

        /* Execute external commands */
        pid = fork();
        if (pid == 0) { /* Child process */
            /* Handle commands by checking if the path is absolute */
            if (args[0][0] != '/') {
                // Command isn't an absolute path, try to find it in $PATH
                char *path = getenv("PATH");
                char *cmd_path = malloc(strlen(path) + strlen(args[0]) + 2);
                char *tok = strtok(path, ":");
                while (tok) {
                    sprintf(cmd_path, "%s/%s", tok, args[0]);
                    if (access(cmd_path, X_OK) == 0) {
                        execve(cmd_path, args, environ);
                    }
                    tok = strtok(NULL, ":");
                }
                free(cmd_path);
            } else {
                /* Command is an absolute path, just execute */
                if (execve(args[0], args, environ) == -1) {
                    perror("./hsh");
                    exit(EXIT_FAILURE);  /* Exit with failure status if execve fails */
                }
            }
        } else if (pid < 0) {
            perror("fork");
        } else { /* Parent process */
            wait(&status);  /* Wait for child process to finish */
            if (WIFEXITED(status)) {
                status = WEXITSTATUS(status);  /* Capture exit status of the child process */
            } else if (WIFSIGNALED(status)) {
                status = 128 + WTERMSIG(status);  /* If child process was terminated by a signal */
            }
        }
    }

    free(line);  /* Free memory after loop ends */
    return 0;
}

