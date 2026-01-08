#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ===================== */
/* Add or update alias   */
/* ===================== */
void add_alias(alias_list_t *list, const char *name, const char *value)
{
    alias_node_t *cur;

    if (!list || !name || !value)
        return;

    cur = list->head;
    while (cur)
    {
        if (strcmp(cur->name, name) == 0)
        {
            free(cur->value);
            cur->value = _strdup(value);
            return;
        }
        cur = cur->next;
    }

    cur = malloc(sizeof(alias_node_t));
    if (!cur)
        return;

    cur->name = _strdup(name);
    cur->value = _strdup(value);
    cur->next = list->head;
    list->head = cur;
}

/* ===================== */
/* Find alias            */
/* ===================== */
char *find_alias(const alias_list_t *list, const char *name)
{
    alias_node_t *cur;

    if (!list || !name)
        return NULL;

    cur = list->head;
    while (cur)
    {
        if (strcmp(cur->name, name) == 0)
            return cur->value;
        cur = cur->next;
    }

    return NULL;
}

/* ===================== */
/* Print all aliases     */
/* ===================== */
void print_aliases(const alias_list_t *list)
{
    alias_node_t *cur;

    if (!list)
        return;

    cur = list->head;
    while (cur)
    {
        printf("%s='%s'\n", cur->name, cur->value);
        cur = cur->next;
    }
}

/* ===================== */
/* Alias builtin handler */
/* ===================== */
int handle_alias_command(vars_t *vars)
{
    int i;
    char *eq, *value;

    if (!vars || !vars->alias_list)
        return 0;

    /* No args → print all */
    if (!vars->av[1])
    {
        print_aliases(vars->alias_list);
        return 1;
    }

    for (i = 1; vars->av[i]; i++)
    {
        eq = strchr(vars->av[i], '=');
        if (eq)
        {
            *eq = '\0';
            add_alias(vars->alias_list, vars->av[i], eq + 1);
            *eq = '=';
        }
        else
        {
            value = find_alias(vars->alias_list, vars->av[i]);
            if (value)
                printf("%s='%s'\n", vars->av[i], value);
        }
    }

    return 1;
}

