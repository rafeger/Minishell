#include "../../include/minishell.h"

t_ta	*init_new_ta(t_ta *t_array, int index)
{
	t_ta	*new_ta;

	new_ta = tokenarray_init();
	if (!new_ta)
		return (NULL);
	free(new_ta->tokens);
	free(new_ta->quoted);
	new_ta->t_tot = t_array->t_tot - index - 1;
	return (new_ta);
}

char	**create_sub_tokens(t_ta *t_array, int index, t_ta *new_ta)
{
	char	**sub_tokens;
	int		i;

	sub_tokens = malloc(sizeof(char *) * (new_ta->t_tot + 1));
	if (!sub_tokens)
		return (NULL);
	i = 0;
	while (i < new_ta->t_tot)
	{
		sub_tokens[i] = ft_strdup(t_array->tokens[index + 1 + i]);
		if (!sub_tokens[i])
			return (NULL);
		i++;
	}
	sub_tokens[i] = NULL;
	return (sub_tokens);
}

int	init_quoted_array(t_ta *new_ta, t_ta *t_array, int index)
{
	int	i;

	new_ta->quoted = malloc(sizeof(int) * new_ta->t_tot);
	if (!new_ta->quoted)
		return (0);
	i = 0;
	while (i < new_ta->t_tot)
	{
		new_ta->quoted[i] = t_array->quoted[index + 1 + i];
		i++;
	}
	return (1);
}
