#include "shell.h"

/**
 * bfree - Frees a pointer and sets it to NULL
 * @ptr: Pointer to the pointer to be freed
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
