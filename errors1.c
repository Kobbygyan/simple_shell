#include "shell.h"

/**
 * _erratoi - Converts a string to an integer
 * @s: The string to convert
 *
 * Return: The converted integer, or -1 if conversion fails
 */
int _erratoi(char *s)
{
	int idx = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++; /* TODO: why does this make main return 255? */

	for (idx = 0; s[idx] != '\0'; idx++)
	{
		if (s[idx] >= '0' && s[idx] <= '9')
		{
			result *= 10;
			result += (s[idx] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return (result);
}

/**
 * print_error - Prints an error message to stderr
 * @info: Pointer to the info struct
 * @error_str: Pointer to the error string
 *
 * Return: None
 */
void print_error(info_t *info, char *estr)
{
	_eputs(info->filename);
	_eputs(": ");
	print_integer(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_integer - Prints an integer to the specified file descriptor
 * @input: The integer to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters printed
 */
int print_integer(int input, int fd)
{
	int (*print_char)(char) = _putchar;
	int i, count = 0;
	unsigned int abs_val, current;

	if (fd == STDERR_FILENO)
		print_char = _eputchar;

	if (input < 0)
	{
		abs_val = -input;
		print_char('-');
		count++;
	}
	else
		abs_val = input;

	current = abs_val;

	for (i = 1000000000; i > 1; i /= 10)
	{
		if (abs_val / i)
		{
			print_char('0' + current / i);
			count++;
		}
		current %= i;
	}

	print_char('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - Converts a number to a string representation in 
 *                  the specified base
 * @num: The number to be converted
 * @base: The base for conversion
 * @flags: Flags for conversion options
 *
 * Return: Pointer to the converted number string
 */
char *convert_number(long int num, int base, int flags)
{
	static char *char_array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long int n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	char_array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" :
		"0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = char_array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * remove_comments - Removes comments from a string
 * @buffer: The string to remove comments from
 *
 * Return: None
 */
void remove_comments(char *buf)
{
	int idx;

	for (idx = 0; buf[idx] != '\0'; idx++)
	{
		if (buf[idx] == '#' && (!idx || buf[idx - 1] == ' '))
		{
			buf[idx] = '\0';
			break;
		}
	}
}
