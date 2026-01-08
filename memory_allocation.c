#include "shell.h"
#include <stdlib.h>

/**
 * _realloc - reallocates a NULL-terminated array of strings
 * @ptr: pointer to old array
 * @size: pointer to number of allocated elements
 *
 * Return: pointer to new array
 */
char **_realloc(char **ptr, size_t *size)
{
    size_t i, new_size;
    char **new;

    if (!size)
        return NULL;

    new_size = *size + 10;
    new = malloc(sizeof(char *) * new_size);
    if (!new)
    {
        free(ptr);
        return NULL;
    }

    for (i = 0; i < *size; i++)
        new[i] = ptr[i];

    *size = new_size;
    free(ptr);

    return new;
}

/**
 * free_tokens - frees a NULL-terminated array of strings
 * @tokens: array of strings
 */
void free_tokens(char **tokens)
{
    size_t i;

    if (!tokens)
        return;

    for (i = 0; tokens[i]; i++)
        free(tokens[i]);

    free(tokens);
}

