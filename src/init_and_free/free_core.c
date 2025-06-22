#include "../../include/minishell.h"

/*
 * Safely frees a single pointer and sets it to NULL.
 * Basic memory management utility function.
*/
void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

/*
 * Frees an array of environment strings.
 * Handles each string in array and the array itself.
*/
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

/*
 * Comprehensive cleanup of tken array structure.
 * Frees all allocated components:
 * - Individual tokens
 * - Token array
 * - Quote tracking array
 * - Temporary token buffer
 * - Main structure
*/
void	free_tokenarray(t_ta *ta)
{
	int	i;

	if (!ta)
		return ;
	if (ta->tokens)
	{
		i = 0;
		while (i < ta->t_tot)
		{
			if (ta->tokens[i])
				free(ta->tokens[i]);
			i++;
		}
		free(ta->tokens);
	}
	if (ta->quoted)
		free(ta->quoted);
	if (ta->token)
		free(ta->token);
	free(ta);
}

/*
 * Frees a linked list of redirections.
 * Clens up file names and redirect structures.
*/
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
