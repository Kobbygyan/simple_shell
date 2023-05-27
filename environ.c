#include "shell.h"

/**
 * _myenv - Prints the environment variables
 * @info: Pointer to the structure containing the environment variables
 *
 * Return: Always 0
 */
int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - Retrieves the value of an environment variable
 * @info: Pointer to the structure containing the environment variables
 * @name: Name of the environment variable to retrieve
 *
 * Return: Pointer to value of the environment variable, or NULL if not found
 */
char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *ptr;

	while (node)
	{
		ptr = starts_with(node->str, name);
		if (ptr && *ptr)
			return (ptr);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Set a new environment variable or modify an existing one
 * @info: Pointer to the structure containing the environment variables
 *
 * Return: 1 on success, 0 on failure
 */
int _mysetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}

	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);

	return (1);
}

/**
 * _myunsetenv - Remove environment variables
 * @info: Pointer to the structure containing the environment variables
 *
 * Return: 0 on success, 1 on failure
 */
int _myunsetenv(info_t *info)
{
	int idx;

	if (info->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}

	for (idx = 1; idx < info->argc; idx++)
		_unsetenv(info, info->argv[idx]);

	return (0);
}

/**
 * populate_env_list - Populate environment variables into a linked list
 * @info: Pointer to the structure containing the environment variables
 *
 * Return: Always 0
 */
int populate_env_list(info_t *info)
{
	list_t *env_node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&env_node, environ[i], 0);
	info->env = env_node;
	return (0);
}
