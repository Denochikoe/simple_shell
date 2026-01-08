#include "shell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

/* ===================== */
/* Check if string contains '/' */
/* ===================== */
int check_for_dir(const char *s)
{
    if (!s)
        return 0;

    while (*s)
    {
        if (*s == '/')
            return 1;
        s++;
    }
    return 0;
}

/* ===================== */
/* Find PATH in environment */
/* ===================== */
char *find_path(char **env)
{
    if (!env)
        return NULL;

    for (int i = 0; env[i]; i++)
        if (_strcmpr_n(env[i], "PATH=", 5) == 0)
            return env[i] + 5;

    return NULL;
}

/* ===================== */
/* Execute command directly (fork + execve) */
/* ===================== */
static void execute_direct(const char *cmd, vars_t *vars)
{
    pid_t pid = fork();

    if (pid == 0) /* child */
    {
        execve(cmd, vars->av, vars->env);
        perror(cmd);
        exit(127);
    }
    else if (pid > 0) /* parent */
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            vars->status = WEXITSTATUS(status);
        else
            vars->status = 127;
    }
    else /* fork failed */
    {
        perror("fork failed");
        vars->status = 127;
    }
}

/* ===================== */
/* Execute command in current working directory */
/* ===================== */
int execute_cwd(vars_t *vars)
{
    if (!vars || !vars->av || !vars->av[0])
        return 0;

    if (access(vars->av[0], X_OK) == 0)
    {
        execute_direct(vars->av[0], vars);
        return 1;
    }
    return 0;
}

/* ===================== */
/* Execute command via PATH */
/* ===================== */
int path_execute(const char *command, vars_t *vars)
{
    char *path_env = find_path(vars->env);
    if (!path_env)
        return 0;

    char *dup = _strdup(path_env);
    if (!dup)
        return 0;

    char **dirs = tokenize(dup, ":");
    if (!dirs)
    {
        free(dup);
        return 0;
    }

    for (int i = 0; dirs[i]; i++)
    {
        size_t len = _strlen(dirs[i]) + _strlen(command) + 2;
        char *full = malloc(len);
        if (!full)
            continue;

        _strcpy(full, dirs[i]);
        _strcat(full, "/");
        _strcat(full, command);

        if (access(full, X_OK) == 0)
        {
            execute_direct(full, vars);
            free(full);
            free_tokens(dirs);
            free(dup);
            return 1;
        }
        free(full);
    }

    free_tokens(dirs);
    free(dup);
    return 0;
}

/* ===================== */
/* Main PATH checker */
/* ===================== */
void check_for_path(vars_t *vars)
{
    if (!vars || !vars->av || !vars->av[0])
        return;

    /* Command contains '/' -> execute directly */
    if (check_for_dir(vars->av[0]))
    {
        if (!execute_cwd(vars))
            error_cmd(vars->av[0]);
        return;
    }

    /* Try PATH execution */
    if (!path_execute(vars->av[0], vars))
        error_cmd(vars->av[0]);
}

