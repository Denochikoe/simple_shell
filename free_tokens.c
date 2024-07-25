#include "shell.h"
/**
 * free_tokens - free the memory alocated for an array of tokens
 * @token: Array of tokens to be freed
 *
 * Return: void
 */
void free_tokens(char **tokens)
{
	if (tokens != NULL)
	{
		char **token = tokens;

		while (*token != NULL)
		{
			free(*token);
			token++;
		}
		free(tokens);
	}
}
