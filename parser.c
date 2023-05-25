#include "shell.h"

/**
 * is_cmd - Check if the given path is a command
 * @info: Pointer to the info_t structure
 * @path: Path of the file to check
 *
 * Return: 1 if the path is a command, 0 otherwise
 */
int is_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st) != 0)
		return (0);

	if (st.st_mode & S_IFREG)
		return (1);

	return (0);
}

/**
 * dup_chars - Duplicate characters from a string within a specified range
 * @pathstr: The string to duplicate characters from
 * @start: The starting index of the range (inclusive)
 * @stop: The ending index of the range (exclusive)
 *
 * Return: The duplicated string
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i, k = 0;

	for (i = start; i < stop; i++)
	{
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	}

	buf[k] = '\0';
	return (buf);
}

/**
 * find_path - Find the full path of a command in the given path string
 * @info: Pointer to the info_t struct
 * @pathstr: The path string containing multiple paths separated by ':'
 * @cmd: The command to find
 *
 * Return: The full path of the command if found, otherwise NULL
 */
char *find_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);

	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(info, cmd))
			return (cmd);
	}

	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}

			if (is_cmd(info, path))
				return (path);

			if (!pathstr[i])
				break;

			curr_pos = i;
		}
		i++;
	}

	return (NULL);
}
