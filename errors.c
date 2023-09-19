#include "shell.h"

/**
 * print_error_message - Print an error message to standard error.
 * @message: The error message to be printed.
 *
 * Return: Nothing.
 */
void print_error_message(char *message)
{
	int index = 0;

	if (!message)
		return;
	while (message[index] != '\0')
	{
		write_error_char(message[index]);
		index++;
	}
}

/**
 * write_error_char - Write a character to standard error.
 * @c: The character to print.
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_error_char(char c)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buffer, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * write_char_to_fd - Write a character to the specified file descriptor.
 * @c: The character to print.
 * @fd: The file descriptor to write to.
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_char_to_fd(char c, int fd)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buffer, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * print_string_to_fd - Print a string to the specified file descriptor.
 * @str: The string to be printed.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */
int print_string_to_fd(char *str, int fd)
{
	int index = 0;

	if (!str)
		return (0);
	while (str[index])
	{
		index += write_char_to_fd(str[index], fd);
	}
	return (index);
}

