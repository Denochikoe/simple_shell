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
	char *dir, *oldpwd, *home;
	char cwd[1024];

	if (vars->av[1] == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				_puts(cwd);
				_puts("\n");
			}
			vars->status = 0;
			return;
		}
		dir = home;
	}
	else if (_strcmpr(vars->av[1], "-") == 0)
	{
		oldpwd = getenv("OLDPWD");
		if (oldpwd == NULL)
		{
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				_puts(cwd);
				_puts("\n");
			}
			print_error(vars, ": OLDPWD not set\n");
			vars->status = 1;
			return;
		}
		dir = oldpwd;
	}
	else
	{
		dir = vars->av[1];
	}
	oldpwd = getenv("PWD");
	if (oldpwd != NULL)
	{
		if (setenv("OLDPWD", oldpwd, 1) == -1)
		{
			print_error(vars, ": failed to set OLDPWD\n");
			vars->status = 1;
			return;
		}
	}
	if (chdir(dir) == -1)
	{
		print_error(vars, ": can't cd to ");
		_puts(dir);
		_puts("\n");
		vars->status = 1;
		return;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (setenv("PWD", cwd, 1) == -1)
		{
			print_error(vars, ": failed to set PWD\n");
			vars->status = 1;
			return;
		}
	}
	vars->status = 0;
}
