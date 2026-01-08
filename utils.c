#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/* ===================== */
/* String utilities      */
/* ===================== */

/* String length */
unsigned int _strlen(const char *s)
{
    unsigned int len = 0;

    if (!s)
        return 0;

    while (s[len])
        len++;

    return len;
}

/* Compare two strings */
int _strcmpr(const char *a, const char *b)
{
    if (!a || !b)
        return -1;

    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

/* Compare first n characters */
int _strcmpr_n(const char *a, const char *b, size_t n)
{
    size_t i;

    if (!a || !b)
        return -1;

    for (i = 0; i < n; i++)
    {
        if (a[i] != b[i])
            return (unsigned char)a[i] - (unsigned char)b[i];

        if (a[i] == '\0')
            break;
    }

    return 0;
}

/* Duplicate a string */
char *_strdup(const char *s)
{
    size_t len, i;
    char *copy;

    if (!s)
        return NULL;

    len = _strlen(s);
    copy = malloc(len + 1);
    if (!copy)
        return NULL;

    for (i = 0; i <= len; i++)
        copy[i] = s[i];

    return copy;
}

/* Copy string */
char *_strcpy(char *dest, const char *src)
{
    size_t i = 0;

    if (!dest || !src)
        return dest;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return dest;
}

/* Concatenate two strings */
char *_strcat(char *dest, const char *src)
{
    size_t i = 0, len;

    if (!dest || !src)
        return dest;

    len = _strlen(dest);
    while (src[i])
    {
        dest[len + i] = src[i];
        i++;
    }

    dest[len + i] = '\0';
    return dest;
}

/* ===================== */
/* Output utilities      */
/* ===================== */

/* Write string to stdout */
ssize_t _puts(const char *s)
{
    ssize_t len;

    if (!s)
        return 0;

    len = _strlen(s);
    if (write(STDOUT_FILENO, s, len) != len)
    {
        perror("write");
        return -1;
    }

    return len;
}

