#include "shell.h"

/**
 * exit_shell - Terminate the shell.
 * @info: Pointer to the structure storing potential arguments.
 *
 * Return: Exits the shell with the specified exit status (0) if
 * info->argv[0] is not equal to "exit."
 */
int exit_shell(info_t *info)
{
	int exit_status;

	if (info->argv[1]) /* Check if an exit argument is provided */
	{
		exit_status = convert_to_integer(info->argv[1]);
		if (exit_status == -1)
		{
			info->status = 2;
			print_error(info, "Invalid number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = convert_to_integer(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * change_directory - Change the current working directory of the process.
 * @info: Pointer to the structure holding potential arguments.
 *
 * Return: Always 0.
 */
int change_directory(info_t *info)
{
	char *current_directory, *new_directory, buffer[1024];
	int chdir_return;

	current_directory = get_current_directory(buffer, 1024);
	if (!current_directory)
		_puts("TODO: Handle getcwd failure message here\n");

	if (!info->argv[1])
	{
		new_directory = get_environment_variable(info, "HOME=");
		if (!new_directory)
			chdir_return = change_dir((new_directory = get_environment_variable(info, "PWD="))
				       	? new_directory : "/");
		else
			chdir_return = change_dir(new_directory);
	}
	else if (are_strings_equal(info->argv[1], "-") == 0)
	{
		if (!get_environment_variable(info, "OLDPWD="))
		{
			_puts(current_directory);
			_putchar('\n');
			return (1);
		}
		_puts(get_environment_variable(info, "OLDPWD="));
		_putchar('\n');
		chdir_return = change_dir((new_directory = get_environment_variable(info, "OLDPWD="))
				? new_directory : "/");
	}
	else
		chdir_return = change_dir(info->argv[1]);

	if (chdir_return == -1)
	{
		print_error(info, "Unable to change directory to ");
		_eputs(info->argv[1]);
		_eputchar('\n');
	}
	else
	{
		set_environment_variable(info, "OLDPWD", get_environment_variable(info, "PWD="));
		set_environment_variable(info, "PWD", get_current_directory(buffer, 1024));
	}
	return (0);
}

/**
 * display_help - Display information about available commands.
 * @info: Pointer to the structure containing potential arguments.
 *
 * Return: Always 0.
 */
int display_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("The 'help' command is functional, but the function has not been implemented yet.\n");
	if (0)
		_puts(*arg_array); /* Temporary unused variable workaround */
	return (0);
}

