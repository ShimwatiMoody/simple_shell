#include "shell.h"

/**
 * add_node_at_beginning - Adds a new node to the beginning of the linked list.
 * @head: Address of the pointer to the head node.
 * @data: Data to be stored in the new node.
 * @index: Index of the new node.
 *
 * Return: Address of the newly created node or NULL on failure.
 */
list_t *add_node_at_beginning(list_t **head, const char *data, int index)
{
	list_t *new_node;

	if (!head)
		return (NULL);

	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	memset(new_node, 0, sizeof(list_t));
	new_node->index = index;

	if (data)
	{
		new_node->str = _strdup(data);
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
 * add_node_at_end - Adds a new node to the end of the linked list.
 * @head: Address of the pointer to the head node.
 * @data: Data to be stored in the new node.
 * @index: Index of the new node.
 *
 * Return: Address of the newly created node or NULL on failure.
 */
list_t *add_node_at_end(list_t **head, const char *data, int index)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	memset(new_node, 0, sizeof(list_t));
	new_node->index = index;

	if (data)
	{
		new_node->str = _strdup(data);
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
 * print_string_list - Prints the strings
 * (str field) of nodes in the linked list.
 * @head: Pointer to the first node.
 *
 * Return: Number of nodes in the list.
 */
size_t print_string_list(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}

	return (count);
}

/**
 * delete_node_at_index - Deletes a node at a specified index
 * in the linked list.
 * @head: Address of the pointer to the head node.
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}

	return (0);
}

/**
 * free_string_list - Frees all nodes in the linked list
 * and sets the head pointer to NULL.
 * @head_ptr: Address of the pointer to the head node.
 *
 * Return: void.
 */
void free_string_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}

	*head_ptr = NULL;
}

