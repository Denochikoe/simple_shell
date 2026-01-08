#include "shell.h"
#include <stdlib.h>
#include <string.h>

/**
 * split_by_operators - splits buffer by ;, &&, ||
 * @buffer: input command line
 * @count: pointer to store number of commands
 *
 * Return: array of command_t
 */
command_t *split_by_operators(char *buffer, int *count)
{
    int i = 0, cmd_start = 0, cmd_len;
    int len;
    command_t *commands;
    int cmd_index = 0;

    if (!buffer || !count)
        return NULL;

    len = _strlen(buffer);
    commands = malloc(sizeof(command_t) * (len / 2 + 1));
    if (!commands)
        return NULL;

    *count = 0;

    while (i <= len)
    {
        int is_op = 0;
        char op = 0;

        if (buffer[i] == '\0' || buffer[i] == ';')
        {
            is_op = 1;
            op = buffer[i];
        }
        else if (buffer[i] == '&' && buffer[i + 1] == '&')
        {
            is_op = 1;
            op = '&';
            i++;
        }
        else if (buffer[i] == '|' && buffer[i + 1] == '|')
        {
            is_op = 1;
            op = '|';
            i++;
        }

        if (is_op)
        {
            cmd_len = i - cmd_start;

            /* Trim leading spaces */
            while (cmd_len > 0 && (buffer[cmd_start] == ' ' || buffer[cmd_start] == '\t'))
            {
                cmd_start++;
                cmd_len--;
            }

            /* Trim trailing spaces */
            while (cmd_len > 0 && (buffer[cmd_start + cmd_len - 1] == ' ' || buffer[cmd_start + cmd_len - 1] == '\t'))
            {
                cmd_len--;
            }

            if (cmd_len > 0)
            {
                commands[cmd_index].cmd = malloc(cmd_len + 1);
                if (!commands[cmd_index].cmd)
                {
                    for (int j = 0; j < cmd_index; j++)
                        free(commands[j].cmd);
                    free(commands);
                    return NULL;
                }

                strncpy(commands[cmd_index].cmd, buffer + cmd_start, cmd_len);
                commands[cmd_index].cmd[cmd_len] = '\0';
                commands[cmd_index].op = op;

                cmd_index++;
            }

            cmd_start = i + 1;
        }

        i++;
    }

    *count = cmd_index;
    return commands;
}

/**
 * free_command_array - frees an array of command_t
 * @arr: array to free
 * @count: number of commands
 */
void free_command_array(command_t *arr, int count)
{
    if (!arr)
        return;

    for (int i = 0; i < count; i++)
        free(arr[i].cmd);

    free(arr);
}

