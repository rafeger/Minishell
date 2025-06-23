
#include "../../include/minishell.h"

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


void	add_redirect(t_cmd *cmd, int type, char *file, int eof_quoted)
{
	t_redirect	*new_redir_node;
	t_redirect	*current_node;

	new_redir_node = malloc(sizeof(t_redirect));
	if (!new_redir_node)
		return ;
	new_redir_node->type = type;
	new_redir_node->file = ft_strdup(file);
	new_redir_node->eof_quoted = eof_quoted;
	if (!new_redir_node->file)
	{
		free(new_redir_node);
		return ;
	}
	new_redir_node->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = new_redir_node;
	else
	{
		current_node = cmd->redirects;
		while (current_node->next)
			current_node = current_node->next;
		current_node->next = new_redir_node;
	}
}
