#include "../../include/minishell.h"

void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	free_tokenarray(t_ta *t_array)
{
	int	i;

	if (!t_array)
		return ;
	if (t_array->tokens)
	{
		i = 0;
		while (i < t_array->t_tot)
		{
			if (t_array->tokens[i])
				free(t_array->tokens[i]);
			i++;
		}
		free(t_array->tokens);
	}
	if (t_array->quoted)
		free(t_array->quoted);
	if (t_array->token)
		free(t_array->token);
	free(t_array);
}

void	free_redirects(t_redirect *redirect)
{
	t_redirect	*temp;

	while (redirect)
	{
		temp = redirect->next;
		free(redirect->file);
		free(redirect);
		redirect = temp;
	}
}
