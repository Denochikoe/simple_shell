#include <stdio.h>
#include <unistd.h>
/**
 * main - forks the child process
 * Return: Always 0
 */
int main()
{
	int child_pid = fork();

	if (child_pid == 0)
	{
		printf("I am the child process\n");
	}
	else
	{
		printf("I am the parent process. The child process id is %d\n", child_pid);
	}
	return (0);
}
