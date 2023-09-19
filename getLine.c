#include "shell.h"

/**
 * input_buffer - Buffer chained commands for processing.
 * @info: Parameter struct.
 * @buf: Address of buffer.
 * @len: Address of length variable.
 *
 * Return: Bytes read.
 */
ssize_t input_buffer(info_t *info, char **buf, size_t *len)
{
	ssize_t bytes_read = 0;
	size_t current_len = 0;

	if (!*len) /* Check if the buffer is empty */
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigint_handler);
#if USE_GETLINE
		bytes_read = getline(buf, &current_len, stdin);
#else
		bytes_read = custom_getline(info, buf, &current_len);
#endif
		if (bytes_read > 0)
		{
			if ((*buf)[bytes_read - 1] == '\n')
			{
				(*buf)[bytes_read - 1] = '\0'; /* Remove trailing newline */
				bytes_read--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
		}
	}
	return (bytes_read);
}

/**
 * get_input - Get a line without the newline character.
 * @info: Parameter struct.
 *
 * Return: Bytes read.
 */
ssize_t get_input(info_t *info)
{
	static char *chain_buf; /* The ';' command chain buffer */
	static size_t chain_i, chain_j, chain_len;
	ssize_t bytes_read = 0;
	char **buf_p = &(info->arg), *current_ptr;

	_putchar(BUF_FLUSH);
	bytes_read = input_buffer(info, &chain_buf, &chain_len);
	if (bytes_read == -1) /* End of file (EOF) */
		return (-1);
	if (chain_len) /* Check if there are commands left in the chain buffer */
	{
		chain_j = chain_i; /* Initialize a new iterator to the current buffer position */
		current_ptr = chain_buf + chain_i; /* Get a pointer for return */

		check_chain(info, chain_buf, &chain_j, chain_i, chain_len);
		while (chain_j < chain_len) /* Iterate to semicolon or end */
		{
			if (is_chain(info, chain_buf, &chain_j))
				break;
			chain_j++;
		}

		chain_i = chain_j + 1; /* Increment past nulled ';'' */
		if (chain_i >= chain_len) /* Reached the end of the buffer? */
		{
			chain_i = chain_len = 0; /* Reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = current_ptr; /* Pass back a pointer to the current command position */
		return (_strlen(current_ptr)); /* Return the length of the current command */
	}

	*buf_p = chain_buf; /* Otherwise, not a chain, pass back the buffer from custom_getline() */
	return (bytes_read); /* Return the length of the buffer from custom_getline() */
}

/**
 * read_buffer - Reads data into a buffer.
 * @info: Parameter struct.
 * @buf: Buffer.
 * @i: Size.
 *
 * Return: Bytes read.
 */
ssize_t read_buffer(info_t *info, char *buf, size_t *i)
{
	ssize_t bytes_read = 0;

	if (*i)
		return (0);
	bytes_read = read(info->readfd, buf, READ_BUF_SIZE);
	if (bytes_read >= 0)
		*i = bytes_read;
	return (bytes_read);
}

/**
 * custom_getline - Gets the next line of input from STDIN.
 * @info: Parameter struct.
 * @ptr: Address of pointer to buffer, preallocated or NULL.
 * @length: Size of preallocated ptr buffer if not NULL.
 *
 * Return: Bytes read.
 */
int custom_getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t buf_i, buf_len;
	size_t current_len;
	ssize_t bytes_read = 0;
	char *current_ptr = NULL, *new_ptr = NULL, *newline_pos;

	current_ptr = *ptr;
	if (current_ptr && length)
		current_len = *length;
	if (buf_i == buf_len)
		buf_i = buf_len = 0;

	bytes_read = read_buffer(info, buf, &buf_len);
	if (bytes_read == -1 || (bytes_read == 0 && buf_len == 0))
		return (-1);

	newline_pos = _strchr(buf + buf_i, '\n');
	current_len = newline_pos ? 1 + (unsigned int)(newline_pos - buf) : buf_len;
	new_ptr = _realloc(current_ptr, current_len, current_len ? current_len + current_len : current_len + 1);
	if (!new_ptr) /* MALLOC FAILURE! */
		return (current_ptr ? free(current_ptr), -1 : -1);

	if (current_len)
		_strncat(new_ptr, buf + buf_i, current_len - buf_i);
	else
		_strncpy(new_ptr, buf + buf_i, current_len - buf_i + 1);

	current_len += current_len - buf_i;
	buf_i = current_len;
	current_ptr = new_ptr;

	if (length)
		*length = current_len;
	*ptr = current_ptr;
	return (current_len);
}

/**
 * sigint_handler - Blocks Ctrl-C signal.
 * @sig_num: Signal number.
 *
 * Return: void.
 */
void sigint_handler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

