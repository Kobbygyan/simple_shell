#include "shell.h"

/**
 * get_environ - Get the environment variables as an array of strings
 * @info: Pointer to the info_t struct
 *
 * Return: The environment variables as an array of strings
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->envChanged)
	{
		info->environ = list_to_strings(info->env);
		info->envChanged = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Unset an environment variable
 * @info: Pointer to the info_t struct
 * @var: The variable to unset
 *
 * Return: 1 if the variable was unset, 0 otherwise
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t idx = 0;
	char *ptr;

	if (!node || !var)
		return (0);

	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			info->envChanged = delete_node_at_index(&(info->env), idx);
			idx = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		idx++;
	}
	return (info->envChanged);
}

/**
 * _setenv - Set an environment variable
 * @info: Pointer to the info_t struct
 * @var: The variable to set
 * @value: The value to assign to the variable
 *
 * Return: 0 on success, 1 on failure
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *ptr;

	if (!var || !value)
		return (0);

	buffer = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buffer)
		return (1);
	_strcpy(buffer, var);
	_strcat(buffer, "=");
	_strcat(buffer, value);
	node = info->env;
	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			free(node->str);
			node->str = buffer;
			info->envChanged = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(info->env), buffer, 0);
	free(buffer);
	info->envChanged = 1;
	return (0);
}
