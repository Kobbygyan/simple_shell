#include "shell.h"

/**
 * list_len - Counts the number of elements in a linked list
 * @h: Pointer to the head of the list
 *
 * Return: The number of elements in the list
 */
size_t list_len(const list_t *h)
{
	size_t count = 0;

	while (h)
	{
		h = h->next;
		count++;
	}

	return (count);
}

/**
 * list_to_strings - Converts a linked list of strings into an array of strings
 * @head: Pointer to the head of the list
 *
 * Return: Pointer to the array of strings
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t count = list_len(head);
	size_t i, j;
	char **strs;
	char *str;

	if (!head || !count)
		return (NULL);

	strs = malloc(sizeof(char *) * (count + 1));
	if (!strs)
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}

	strs[i] = NULL;
	return (strs);
}

/**
 * print_list - Prints the contents of a linked list
 * @h: Pointer to the head of the list
 *
 * Return: Number of nodes in the list
 */
size_t print_list(const list_t *h)
{
	size_t count = 0;

	while (h)
	{
		_puts(convert_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		count++;
	}

	return (count);
}

/**
 * node_starts_with - Find a node in a linked list with a string that starts
 *                    with a prefix
 * @node: Pointer to the head of the list
 * @prefix: Prefix to search for
 * @c: Optional character check
 *
 * Return: Pointer to the found node, or NULL if not found
 */
list_t *node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}

	return (NULL);
}

/**
 * get_node_index - Get the index of a node in a linked list
 * @head: Pointer to the head of the list
 * @node: Pointer to the node to find the index of
 *
 * Return: The index of the node, or -1 if the node is not found
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}

	return (-1);
}
