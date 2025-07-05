/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:06:44 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:06:45 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
	t_redirect	*n_redir_node;
	t_redirect	*current_node;

	n_redir_node = malloc(sizeof(t_redirect));
	if (!n_redir_node)
		return ;
	n_redir_node->type = type;
	n_redir_node->file = ft_strdup(file);
	n_redir_node->eof_quoted = eof_quoted;
	if (!n_redir_node->file)
	{
		free(n_redir_node);
		return ;
	}
	n_redir_node->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = n_redir_node;
	else
	{
		current_node = cmd->redirects;
		while (current_node->next)
			current_node = current_node->next;
		current_node->next = n_redir_node;
	}
}
