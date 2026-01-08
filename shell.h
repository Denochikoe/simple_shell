#ifndef SHELL_H
#define SHELL_H

/* ===================== */
/* Standard headers      */
/* ===================== */
#include <stddef.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* ===================== */
/* Alias structures      */
/* ===================== */
typedef struct alias_node
{
    char *name;
    char *value;
    struct alias_node *next;
} alias_node_t;

typedef struct alias_list
{
    alias_node_t *head;
} alias_list_t;

/* ===================== */
/* Command chaining      */
/* ===================== */
typedef struct command_s
{
    char *cmd;
    char op;
} command_t;

/* ===================== */
/* Shell variables       */
/* ===================== */
typedef struct vars_s
{
    char **av;               /* tokenized arguments */
    char *buffer;            /* raw input buffer */
    char **env;              /* environment array */
    unsigned int count;      /* command count */
    char **argv;             /* argv from main */
    int status;              /* last command exit status */
    alias_list_t *alias_list;/* pointer to alias list */
} vars_t;

/* ===================== */
/* Builtins table        */
/* ===================== */
typedef struct builtins_s
{
    char *name;
    int (*func)(vars_t *);
} builtins_t;

/* ===================== */
/* Builtin functions     */
/* ===================== */
int builtin_cd(vars_t *vars);
int builtin_pwd(vars_t *vars);
int builtin_echo(vars_t *vars);
int builtin_env(vars_t *vars);
int builtin_exit(vars_t *vars);
int builtin_setenv(vars_t *vars);
int builtin_unsetenv(vars_t *vars);
int handle_alias_command(vars_t *vars);
int check_for_builtins(vars_t *vars);

/* ===================== */
/* Alias helpers         */
/* ===================== */
void add_alias(alias_list_t *alias_list, const char *name, const char *value);
char *find_alias(const alias_list_t *alias_list, const char *name);
void print_aliases(const alias_list_t *alias_list);

/* ===================== */
/* Environment helpers   */
/* ===================== */
char **make_env(char **env);
void free_env(char **env);
char **find_key(char **env, const char *key);
char *add_value(const char *key, const char *value);
void add_key(vars_t *vars);

/* ===================== */
/* Execution             */
/* ===================== */
void execute_chained_commands(vars_t *vars, char *buffer);
command_t *split_by_operators(char *buffer, int *count);
void free_command_array(command_t *arr, int count);

void check_for_path(vars_t *vars);
int execute_cwd(vars_t *vars);
int path_execute(const char *command, vars_t *vars); /* updated to const */
char *find_path(char **env);
int check_for_dir(const char *str);                 /* updated to const */

/* ===================== */
/* Tokenization          */
/* ===================== */
char **tokenize(char *buffer, const char *delimiter);
char *new_strtok(char *str, const char *delim);
void free_tokens(char **tokens);

/* ===================== */
/* Utilities             */
/* ===================== */
int _atoi(const char *str);
unsigned int _strlen(const char *str);
int _strcmpr(const char *s1, const char *s2);
int _strcmpr_n(const char *s1, const char *s2, size_t n);
char *_strdup(const char *str);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
char **_realloc(char **ptr, size_t *size);

/* ===================== */
/* Output / errors       */
/* ===================== */
ssize_t _puts(const char *str);
void _puts2(const char *str);
void print_error(vars_t *vars, const char *msg);
void error_cmd(const char *cmd);
char *_uitoa(unsigned int count);

#endif /* SHELL_H */

