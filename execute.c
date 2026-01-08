#include "shell.h"
#include <stdlib.h>
#include <string.h>

/**
 * execute_chained_commands - executes commands separated by ; && ||
 * @vars: shell variables
 * @buffer: input command line
 */
void execute_chained_commands(vars_t *vars, char *buffer)
{
    int count = 0;
    size_t i;
    int last_status = 0;
    command_t *cmds;

    if (!vars || !buffer)
        return;

    /* Split the input line into commands based on ; && || */
    cmds = split_by_operators(buffer, &count);
    if (!cmds)
        return;

    for (i = 0; i < (size_t)count; i++)
    {
        /* Handle && and || logic */
        if (i > 0)
        {
            if (cmds[i - 1].op == '&' && last_status != 0)
                continue;
            if (cmds[i - 1].op == '|' && last_status == 0)
                continue;
        }

        /* Free previous arguments */
        if (vars->av)
        {
            free_tokens(vars->av);
            vars->av = NULL;
        }

        if (!cmds[i].cmd)
            continue;

        /* Tokenize current command */
        vars->av = tokenize(cmds[i].cmd, " \t\n");
        if (!vars->av || !vars->av[0])
            continue;

        /* ========================= */
        /* Alias expansion           */
        /* ========================= */
        if (vars->alias_list && vars->av[0])
        {
            /* Prevent infinite alias loops */
            int alias_depth = 0;
            char *alias_val = NULL;
            char *cmd_name = vars->av[0];

            while ((alias_val = find_alias(vars->alias_list, cmd_name)) != NULL)
            {
                alias_depth++;
                if (alias_depth > 10) /* safety limit */
                    break;

                free_tokens(vars->av);
                vars->av = tokenize(alias_val, " \t\n");
                if (!vars->av || !vars->av[0])
                    break;

                cmd_name = vars->av[0];
            }
        }

        /* Execute builtin or PATH command */
        if (!check_for_builtins(vars))
            check_for_path(vars);

        /* Save last status for && and || */
        last_status = vars->status;
    }

    /* Free the command array */
    free_command_array(cmds, count);
}

