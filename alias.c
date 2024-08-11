#include "shell.h"

/**
 * add_alias - Adds or updates an alias in the list
 * @alias_list: Pointer to the alias list
 * @name: The alias name
 * @value: The alias value
 */
void add_alias(alias_list_t *alias_list, const char *name, const char *value)
{
	alias_node_t *new_node;
	alias_node_t *current = alias_list->head;

	while (current)
	{
		if (strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = strdup(value);
			if (current->value == NULL)
			{
				perror("strdup");
				exit(EXIT_FAILURE);
			}
			return;
		}
		current = current->next;
	}

	new_node = malloc(sizeof(alias_node_t));
	if (new_node == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->name = strdup(name);
	if (new_node->name == NULL)
	{
		free(new_node);
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	new_node->value = strdup(value);
	if (new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node);
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	new_node->next = alias_list->head;
	alias_list->head = new_node;
}

/**
 * print_aliases - Prints all aliases in the list
 * @alias_list: Pointer to the alias list
 */
void print_aliases(const alias_list_t *alias_list)
{
	alias_node_t *current = alias_list->head;

	while (current)
	{
		printf("%s='%s'\n", current->name, current->value);
		current = current->next;
	}
}

/**
 * find_alias - Finds an alias by name
 * @alias_list: Pointer to the alias list
 * @name: The alias name
 *
 * Return: The alias value if found, NULL otherwise
 */
char *find_alias(const alias_list_t *alias_list, const char *name)
{
	alias_node_t *current = alias_list->head;

	while (current)
	{
		if (strcmp(current->name, name) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return NULL;
}

/**
 * handle_alias_command - Handles the 'alias' command
 * @alias_list: Pointer to the alias list
 * @vars: Pointer to the variable struct
 */
void handle_alias_command(alias_list_t *alias_list, vars_t *vars)
{
	int i;
	char *equals_pos;
	char *alias_value;

	if (vars->av[1] == NULL)
	{
		print_aliases(alias_list);
	}
	else
	{
		for (i = 1; vars->av[i]; i++)
		{
			if (vars->av[i] == NULL)
				break;

			equals_pos = strchr(vars->av[i], '=');
			if (equals_pos != NULL)
			{
				*equals_pos = '\0';
				alias_value = equals_pos + 1;
				add_alias(alias_list, vars->av[i], alias_value);
			}
			else
			{
				printf("%s\n", find_alias(alias_list, vars->av[i]));
			}
		}
	}
}
