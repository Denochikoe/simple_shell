#include "shell.h"

/**
 * update_pwd - update the PWD environment variable
 *
 * Return: void
 */
void update_pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		setenv("PWD", cwd, 1);
	}
}

/**
 * _cd - changes the current directory
 * @vars: struct of variables
 *
 * Return: void
 */
void _cd(vars_t *vars)
{
	char *dir;
	char *prev_dir;
	char *cwd;

	if (vars->av[1] && strcmp(vars->av[1], "-") == 0)
	{
		prev_dir = getenv("OLDPWD");
		if (prev_dir == NULL)
		{
			print_error(vars, ": OLDPWD not set\n");
			vars->status = 2;
			return;
		}
		dir = prev_dir;
	}
	else
	{
		if (vars->av[1] == NULL)
		{
			dir = getenv("HOME");
		}
		else
		{
			dir = vars->av[1];
		}
	}
	if (chdir(dir) != 0)
	{
		perror("cd");
		vars->status = 2;
		return;
	}
	cwd = getenv("PWD");
	if (cwd != NULL)
	{
		setenv("OLDPWD", cwd, 1);
	}
	update_pwd();
	vars->status = 0;
}
