#include "shell.h"

/**
 * _memset - Fill memory with a constant byte
 * @s: Pointer to the memory area to be filled
 * @b: The byte to be set
 * @n: Number of bytes to be set
 *
 * Return: Pointer to the filled memory area (s)
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;

	return (s);
}

/**
 * ffree - Free a double pointer and its contents
 * @pp: Double pointer to be freed
 *
 * Return: None
 */
void ffree(char **pp)
{
	char **a = pp;

	if (!pp)
		return;

	while (*pp)
		free(*pp++);

	free(a);
}

/**
 * _realloc - Reallocate memory block with new size
 * @ptr: Pointer to the memory block to be reallocated
 * @old_size: Old size of the memory block
 * @new_size: New size of the memory block
 *
 * Return: Pointer to the reallocated memory block, or NULL on failure
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}

	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];

	free(ptr);
	return (p);
}
