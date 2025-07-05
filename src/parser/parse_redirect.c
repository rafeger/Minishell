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
		{
			free(sub_tokens[i]);
			i++;
		}
		free(sub_tokens);
	}
	if (new_ta)
		free_tokenarray(new_ta);
}

int	get_redirect_type(char *token)
{
	int			i;
	static const t_redirect_map	redirect_types[] = {
	{"<", REDIR_IN},
	{">", REDIR_OUT},
	{"<<", HERE_DOC},
	{">>", APPEND},
	{NULL, -1}
	};

	i = 0;
	while (redirect_types[i].symbol)
	{
		if (ft_strcmp(token, redirect_types[i].symbol) == 0)
			return (redirect_types[i].type);
		i++;
	}
	return (-1);
}

t_redirect	*create_redirect_node(int type, char *file, int quoted)
{
	t_redirect	*new_node;

	new_node = malloc(sizeof(t_redirect));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->file = ft_strdup(file);
	new_node->eof_quoted = quoted;
	new_node->next = NULL;
	if (!new_node->file)
	{
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

t_redirect	*find_list_tail(t_redirect *head)
{
	t_redirect	*current;

	current = head;
	while (current->next)
		current = current->next;
	return (current);
}
