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

static int	is_single_char_redirect(char *token, char c, int expected_type)
{
	if (ft_strlen(token) == 1 && token[0] == c)
		return (expected_type);
	return (-1);
}

static int	is_double_char_redirect(char *token, char c, int expected_type)
{
	if (ft_strlen(token) == 2 && token[0] == c && token[1] == c)
		return (expected_type);
	return (-1);
}

int	get_redirect_type(char *token)
{
	int	result;

	if (!token)
		return (-1);
	result = is_single_char_redirect(token, '<', REDIR_IN);
	if (result != -1)
		return (result);
	result = is_single_char_redirect(token, '>', REDIR_OUT);
	if (result != -1)
		return (result);
	result = is_double_char_redirect(token, '<', HERE_DOC);
	if (result != -1)
		return (result);
	result = is_double_char_redirect(token, '>', APPEND);
	if (result != -1)
		return (result);
	return (-1);
}

t_redr	*create_redirect_node(int type, char *file, int quoted)
{
	t_redr	*new_node;

	new_node = malloc(sizeof(t_redr));
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
