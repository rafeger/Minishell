#include "../../include/minishell.h"

static int	*alloc_quoted(int cap)
{
	int	*quoted;

	quoted = malloc(sizeof(int) * cap);
	if (!quoted)
		return (NULL);
	ft_memset(quoted, 0, sizeof(int) * cap);
	return (quoted);
}

void	tokenarray_init_second(t_ta *t_array)
{	
	t_array->tokenindex = 0;
	t_array->in_q = 0;
	t_array->quotechar = '\0';
	t_array->token = NULL;
	t_array->tokensize = 0;
	t_array->trailing_space = 0;
	t_array->second_quote = 0;
	t_array->t_tot = 0;
	t_array->cap = 10;
}

t_ta	*tokenarray_init(void)
{
	t_ta	*t_array;

	t_array = (t_ta *)malloc(sizeof(t_ta));
	if (!t_array)
		return (NULL);
	ft_memset(t_array, 0, sizeof(t_ta));
	t_array->tokens = malloc(sizeof(char *) * 10);
	if (!t_array->tokens)
	{
		free(t_array);
		return (NULL);
	}
	t_array->quoted = alloc_quoted(10);
	if (!t_array->quoted)
	{
		free(t_array->tokens);
		free(t_array);
		return (NULL);
	}
	tokenarray_init_second(t_array);
	return (t_array);
}