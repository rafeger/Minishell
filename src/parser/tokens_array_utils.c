#include "../../minishell.h"

/*
 * Initializes new token array structure for pipe parsing.
 * Sets up count and basic structures.
 * Returns NULL on allocation failure.
*/
t_ta	*init_new_ta(t_ta *ta, int index)
{
	t_ta	*new_ta;

	new_ta = tokenarray_init();
	if (!new_ta)
		return (NULL);
	free(new_ta->tokens);
	free(new_ta->quoted);
	new_ta->count = ta->count - index - 1;
	return (new_ta);
}

/*
 * Creates subset of tokens for pipe command.
 * Allocates and copies relevant tokens from original array.
 * Returns NULL on allocation failure.
*/
char	**create_sub_tokens(t_ta *ta, int index, t_ta *new_ta)
{
	char	**sub_tokens;
	int		i;

	sub_tokens = malloc(sizeof(char *) * (new_ta->count + 1));
	if (!sub_tokens)
		return (NULL);
	i = 0;
	while (i < new_ta->count)
	{
		sub_tokens[i] = ft_strdup(ta->tokens[index + 1 + i]);
		if (!sub_tokens[i])
			return (NULL);
		i++;
	}
	sub_tokens[i] = NULL;
	return (sub_tokens);
}

/*
 * Initializes quote tracking array for pipe command.
 * Copies quote status from original token array.
 * Returns 0 on failure, 1 on success.
*/
int	init_quoted_array(t_ta *new_ta, t_ta *ta, int index)
{
	int	i;

	new_ta->quoted = malloc(sizeof(int) * new_ta->count);
	if (!new_ta->quoted)
		return (0);
	i = 0;
	while (i < new_ta->count)
	{
		new_ta->quoted[i] = ta->quoted[index + 1 + i];
		i++;
	}
	return (1);
}
