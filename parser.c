#include "shell.h"

/**
 * parse_path - Splits the PATH string into individual directories.
 * @path_str: The PATH string to parse.
 *
 * Return: An array of strings representing the individual directories.
 */
char **parse_path(char *path_str)
{
	char **directories = NULL;
	int dir_count = 0;
	int i = 0, start = 0;

	if (!path_str)
		return (NULL);

	for (i = 0; path_str[i]; i++)
	{
		if (path_str[i] == ':')
		{
			path_str[i] = '\0';
			dir_count++;
		}
	}

	dir_count++; /* Account for the last directory */
	directories = malloc(sizeof(char *) * (dir_count + 1));
	if (!directories)
		return (NULL);

	for (i = 0, start = 0; i < dir_count; i++)
	{
		directories[i] = path_str + start;
		start += _strlen(path_str + start) + 1;
	}
	directories[i] = NULL;

	return (directories);
}

