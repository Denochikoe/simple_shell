#include "shell.h"
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

/* ===================== */
/* Helper: update or add env key */
/* ===================== */
static void update_env(vars_t *vars, const char *key, const char *value)
{
    char **entry;
    char *newval;

    if (!vars || !key || !value)
        return;

    entry = find_key(vars->env, key);
    newval = add_value(key, value);
    if (!newval)
        return;

    if (entry)
    {
        free(*entry);
        *entry = newval;
    }
    else
    {
        /* add_key expects vars->av[1] and vars->av[2] */
        char *old1 = vars->av[1];
        char *old2 = vars->av[2];

        vars->av[1] = (char *)key;
        vars->av[2] = (char *)value;
        add_key(vars);

        vars->av[1] = old1;
        vars->av[2] = old2;

        free(newval); /* add_key created its own copy */
    }
}

/* ===================== */
/* Builtin: cd           */
/* ===================== */
int builtin_cd(vars_t *vars)
{
    char cwd[PATH_MAX];
    char *dir;

    if (!vars || !vars->av)
        return 1;

    dir = vars->av[1];

    /* No argument → HOME */
    if (!dir)
    {
        char **home = find_key(vars->env, "HOME");
        if (!home)
        {
            print_error(vars, ": HOME not set\n");
            vars->status = 1;
            return 1;
        }
        dir = *home + 5; /* skip "HOME=" */
    }

    if (chdir(dir) != 0)
    {
        print_error(vars, ": can't cd to ");
        print_error(vars, dir);
        _puts("\n");
        vars->status = 1;
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)))
        update_env(vars, "PWD", cwd);

    vars->status = 0;
    return 1;
}

