#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define PROMPT "simple_shell$ "

/**
 * print_error - Prints an error message
 * @msg: The error message to print
 */
void print_error(const char *msg)
{
	perror(msg);
}
/**
 * shell_exit - Exits the shell
 *
 * Return: void.
 */
void shell_exit(void)
{
	printf("Exiting shell...\n");
	exit(EXIT_SUCCESS);
}


/**
 * main - Entry point of the simple shell program
 *
 * This function displays a prompt, reads user input,
 * executes the input command, and then displays the prompt
 * again
 * The shell will handle errors, and it will exit gracefully
 * when it encounters end-of-file (Ctrl-D) or an interrupt
 * (Ctrl+C).
 *
 * Return: Always return 0 to indicate success.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;
	extern char **environ;

	while (1)
	{
		printf(PROMPT);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			else {
				print_error("getline");
				continue;
			}
		}
		if (line[nread - 1] == '\n')
		{
			line[nread - 1] = '\0';
		}
		if (strlen(line) == 0)
		{
			continue;
		}
		if (strcmp(line, "exit") == 0)
		{
			shell_exit();
		}
		else if (strcmp(line, "help") == 0)
		{
			printf("Help command not implemented.\n");
			continue;
		}
		pid = fork();
		if (pid == -1)
		{
			print_error("fork");
			continue;
		}
		else if (pid == 0)
		{
			char *argv[2];
			argv[0] = line;
			argv[1] = NULL;
			if (execve(line, argv, environ) == -1)
			{
				print_error(line);
				exit(EXIT_FAILURE);
			}
			else {
				if (wait(&status) == -1)
				{
					print_error("wait");
				}
			}
		}
	}
		free(line);
		return (0);
}
