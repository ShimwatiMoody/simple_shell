#include "shell.h"

/**
 * list_length - Calculates the length of a linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: Size of the list.
 */
size_t list_length(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		head = head->next;
		count++;
	}
	return (count);
}

/**
 * list_to_strings - Converts a linked list of strings to an array of strings.
 * @head: Pointer to the first node of the list.
 *
 * Return: Array of strings.
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t list_size = list_length(head);
	size_t i;
	char **string_array;
	char *string;

	if (!head || !list_size)
		return (NULL);

	string_array = malloc(sizeof(char *) * (list_size + 1));
	if (!string_array)
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		string = malloc(_strlen(node->str) + 1);
		if (!string)
		{
			for (size_t j = 0; j < i; j++)
				free(string_array[j]);
			free(string_array);
			return (NULL);
		}

		string = _strcpy(string, node->str);
		string_array[i] = string;
	}
	string_array[i] = NULL;
	return (string_array);
}

/**
 * print_list - Prints all elements of a linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: Size of the list.
 */
size_t print_list(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(convert_number(head->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}
	return (count);
}

/**
 * find_node_with_prefix - Finds a node whose string
 * starts with a given prefix.
 * @head: Pointer to the first node of the list.
 * @prefix: Prefix to match.
 * @next_char: The next character after the prefix to match.
 *
 * Return: Pointer to the matching node or NULL if not found.
 */
list_t *find_node_with_prefix(list_t *head, char *prefix, char next_char)
{
	char *prefix_start = NULL;

	while (head)
	{
		prefix_start = starts_with(head->str, prefix);
		if (prefix_start && ((next_char == -1) || (*prefix_start == next_char)))
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * get_node_index - Gets the index of a node within a linked list.
 * @head: Pointer to the first node of the list.
 * @node: Pointer to the target node.
 *
 * Return: Index of the node or -1 if not found.
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}

