#include "shell.h"

/**
 * _eputs - Prints a string to the standard error stream
 * @str: The string to be printed
 *
 * Return: None
 */
void _eputs(char *str)
{
	int index = 0;

	if (!str)
		return;

	while (str[index] != '\0')
	{
		_eputchar(str[index]);
		index++;
	}
}

/**
 * _eputchar - Writes a character to the standard error stream
 * @c: The character to be printed
 *
 * Return: On success, returns 1.
 *         On error, returns -1 and sets errno appropriately.
 */
int _eputchar(char c)
{
	static int index;
	static char buf[WRITE_BUFF_SIZE];

	if (c == BUFF_FLUSH || i >= WRITE_BUFF_SIZE)
	{
		write(2, buf, index);
		index = 0;
	}

	if (c != BUFF_FLUSH)
		buf[index++] = c;

	return (1);
}

/**
 * _putfd - Writes a character to the specified file descriptor
 * @c: The character to be written
 * @fd: The file descriptor to write to
 *
 * Return: On success, returns 1.
 *         On error, returns -1 and sets errno appropriately.
 */
int _putfd(char c, int fd)
{
	static int index;
	static char buf[WRITE_BUFF_SIZE];

	if (c == BUFF_FLUSH || index >= WRITE_BUFF_SIZE)
	{
		write(fd, buf, index);
		index = 0;
	}
	if (c != BUFF_FLUSH)
		buf[index++] = c;
	return (1);
}

/**
 * _putsfd - Prints a string to the specified file descriptor
 * @str: The string to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters successfully written.
 */
int _putsfd(char *str, int fd)
{
	int index = 0;

	if (!str)
		return (0);

	while (*str)
	{
		index += _putfd(*str++, fd);
	}

	return (index);
}
