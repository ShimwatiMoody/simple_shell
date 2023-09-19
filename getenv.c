#include "shell.h"

/**
 * get_environ - Retrieves a copy of the environment variables as an array.
 * @info: Pointer to a structure containing potential arguments.
 *
 * Return: A string array containing environment variables.
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Removes an environment variable.
 * @info: Pointer to a structure containing potential arguments.
 * @var: The string representing the environment variable to remove.
 *
 * Return: 1 if the variable is deleted, 0 otherwise.
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *current_node = info->env;
	size_t index = 0;
	char *p;

	if (!current_node || !var)
		return (0);

	while (current_node)
	{
		p = starts_with(current_node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), index);
			index = 0;
			current_node = info->env;
			continue;
		}
		current_node = current_node->next;
		index++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initializes a new environment variable
 * or modifies an existing one.
 * @info: Pointer to a structure containing potential arguments.
 * @var: The string representing the environment variable name.
 * @value: The string representing the environment variable value.
 *
 * Return: Always 0.
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *current_node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	current_node = info->env;
	while (current_node)
	{
		p = starts_with(current_node->str, var);
		if (p && *p == '=')
		{
			free(current_node->str);
			current_node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		current_node = current_node->next;
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

