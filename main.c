#include "main.h"
/**
 * main - provides a prompt, reads and displays input
 * @ac: argument count
 * @av: argument vector
 * Return: input
 */
int main(int ac, char **argv)
{
	char *prompt = "simple_shell $";
	char *lineptr;
	size_t n = 0;

	(void)ac; (void)argv;

	printf("%s", prompt);
	getline(&lineptr, &n, stdin);
	printf("%s\n", lineptr);

	free(lineptr);
	return (0);
}
