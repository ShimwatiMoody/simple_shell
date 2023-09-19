#include "shell.h"

/**
 * str_to_int - Converts a string to an integer.
 * @str: The string to be converted.
 *
 * Return: The converted integer or -1 on error.
 */
int str_to_int(char *str)
{
	int index = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++; /* Skip leading '+' */
	for (index = 0; str[index] != '\0'; index++)
	{
		if (str[index] >= '0' && str[index] <= '9')
		{
			result *= 10;
			result += (str[index] - '0');
			if (result > INT_MAX)
				return (-1); /* Integer overflow */
		}
		else
			return (-1); /* Invalid character in string */
	}
	return (result);
}

/**
 * print_error_msg - Prints an error message.
 * @info: Information about the command.
 * @error_msg: The specific error message.
 *
 * Return: Nothing.
 */
void print_error_msg(info_t *info, char *error_msg)
{
	_eputs(info->prog_name);
	_eputs(": ");
	print_line_num(info->line_num, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->args[0]);
	_eputs(": ");
	_eputs(error_msg);
}

/**
 * print_line_num - Prints a decimal line number.
 * @line_num: The line number to print.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */
int print_line_num(int line_num, int fd)
{
	int (*putchar_func)(char) = _putchar;
	int i, count = 0;
	unsigned int abs_line_num, current;

	if (fd == STDERR_FILENO)
		putchar_func = _eputchar;
	if (line_num < 0)
	{
		abs_line_num = -line_num;
		putchar_func('-');
		count++;
	}
	else
		abs_line_num = line_num;
	current = abs_line_num;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (abs_line_num / i)
		{
			putchar_func('0' + current / i);
			count++;
		}
		current %= i;
	}
	putchar_func('0' + current);
	count++;

	return (count);
}

/**
 * num_to_str - Converts a number to a string.
 * @num: The number to convert.
 * @base: The base for the conversion.
 * @flags: Conversion flags.
 *
 * Return: The resulting string.
 */
char *num_to_str(long int num, int base, int flags)
{
	static char *hex_lower = "0123456789abcdef";
	static char *hex_upper = "0123456789ABCDEF";
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = (num < 0 && !(flags & UNSIGNED_CONV)) ? -num : num;

	if (num < 0 && !(flags & UNSIGNED_CONV))
		sign = '-';

	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = (flags & LOWERCASE_CONV) ?
			hex_lower[n % base] : hex_upper[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * remove_comments_from_str - Replaces the first '#' character with '\0'.
 * @str: The string to modify.
 *
 * Return: Always 0.
 */
void remove_comments_from_str(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == '#' && (!i || str[i - 1] == ' '))
		{
			str[i] = '\0';
			break;
		}
}

