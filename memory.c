#include "shell.h"

/**
 * bfree - Frees a pointer and sets it to NULL.
 * @ptr: Address of the pointer to free.
 *
 * This function frees the memory pointed to by 'ptr' and sets the pointer
 * itself to NULL to prevent a dangling pointer.
 *
 * Return: 1 if memory was freed, otherwise 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

