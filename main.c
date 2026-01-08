#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

unsigned int sig_flag;

static void sig_handler(int sig)
{
    (void)sig;
    _puts("\n");
    if (!sig_flag)
        _puts("$ ");
}

int main(int argc __attribute__((unused)), char **argv, char **env)
{
    size_t len = 0;
    vars_t vars;
    alias_list_t alias_list = {NULL};
    alias_node_t *cur, *next;

    vars.av = NULL;
    vars.buffer = NULL;
    vars.env = make_env(env);
    vars.count = 0;
    vars.argv = argv;
    vars.status = 0;
    vars.alias_list = &alias_list;

    signal(SIGINT, sig_handler);

    if (isatty(STDIN_FILENO))
        _puts("$ ");

    while (getline(&vars.buffer, &len, stdin) != -1)
    {
        sig_flag = 1;
        vars.count++;

        execute_chained_commands(&vars, vars.buffer);

        sig_flag = 0;
        free(vars.buffer);
        vars.buffer = NULL;

        if (isatty(STDIN_FILENO))
            _puts("$ ");
    }

    _puts("\n");

    for (cur = alias_list.head; cur; cur = next)
    {
        next = cur->next;
        free(cur->name);
        free(cur->value);
        free(cur);
    }

    free_env(vars.env);
    return vars.status;
}

