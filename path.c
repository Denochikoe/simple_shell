#include "shell.h"

/**
 * path_execute - executes a command in the path
 * @command: full path to the command
 * @vars: pointer to struct of variables
 *
 * Return: 0 on success, 1 on failure
 */
int path_execute(char *command, vars_t *vars)
{
	pid_t child_pid;
	int status;

	if (access(command, X_OK) == 0)
	{
		child_pid = fork();
		if (child_pid == -1)
			print_error(vars, NULL);
		if (child_pid == 0)
		{
			if (execve(command, vars->av, vars->env) == -1)
				print_error(vars, NULL);
		}
		else
		{
			if (wait(&status) == 0)
				print_error(vars, "wait failed\n");
			if (WIFEXITED(status))
				vars->status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				vars->status = 130;
			return (0);
		}
		vars->status = 127;
		return (1);
	}
	else
	{
		print_error(vars, ": Permission denied\n");
		vars->status = 126;
	}
	return (0);
}

/**
 * find_path - finds the PATH variable
 * @env: array of environment variables
 *
 * Return:pointer to the node that contains the PATH, or NULL
 * on failure
 */
char *find_path(char **env)
{
	char *path = "PATH=";
	unsigned int i, j;

	for (i = 0; env[i] != NULL; i++)
	{
		for (j = 0; j < 5; j++)
			if (path[j] != env[i][j])
				break;
		if (j == 5)
			break;
	}
	free(path);
	return (env[i]);
}

/**
 * check_for_path - checks if the command is in the PATH
 * @vars: variables
 *
 * Return: void
 */
void check_for_path(vars_t *vars)
{
	char *path, *path_dup = NULL, *check = NULL;
	unsigned int i = 0;
	int execution_result = 0;
	char **path_tokens;
	struct stat buf;

	if (is_absolute_path(vars->av[0]))
		execution_result = execute_cwd(vars);
	else
	{
		path = find_path(vars->env);
		if (path != NULL)
		{
			path_dup = _strdup(path + 5);
			if (path_dup == NULL)
			{
				print_error(vars, "Memory allocation failed\n");
				vars->status = 127;
				return;
			}
			path_tokens = tokenize(path_dup, ":");
			if (path_tokens == NULL)
			{
				print_error(vars, "Memory allocation failed\n");
				vars->status = 127;
				free(path_dup);
				return;
			}
			for (i = 0; path_tokens && path_tokens[i]; i++, free(check))
			{
				check = _strcat(path_tokens[i], vars->av[0]);
				if (stat(check, &buf) == 0)
				{
					execution_result = path_execute(check, vars);
					free(check);
					break;
				}
			}
			free(path_dup);
			free(path_tokens);
		}
		else
		{
			print_error(vars, ": not found\n");
			vars->status = 127;
		}
	}
	if (execution_result == 1)
		new_exit(vars);
}

/**
 * execute_cwd - executes the command in the current working
 * directory
 * @vars: pointer to struct of variables
 *
 * Return: 0 on success, 1 on failure
 */
int execute_cwd(vars_t *vars)
{
	pid_t child_pid;
	struct stat buf;
	int status;

	if (vars->av[0] == NULL && stat(vars->av[0], &buf) == 0)
	{
		if (access(vars->av[0], X_OK) == 0)
		{
			child_pid = fork();
			if (child_pid == -1)
				print_error(vars, NULL);
			if (child_pid == 0)
			{
				if (execve(vars->av[0], vars->av, vars->env) == -1)
					print_error(vars, NULL);
			}
			else
			{
				if (wait(&status) == 0)
					print_error(vars, "wait failed\n");
				if (WIFEXITED(status))
					vars->status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
					vars->status = 130;
				return (0);
			}
			vars->status = 127;
			return (1);
		}
		else
		{
			print_error(vars, ": Permission denied\n");
			vars->status = 126;
		}
	}
	else
	{
		print_error(vars, ": not found\n");
		vars->status = 127;
	}
	return (0);
}

/**
 * is_absolute_path - checks if the command is a part of a path
 * @str: command
 *
 * Return: 1 on success, 0 on failure
 */
int is_absolute_path(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
}
