#include "shell.h"

/**
 * get_history_file - Retrieves the path to the history file
 * @info: Pointer to the info_t struct
 *
 * Return: Pointer to the history file path, or NULL if not found
 */
char *get_history_file(info_t *info)
{
	char *buff, *directory;

	directory = _getenv(info, "HOME=");
	if (!directory)
		return (NULL);

	buff = malloc(sizeof(char) * (_strlen(directory) + _strlen(HIST_FILE) + 2));
	if (!buff)
		return (NULL);

	buff[0] = '\0';
	_strcpy(buff, directory);
	_strcat(buff, "/");
	_strcat(buff, HIST_FILE);

	return (buff);
}

/**
 * write_history - Writes the command history to a file
 * @info: Pointer to the info_t struct
 *
 * Return: 1 on success, -1 on failure
 */
int write_history(info_t *info)
{
	ssize_t fileDes;
	char *file_name = get_history_file(info);
	list_t *currentNode = NULL;

	if (!file_name)
		return (-1);

	fileDes = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_name);
	if (fileDes == -1)
		return (-1);

	for (currentNode = info->history; currentNode; currentNode = currentNode->next)
	{
		_putsfd(currentNode->str, fileDes);
		_putfd('\n', fileDes);
	}
	_putfd(BUFF_FLUSH, fileDes);
	close(fileDes);

	return (1);
}

/**
 * read_history - Reads command history from a file
 * @info: Pointer to the info_t struct
 *
 * Return: Number of history entries read, or 0 on failure
 */
int read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fileDes, readLen, fileSize = 0;
	struct stat fileStat;
	char *buff = NULL, *file_name = get_history_file(info);

	if (!file_name)
		return (0);

	fileDes = open(file_name, O_RDONLY);
	free(file_name);
	if (fileDes == -1)
		return (0);

	if (!fstat(fileDes, &fileStat))
		fileSize = fileStat.st_size;

	if (fileSize < 2)
		return (0);

	buff = malloc(sizeof(char) * (fileSize + 1));
	if (!buff)
		return (0);

	readLen = read(fileDes, buff, fileSize);
	buff[fileSize] = 0;
	if (readLen <= 0)
		return (free(buff), 0);

	close(fileDes);

	for (i = 0; i < fileSize; i++)
	{
		if (buff[i] == '\n')
		{
			buff[i] = 0;
			build_history_list(info, buff + last, linecount++);
			last = i + 1;
		}
	}

	if (last != i)
		build_history_list(info, buff + last, linecount++);

	free(buff);
	info->histcount = linecount;

	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);

	return (info->histcount);
}

/**
 * build_history_list - Builds the command history list
 * @info: Pointer to the info_t struct
 * @buf: Buffer containing the command history entry
 * @linecount: Line count of the command history entry
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *command, int line_count)
{
	list_t *new_node = NULL;

	if (info->history)
		new_node = info->history;

	add_node_end(&new_node, command, line_count);

	if (!info->history)
		info->history = new_node;

	return (0);
}

/**
 * renumber_history - Renumber the command history list
 * @info: Pointer to the info_t struct
 *
 * Return: Number of history entries
 */
int renumber_history(info_t *info)
{
	list_t *current_node = info->history;
	int count = 0;

	while (current_node)
	{
		current_node->num = count++;
		current_node = current_node->next;
	}

	info->histcount = count;
	return (count);
}
