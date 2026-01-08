#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

/* ===================== */
/* Utility: safe atoi    */
/* ===================== */
int _atoi(const char *str)
{
    unsigned int i, res = 0;

    if (!str)
        return -1;

    for (i = 0; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return -1;
        res = res * 10 + (str[i] - '0');
    }
    return res;
}

/* ===================== */
/* Utility: KEY=VALUE    */
/* ===================== */
char *add_value(const char *key, const char *value)
{
    size_t len1 = _strlen(key);
    size_t len2 = _strlen(value);
    char *s = malloc(len1 + len2 + 2);

    if (!s)
        return NULL;

    _strcpy(s, key);
    s[len1] = '=';
    _strcpy(s + len1 + 1, value);

    return s;
}

/* ===================== */
/* Builtin: setenv core  */
/* ===================== */
void add_key(vars_t *vars)
{
    size_t i = 0;
    char **newenv;

    if (!vars || !vars->av[1] || !vars->av[2])
    {
        print_error(vars, "Usage: setenv KEY VALUE\n");
        vars->status = 1;
        return;
    }

    while (vars->env[i])
        i++;

    newenv = malloc(sizeof(char *) * (i + 2));
    if (!newenv)
    {
        vars->status = 127;
        return;
    }

    for (size_t j = 0; j < i; j++)
        newenv[j] = vars->env[j];

    newenv[i] = add_value(vars->av[1], vars->av[2]);
    newenv[i + 1] = NULL;

    free(vars->env);
    vars->env = newenv;
}

/* ===================== */
/* Builtin dispatcher    */
/* ===================== */
int check_for_builtins(vars_t *vars)
{
    if (!vars || !vars->av || !vars->av[0])
        return 0;

    if (_strcmpr(vars->av[0], "alias") == 0)
        return handle_alias_command(vars);
    if (_strcmpr(vars->av[0], "exit") == 0)
        return builtin_exit(vars);
    if (_strcmpr(vars->av[0], "env") == 0)
        return builtin_env(vars);
    if (_strcmpr(vars->av[0], "setenv") == 0)
        return builtin_setenv(vars);
    if (_strcmpr(vars->av[0], "unsetenv") == 0)
        return builtin_unsetenv(vars);
    if (_strcmpr(vars->av[0], "cd") == 0)
        return builtin_cd(vars);
    if (_strcmpr(vars->av[0], "echo") == 0)
        return builtin_echo(vars);
    if (_strcmpr(vars->av[0], "pwd") == 0)
        return builtin_pwd(vars);

    return 0;
}

/* ===================== */
/* Individual builtins   */
/* ===================== */

int builtin_exit(vars_t *vars)
{
    unsigned int status = 0;

    if (vars->av[1])
        status = _atoi(vars->av[1]);

    free_env(vars->env);
    free(vars->buffer);
    exit(status);
}

int builtin_env(vars_t *vars)
{
    for (size_t i = 0; vars->env[i]; i++)
    {
        _puts(vars->env[i]);
        _puts("\n");
    }
    return 1;
}

int builtin_setenv(vars_t *vars)
{
    add_key(vars);
    return 1;
}

int builtin_unsetenv(vars_t *vars)
{
    char **newenv = NULL;
    size_t i, j = 0;

    if (!vars->av[1])
        return 1;

    for (i = 0; vars->env[i]; i++)
    {
        if (_strcmpr_n(vars->env[i], vars->av[1],
                       _strlen(vars->av[1])) != 0)
        {
            newenv = _realloc(newenv, &j);
            newenv[j++] = _strdup(vars->env[i]);
        }
    }

    newenv = _realloc(newenv, &j);
    newenv[j] = NULL;

    free_env(vars->env);
    vars->env = newenv;
    return 1;
}

int builtin_echo(vars_t *vars)
{
    for (size_t i = 1; vars->av[i]; i++)
    {
        _puts(vars->av[i]);
        if (vars->av[i + 1])
            _puts(" ");
    }
    _puts("\n");
    return 1;
}

int builtin_pwd(vars_t *vars)
{
    char cwd[PATH_MAX];
    (void)vars;

    if (getcwd(cwd, sizeof(cwd)))
    {
        _puts(cwd);
        _puts("\n");
    }
    return 1;
}

