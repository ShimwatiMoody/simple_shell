#include "shell.h"

/**
 * display_history - Display the command history with line numbers.
 * @info: Structure containing relevant information.
 *
 * Return: Always 0.
 */
int display_history(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - Remove an alias from the list.
 * @info: Pointer to the structure holding relevant information.
 * @alias: The alias to unset.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *alias)
{
	char *equal_sign;
	char temp;

	equal_sign = _strchr(alias, '=');
	if (!equal_sign)
		return (1);

	temp = *equal_sign;
	*equal_sign = '\0';
	int ret = delete_node_at_index(&(info->alias),
				    get_node_index(info->alias,
						   node_starts_with(info->alias, alias, -1)));
	*equal_sign = temp;

	return (ret);
}

/**
 * set_alias - Set an alias.
 * @info: Pointer to the structure holding relevant information.
 * @alias: The alias string to set.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *alias)
{
	char *equal_sign;

	equal_sign = _strchr(alias, '=');
	if (!equal_sign)
		return (1);

	if (!*++equal_sign)
		return (unset_alias(info, alias));

	unset_alias(info, alias);
	return (add_node_end(&(info->alias), alias, 0) == NULL);
}

/**
 * print_single_alias - Print a single alias.
 * @node: The alias node to print.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_single_alias(list_t *node)
{
	char *equal_sign;

	if (node)
	{
		equal_sign = _strchr(node->str, '=');
		for (char *alias = node->str; alias <= equal_sign; alias++)
			_putchar(*alias);
		_putchar('\'');
		_puts(equal_sign + 1);
		_puts("'\n");
		return (0);
	}

	return (1);
}

/**
 * manage_alias - Manage aliases (set, unset, or display).
 * @info: Pointer to the structure holding relevant information.
 *
 * Return: Always 0.
 */
int manage_alias(info_t *info)
{
	int i = 0;
	char *equal_sign;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_single_alias(node);
			node = node->next;
		}
		return (0);
	}

	for (i = 1; info->argv[i]; i++)
	{
		equal_sign = _strchr(info->argv[i], '=');
		if (equal_sign)
			set_alias(info, info->argv[i]);
		else
			print_single_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}

