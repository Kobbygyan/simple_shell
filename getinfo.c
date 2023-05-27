#include "shell.h"

/**
 * clear_info - Clear the fields of the info_t structure
 * @info: Pointer to the info_t structure
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - Set the info_t structure with command line arguments
 * @info: Pointer to the info_t structure
 * @av: Array of strings containing command line arguments
 */
void set_info(info_t *info, char **av)
{
	int idx = 0;

	info->filename = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (idx = 0; info->argv && info->argv[idx]; idx++)
			;
		info->argc = idx;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - Free memory allocated for info_t structure
 * @info: Pointer to the info_t structure
 * @all: Flag indicating whether to free all resources or not
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmmd_buff)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->cmmd_buff);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUFF_FLUSH);
	}
}
