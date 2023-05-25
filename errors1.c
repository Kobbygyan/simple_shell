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
	unsigned long int res = 0;

	if (*s == '+')
		s++; /* TODO: why does this make main return 255? */

	for (idx = 0; s[idx] != '\0'; idx++)
	{
		if (s[idx] >= '0' && s[idx] <= '9')
		{
			res *= 10;
			res += (s[idx] - '0');
			if (res > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return (res);
}

/**
 * print_error - Prints an error message to stderr
 * @info: Pointer to the info struct
 * @error_str: Pointer to the error string
 *
 * Return: None
 */
void print_error(info_t *info, char *error_str)
{
	_eputs(info->fname);
	_eputs(": ");
	print_d(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(error_str);
}

/**
 * print_d - Prints an integer to a file descriptor
 * @input: The integer to print
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters printed
 */
int print_d(int input, int fd)
{
	int (*output_char)(char) = _putchar;
	int i, count = 0;
	unsigned int absolute_value, current;

	if (fd == STDERR_FILENO)
		output_char = _eputchar;

	if (input < 0)
	{
		absolute_value = -input;
		output_char('-');
		count++;
	}
	else
		absolute_value = input;

	current = absolute_value;

	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute_value / i)
		{
			output_char('0' + current / i);
			count++;
		}
		current %= i;
	}

	output_char('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - Converts a number to a specified base and returns it as 
 *                  a string
 * @num: The number to convert
 * @base: The base to convert the number to
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
void remove_comments(char *buffer)
{
	int idx;

	for (idx = 0; buffer[idx] != '\0'; idx++)
	{
		if (buffer[idx] == '#' && (!idx || buffer[idx - 1] == ' '))
		{
			buffer[idx] = '\0';
			break;
		}
	}
}
