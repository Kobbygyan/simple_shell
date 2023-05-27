#include "shell.h"

/**
 * interactive - Determines if the shell is in interactive mode.
 * @info: Pointer to the struct containing shell information.
 *
 * Return: 1 if the shell is in interactive mode, 0 otherwise.
 */
int interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delim - Checks if a character is a delimiter.
 * @c: The character to check.
 * @delim: The string of delimiters.
 *
 * Return: 1 if the character is a delimiter, 0 otherwise.
 */
int is_delim(char c, char *delimiters)
{
	while (*delimiters)
	{
		if (*delimiters++ == c)
		{
			return (1);
		}
	}
	return (0);
}

/**
 * _isalpha - Checks if a character is an alphabetic character.
 * @ch: The character to check.
 *
 * Return: 1 if the character is alphabetic, 0 otherwise.
 */
int _isalpha(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	{
		return (1);
	}
	{
		return (0);
	}
}

/**
 * _atoi - Converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: 0 if there are no numbers in the string, converted number otherwise.
 */
int _atoi(char *str)
{
	int idx, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (idx = 0; str[idx] != '\0' && flag != 2; idx++)
	{
		if (str[idx] == '-')
		{
			sign *= -1;
		}
		if (str[idx] >= '0' && str[idx] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (str[idx] - '0');
		} else if (flag == 1)
		{
			flag = 2;
		}
	}
	if (sign == -1)
	{
		output = -result;
	} else
	{
		output = result;
	}
	return (output);
}
