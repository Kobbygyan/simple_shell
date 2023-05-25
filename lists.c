#include "shell.h"

/**
 * add_node - Add a new node at the beginning of a linked list
 * @head: Pointer to a pointer to the head of the list
 * @str: String to be stored in the new node
 * @num: Number to be stored in the new node
 *
 * Return: Pointer to the newly added node, or NULL on failure
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_node;

	if (!head)
		return (NULL);

	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}

	new_node->next = *head;
	*head = new_node;

	return (new_node);
}

/**
 * add_node_end - Add a new node at the end of a linked list
 * @head: Pointer to a pointer to the head of the list
 * @str: String to be stored in the new node
 * @num: Number to be stored in the new node
 *
 * Return: Pointer to the newly added node, or NULL on failure
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}

	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * print_list_str - Print the strings stored in a linked list
 * @h: Pointer to the head of the list
 *
 * Return: Number of nodes in the list
 */
size_t print_list_str(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}

	return (i);
}

/**
 * delete_node_at_index - Deletes a node at a given index in a linked list
 * @head: Pointer to the head of the list
 * @index: Index of the node to delete
 *
 * Return: 1 if successful, 0 otherwise
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *current, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (index == 0)
	{
		current = *head;
		*head = (*head)->next;
		free(current->str);
		free(current);
		return (1);
	}

	current = *head;
	while (current)
	{
		if (i == index)
		{
			prev_node->next = current->next;
			free(current->str);
			free(current);
			return (1);
		}
		i++;
		prev_node = current;
		current = current->next;
	}

	return (0);
}

/**
 * free_list - Frees all nodes of a linked list
 * @head_ptr: Address of pointer to head node
 *
 * Return: void
 */
void free_list(list_t **head_ptr)
{
	list_t *current, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;
	current = head;
	while (current)
	{
		next_node = current->next;
		free(current->str);
		free(current);
		current = next_node;
	}

	*head_ptr = NULL;
}
