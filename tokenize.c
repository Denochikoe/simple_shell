#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ===================== */
/* Helper: check if char is in delimiter string */
/* ===================== */
static int check_match(char c, const char *str)
{
    size_t i;

    for (i = 0; str[i]; i++)
        if (c == str[i])
            return 1;

    return 0;
}

/* ===================== */
/* Custom strtok that handles quotes */
/* ===================== */
char *new_strtok(char *str, const char *delim)
{
    static char *next;
    char *start;
    int in_quotes = 0;
    char quote_char = 0;
    size_t i;

    if (str)
        next = str;

    if (!next)
        return NULL;

    /* Skip leading delimiters */
    while (*next && check_match(*next, delim))
        next++;

    if (!*next)
        return NULL;

    start = next;

    for (i = 0; next[i]; i++)
    {
        if ((next[i] == '\'' || next[i] == '"') && !in_quotes)
        {
            in_quotes = 1;
            quote_char = next[i];
        }
        else if (in_quotes && next[i] == quote_char)
        {
            in_quotes = 0;
        }
        else if (!in_quotes && check_match(next[i], delim))
        {
            next[i] = '\0';
            next += i + 1;
            return start;
        }
    }

    next = NULL;
    return start;
}

/* ===================== */
/* Tokenize buffer into array of strings */
/* ===================== */
char **tokenize(char *buffer, const char *delimiter)
{
    size_t capacity = 10, i = 0;
    char **tokens;
    char *token;

    if (!buffer || !delimiter)
        return NULL;

    tokens = malloc(sizeof(char *) * capacity);
    if (!tokens)
        return NULL;

    while ((token = new_strtok(buffer, delimiter)))
    {
        buffer = NULL;

        /* Strip surrounding quotes */
        size_t len = _strlen(token);
        if (len >= 2 &&
            (token[0] == '\'' || token[0] == '"') &&
            token[len - 1] == token[0])
        {
            token[len - 1] = '\0';
            token++;
        }

        tokens[i] = _strdup(token);
        if (!tokens[i])
        {
            while (i > 0)
                free(tokens[--i]);
            free(tokens);
            return NULL;
        }

        i++;
        if (i + 1 == capacity)
        {
            char **tmp = _realloc(tokens, &capacity);
            if (!tmp)
            {
                while (i > 0)
                    free(tokens[--i]);
                free(tokens);
                return NULL;
            }
            tokens = tmp;
        }
    }

    tokens[i] = NULL;
    return tokens;
}

