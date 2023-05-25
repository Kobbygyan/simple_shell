#include "shell.h"

/**
 * input_buf - Read input into the buffer
 * @info: Pointer to the info_t struct
 * @buf: Pointer to the input buffer
 * @len: Pointer to the length of the input buffer
 *
 * Return: Number of characters read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t read_len = 0;
	size_t buffer_len = 0;

	if (!(*len)) /* If nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);

#if USE_GETLINE
		read_len = getline(buf, &buffer_len, stdin);
#else
		read_len = _getline(info, buf, &buffer_len);
#endif

		if (read_len > 0)
		{
			if ((*buf)[read_len - 1] == '\n')
			{
				(*buf)[read_len - 1] = '\0'; /* Remove trailing newline */
				read_len--;
			}

			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);

			/* if (_strchr(*buf, ';')) Is this a command chain? */
			{
				*len = read_len;
				info->cmd_buf = buf;
			}
		}
	}

	return (read_len);
}

/**
 * get_input - Get user input
 * @info: Pointer to the info_t struct
 *
 * Return: Number of characters read
 */
ssize_t get_input(info_t *info)
{
	static char *chain_buf; /* The ';' command chain buffer */
	static size_t buf_pos, chain_buf_len;
	ssize_t read_len = 0;
	char **arg_buf = &(info->arg), *buf_ptr;

	_putchar(BUF_FLUSH);
	read_len = input_buf(info, &chain_buf, &chain_buf_len);

	if (read_len == -1) /* EOF */
		return (-1);

	if (chain_buf_len) /* We have commands left in the chain buffer */
	{
		buf_pos = buf_pos; /* Init new iterator to current buffer position */
		buf_ptr = chain_buf + buf_pos; /* Get pointer for return */

		check_chain(info, chain_buf, &buf_pos, buf_pos, chain_buf_len);

		while (buf_pos < chain_buf_len) /* Iterate to semicolon or end */
		{
			if (is_chain(info, chain_buf, &buf_pos))
				break;
			buf_pos++;
		}

		buf_pos = buf_pos + 1; /* Increment past nulled ';' */

		if (buf_pos >= chain_buf_len) /* Reached end of buffer? */
		{
			buf_pos = chain_buf_len = 0; /* Reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*arg_buf = buf_ptr; /* Pass back pointer to current command position */
		return (_strlen(buf_ptr)); /* Return length of current command */
	}

	*arg_buf = chain_buf; /* Not a chain, pass back buffer from input_buf() */
	return (read_len); /* Return length of buffer from input_buf() */
}

/**
 * read_buf - Read input into a buffer
 * @info: Pointer to the info_t struct
 * @buf: Buffer to store the read data
 * @i: Pointer to the current position in the buffer
 *
 * Return: Number of bytes read
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t read_len = 0;

	if (*i)
		return (0);

	read_len = read(info->readfd, buf, READ_BUF_SIZE);

	if (read_len >= 0)
		*i = read_len;

	return (read_len);
}

/**
 * _getline - Read a line of input
 * @info: Pointer to the info_t struct
 * @ptr: Pointer to the buffer to store the line
 * @length: Pointer to the length of the buffer
 *
 * Return: Number of bytes read
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t read_len = 0, size = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		size = *length;
	if (i == len)
		i = len = 0;

	read_len = read_buf(info, buf, &len);
	if (read_len == -1 || (read_len == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, size, size ? size + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (size)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	size += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = size;
	*ptr = p;
	return (size);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
