#include "shell.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * make_env - duplicate the environment array passed to main
 * @env: environment passed to main
 *
 * Return: pointer to the new duplicated environment
 */
char **make_env(char **env)
{
    size_t i;
    char **newenv;

    if (!env)
        return NULL;

    /* Count entries */
    for (i = 0; env[i] != NULL; i++)
        ;

    /* Allocate new environment array (+1 for NULL terminator) */
    newenv = malloc(sizeof(char *) * (i + 1));
    if (!newenv)
    {
        perror("Fatal Error");
        exit(1);
    }

    /* Duplicate each string */
    for (i = 0; env[i] != NULL; i++)
        newenv[i] = _strdup(env[i]);

    newenv[i] = NULL;
    return newenv;
}

/**
 * free_env - free a duplicated environment
 * @env: environment array
 */
void free_env(char **env)
{
    unsigned int i;

    if (!env)
        return;

    for (i = 0; env[i] != NULL; i++)
        free(env[i]);

    free(env);
}

/**
 * find_key - find an environment variable by key
 * @env: environment array
 * @key: key to search for
 *
 * Return: pointer to matching env entry, or NULL if not found
 */
char **find_key(char **env, const char *key)
{
    size_t key_len;
    unsigned int i;

    if (!env || !key)
        return NULL;

    key_len = _strlen(key);

    for (i = 0; env[i]; i++)
    {
        if (_strcmpr_n(env[i], key, key_len) == 0 && env[i][key_len] == '=')
        {
            return &env[i];
        }
    }

    return NULL;
}

