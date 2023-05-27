#include "shell.h"

/**
 * bfree - Frees a block of memory and sets the pointer to NULL
 * @ptr: Pointer to the block of memory to be freed
 *
 * Return: 1 if the pointer was freed successfully, otherwise 0
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}

	return (0);
}
