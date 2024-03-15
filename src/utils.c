#include "minishell.h"

void	ft_bzero(void *memory, size_t size)
{
	while (size--)
		*(unsigned char *)memory++ = 0;
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*memory;

	if (count && size && count > (UINT_MAX / size))
		return (NULL);
	memory = malloc(count * size);
	if (!memory)
		return (NULL);
	ft_bzero(memory, count * size);
	return (memory);
}

// This is the cleanest way to malloc,
// by adding ft_bzero() and a clean exit()
// with an error message
void	*calloc_or_exit(size_t size, int count)
{
	void	*result;

	result = ft_calloc(size, count);
	if (!result)
    {
        printf("ERROR - calloc_or_exit\n"); // to output in STDERR
		exit(1);
    }
	return (result);
}

// This is a singleton. The concept is to create an object
// once and then be able to access it any time while not
// using global variable thanks to a stored pointer.
t_minishell	*get_minishell(t_minishell *minishell)
{
	static t_minishell	*pointer_to_minishell = NULL;

	if (!pointer_to_minishell && minishell)
		pointer_to_minishell = minishell;
	return (pointer_to_minishell);
}