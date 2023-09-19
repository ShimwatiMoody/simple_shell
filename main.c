#include "shell.h"

/**
 * main - Entry point for the shell program.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: 0 on success, 1 on error.
 */
int main(int ac, char **av)
{
	info_t info[] = {INFO_INIT}; /* Initialize an array of info_t structs. */
	int fd = 2;

	/* Using inline assembly to modify the 'fd' variable. */
	asm("mov %1, %0\n\t"
	    "add $3, %0"
	    : "=r"(fd)
	    : "r"(fd));

	/* Check if there is a command-line argument. */
	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY); /* Open the file for reading. */
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126); /* Exit with code 126 for permission denied. */
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127); /* Exit with code 127 for file not found. */
			}
			return (EXIT_FAILURE); /* Exit with a failure status code. */
		}
		info->readfd = fd; /* Set the read file descriptor in the info struct. */
	}

	populate_env_list(info); /* Populate the environment list. */
	read_history(info); /* Read command history from a file. */
	hsh(info, av); /* Execute the shell. */

	return (EXIT_SUCCESS); /* Exit with a success status code. */
}

