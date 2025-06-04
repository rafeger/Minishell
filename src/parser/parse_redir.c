
#include "../../minishell.h"

/*
 * Cleans up resources during pipe parsing.
 * Frees allocated token arrays and structures.
 * Used for error handling in pipe setup.
*/
void	cleanup_pipe_data(t_ta *new_ta, char **sub_tokens, int last_alloc)
{
	int	i;

	if (sub_tokens)
	{
		i = 0;
		while (i < last_alloc)
			free(sub_tokens[i++]);
		free(sub_tokens);
	}
	if (new_ta)
		free_tokenarray(new_ta);
}

/*
 * Checks if token is a redirection operator.
 * Recognizes <, >, << and >> as valid redirections.
 * Returns 1 if token is redirection, 0 otherwise.
*/
int	is_redirect(const char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (1);
	if (ft_strcmp(token, ">>") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

/*
 * Determines type of redirection from token.
 * Returns:
 * - 0 for input (<)
 * - 1 for output (>)
 * - 2 for heredoc (<<)
 * - 3 for append (>>)
 * - -1 for non-redirection tokens
*/
int	get_redirect_type(char *token)
{
	if (ft_strcmp(token, "<") == 0)
		return (0);
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (2);
	if (ft_strcmp(token, ">>") == 0)
		return (3);
	return (-1);
}

/*
 * Adds new redirection to command structure.
 * Creates and initialisez redirection structure.
 * Links redirection into command's redirection list.
 * Handles heredoc and quote status tracking.
*/
void	add_redirect(t_cmd *cmd, int type, char *file, int eof_quoted)
{
	t_redirect	*new;
	t_redirect	*last;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return ;
	new->type = type;
	new->file = ft_strdup(file);
	new->eof_quoted = eof_quoted;
	if (!new->file)
	{
		free(new);
		return ;
	}
	new->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = new;
	else
	{
		last = cmd->redirects;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}