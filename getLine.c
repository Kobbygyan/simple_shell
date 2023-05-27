#include "shell.h"

/**
 * input_buf - Read input from the user into a buffer
 * @info: Pointer to the info_t structure
 * @buf: Pointer to the input buffer
 * @len: Pointer to the length of the input buffer
 *
 * Return: The number of characters read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t bytesRead = 0;
	size_t newLen = 0;

	if (!(*len)) /* If nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);

#if USE_GETLINE
		bytesRead = getline(buf, &newLen, stdin);
#else
		bytesRead = _getline(info, buf, &newLen);
#endif

		if (bytesRead > 0)
		{
			if ((*buf)[bytesRead - 1] == '\n')
			{
				(*buf)[bytesRead - 1] = '\0'; /* Remove trailing newline */
				bytesRead--;
			}

			info->lineCountFlag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);

			/* if (_strchr(*buf, ';')) Is this a command chain? */
			{
				*len = bytesRead;
				info->cmmd_buff = buf;
			}
		}
	}

	return (bytesRead);
}

/**
 * get_input - Get input from user and process command chain
 * @info: Pointer to the info_t structure
 *
 * Return: Number of characters read
 */
ssize_t get_input(info_t *info)
{
	static char *chainBuf; /* The ';' command chain buffer */
	static size_t chainIndex, chainIterator, chainLen;
	ssize_t bytesRead = 0;
	char **bufPtr = &(info->arg), *chainPtr;

	_putchar(BUFF_FLUSH);
	bytesRead = input_buf(info, &chainBuf, &chainLen);

	if (bytesRead == -1) /* EOF */
		return (-1);

	if (chainLen) /* We have commands left in the chain buffer */
	{
		chainIterator = chainIndex; /* Init new iterator to current chain buffer position */
		chainPtr = chainBuf + chainIndex; /* Get pointer for return */

		check_chain(info, chainBuf, &chainIterator, chainIndex, chainLen);

		while (chainIterator < chainLen) /* Iterate to semicolon or end */
		{
			if (is_chain(info, chainBuf, &chainIterator))
				break;
			chainIterator++;
		}

		chainIndex = chainIterator + 1; /* Increment past nulled ';' */

		if (chainIndex >= chainLen) /* Reached end of buffer? */
		{
			chainIndex = chainLen = 0; /* Reset position and length */
			info->cmmd_buff_type = CMMD_NORM;
		}

		*bufPtr = chainPtr; /* Pass back pointer to current command position */
		return (_strlen(chainPtr)); /* Return length of current command */
	}

	*bufPtr = chainBuf; /* else not a chain, pass back buffer from input_buf() */
	return (bytesRead); /* Return length of buffer from input_buf() */
}

/**
 * read_buf - Read input from a file descriptor into a buffer
 * @info: Pointer to the info_t structure
 * @buf: Buffer to store the read input
 * @i: Pointer to the current index in the buffer
 *
 * Return: Number of bytes read
 */
ssize_t read_buf(info_t *info, char *buff, size_t *idx)
{
	ssize_t bytesRead = 0;

	if (*idx)
		return (0);

	bytesRead = read(info->readfd, buff, READ_BUFF_SIZE);

	if (bytesRead >= 0)
		*idx = bytesRead;

	return (bytesRead);
}

/**
 * _getline - Read a line of input from the file descriptor
 * @info: Pointer to the info_t structure
 * @ptr: Pointer to the buffer to store the line
 * @length: Pointer to the length of the buffer
 *
 * Return: Number of bytes read (-1 on failure)
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUFF_SIZE];
	static size_t buffIndex, buffLength;
	size_t k;
	ssize_t bytesRead = 0, buffStatus = 0;
	char *p = NULL, *newPtr = NULL, *c;

	p = *ptr;
	if (p && length)
		buffStatus = *length;
	if (buffIndex == buffLength)
		buffIndex = buffLength = 0;

	bytesRead = read_buf(info, buffer, &buffLength);
	if (bytesRead == -1 || (bytesRead == 0 && buffLength == 0))
		return (-1);

	c = _strchr(buffer + buffIndex, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : buffLength;
	newPtr = _realloc(p, buffStatus, buffStatus ? buffStatus + k : k + 1);
	if (!newPtr) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (buffStatus)
		_strncat(newPtr, buffer + buffIndex, k - buffIndex);
	else
		_strncpy(newPtr, buffer + buffIndex, k - buffIndex + 1);

	buffStatus += k - buffIndex;
	buffIndex = k;
	p = newPtr;

	if (length)
		*length = buffStatus;
	*ptr = p;
	return (buffStatus);
}

/**
 * sigintHandler - Signal handler for SIGINT
 * @sig_num: the signal number (unused)
 */
void sigintHandler(__attribute__((unused))int sigNum)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFF_FLUSH);
}
