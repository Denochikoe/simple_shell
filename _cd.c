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
	char *dir, *home;
	char *oldpwd;
	struct stat st;

	if (vars->av[1] == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			print_error(vars, ": HOME not set\n");
			vars->status = 1;
			return;
		}
		dir = home;
	}
	else if (_strcmpr(vars->av[1], "-") == 0)
	{
		oldpwd = getenv("OLDPWD");
		if (oldpwd == NULL)
		{
			print_error(vars, ": OLDPWD not set\n");
			vars->status = 1;
			return;
		}
		dir = oldpwd;
		_puts(dir);
		_puts("\n");
	}
	else
	{
		dir = vars->av[1];
	}
	if (stat(dir, &st) != 0)
	{
		print_error(vars, ": No such file or directory\n");
		vars->status = 1;
		return;
	}
	if (chdir(dir) != 0)
	{
		perror("cd");
		vars->status = 1;
		return;
	}
	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", dir, 1);
	vars->status = 0;
}
