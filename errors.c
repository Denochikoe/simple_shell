#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * _puts2 - print string to stderr
 */
void _puts2(const char *str)
{
    ssize_t len;

    if (!str)
        return;

    len = _strlen(str);
    if (write(STDERR_FILENO, str, len) != len)
    {
        perror("write");
        exit(1);
    }
}

/**
 * _uitoa - unsigned int to string
 */
char *_uitoa(unsigned int n)
{
    char buffer[20];
    int i = 18;

    buffer[19] = '\0';

    if (n == 0)
        buffer[i--] = '0';

    while (n > 0)
    {
        buffer[i--] = (n % 10) + '0';
        n /= 10;
    }

    return _strdup(&buffer[i + 1]);
}

/**
 * print_error - formatted shell error
 */
void print_error(vars_t *vars, const char *msg)
{
    char *count;

    if (!vars || !vars->av || !vars->av[0])
        return;

    _puts2(vars->argv[0]);
    _puts2(": ");

    count = _uitoa(vars->count);
    if (count)
    {
        _puts2(count);
        free(count);
    }

    _puts2(": ");
    _puts2(vars->av[0]);

    if (msg)
        _puts2(msg);
}

/**
 * error_cmd - command not found (path.c)
 */
void error_cmd(const char *cmd)
{
    _puts2(cmd);
    _puts2(": not found\n");
}

