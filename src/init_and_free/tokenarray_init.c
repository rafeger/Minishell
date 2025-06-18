#include "../../include/minishell.h"

/*
 * Allocates and initializes quoted array for token tracking.
 * Sets initial memory to zero using ft_memset.
 * Used internally by tokenarray initialization.
*/
static int	*alloc_quoted(int capacity)
{
	int	*quoted;

	quoted = malloc(sizeof(int) * capacity);
	if (!quoted)
		return (NULL);
	ft_memset(quoted, 0, sizeof(int) * capacity);
	return (quoted);
}

/*
 * Initializes secondary token array parameters.
 * Sets counters, flags, and state variables to their initial values.
 * Part of the two-step initialization process for token arrays.
*/
void	tokenarray_init_second(t_ta *ta)
{
	ta->second_quote = 0;
	ta->count = 0;
	ta->capacity = 10;
	ta->token = NULL;
	ta->tokensize = 0;
	ta->tokenindex = 0;
	ta->inquotes = 0;
	ta->quotechar = '\0';
	ta->trailing_space = 0;
}

/*
 * Main token array initialization function.
 * Allocates and sets up all necessary memory for tokenization:
 * - Main token array structure
 * - Token storage array
 * - Quote tracking array
 * Includes error handleing for allocation failures.
 * Returns NULL if any allocation fails, otherwise returns
 * initialized structure.
*/
t_ta	*tokenarray_init(void)
{
	t_ta	*ta;

	ta = (t_ta *)malloc(sizeof(t_ta));
	if (!ta)
		return (NULL);
	ft_memset(ta, 0, sizeof(t_ta));
	ta->tokens = malloc(sizeof(char *) * 10);
	if (!ta->tokens)
	{
		free(ta);
		return (NULL);
	}
	ta->quoted = alloc_quoted(10);
	if (!ta->quoted)
	{
		free(ta->tokens);
		free(ta);
		return (NULL);
	}
	tokenarray_init_second(ta);
	return (ta);
}