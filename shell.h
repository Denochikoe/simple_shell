#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

/*
 * struct alias_node_s - Node in the alias list.
 * @name: The name of the alias.
 * @value: The value associated with the alias.
 * @next: Pointer to the next node in the list.
 *
 * Description: This structure represents a node in a singly
 * linked list used for storing alias definitions.
 */
typedef struct alias_node
{
	char *name;
	char *value;
	struct alias_node *next;
} alias_node_t;

/**
 * struct - A list of aliases.
 * @head: Pointer to the first node in the alias list.
 *
 * Description: This structure represents a list of aliases,
 * starting with the head node of a singly linked list.
 */
typedef struct
{
	alias_node_t *head;
} alias_list_t;

/**
 * struct vars_s - variables used in the shell.
 * @av: command line arguments
 * @buffer: buffer of command
 * @env: environment variables
 * @count: count of commands entered
 * @argv: arguments at opening of shell
 * @status: exit status
 * @commands: commands to execute
 * @alias_list: Pointer to the alias list.
 *
 * Description: This structure holds various variables and
 * data used throught the shell's execution.
 */
typedef struct vars_s
{
	char **av;
	char *buffer;
	char **env;
	size_t count;
	char **argv;
	int status;
	char **commands;
} vars_t;

/**
 * struct builtins - struct for the builtin functions
 * @name: name of builtin command
 * @f: function for corresponding builtin
 */
typedef struct builtins
{
	char *name;
	void (*f)(vars_t *);
} builtins_t;

char **make_env(char **env);
void free_env(char **env);

ssize_t _puts(char *str);
char *_strdup(char *strodup);
int _strcmpr(char *strcmp1, char *strcmp2);
char *_strcat(char *strc1, char *strc2);
unsigned int _strlen(char *str);

char **tokenize(char *buffer, char *delimiter);
char **_realloc(char **ptr, size_t *size);
char *new_strtok(char *str, const char *delim);

void (*check_for_builtins(vars_t *vars))(vars_t *vars);
void new_exit(vars_t *vars);
void _env(vars_t *vars);
void new_setenv(vars_t *vars);
void new_unsetenv(vars_t *vars);

void add_key(vars_t *vars);
char **find_key(char **env, char *key);
char *add_value(char *key, char *value);
int _atoi(char *str);

void check_for_path(vars_t *vars);
int path_execute(char *command, vars_t *vars);
char *find_path(char **env);
int execute_cwd(vars_t *vars);
int is_absolute_path(char *str);

void print_error(vars_t *vars, char *msg);
void _puts2(char *str);
char *_uitoa(unsigned int count);
void _cd(vars_t *vars);
void update_pwd(void);

void add_alias(alias_list_t *alias_list, const char *name, const char *value);
void print_aliases(const alias_list_t *alias_list);
char *find_alias(const alias_list_t *alias_list, const char *name);
void handle_alias_command(alias_list_t *alias_list, vars_t *vars);

#endif /*_SHELL_H_ */
