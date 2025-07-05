/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:56:56 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:56:58 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_cmd	*setup_pipe(t_ta *new_ta, t_ta *t_a, int idx, char **stok)
{
	t_cmd	*next_cmd;

	new_ta->tokens = stok;
	if (!init_quoted_array(new_ta, t_a, idx))
	{
		cleanup_pipe_data(new_ta, stok, new_ta->t_tot);
		return (NULL);
	}
	next_cmd = parse_tokens(new_ta);
	free_tokenarray(new_ta);
	return (next_cmd);
}

int	handle_pipe(t_cmd *cmd, t_ta *t_a, int index)
{
	t_ta	*new_ta;
	t_cmd	*next_cmd;
	char	**sub_tokens;

	if (index + 1 >= t_a->t_tot)
		return (0);
	new_ta = init_new_ta(t_a, index);
	if (!new_ta)
		return (0);
	sub_tokens = create_sub_tokens(t_a, index, new_ta);
	if (!sub_tokens)
	{
		cleanup_pipe_data(new_ta, NULL, 0);
		return (0);
	}
	next_cmd = setup_pipe(new_ta, t_a, index, sub_tokens);
	if (!next_cmd)
		return (0);
	next_cmd->prev = cmd;
	cmd->next = next_cmd;
	return (1);
}

int	handle_pipe_token(t_cmd *cmd, t_ta *t_a, int *i)
{
	if (ft_strcmp(t_a->tokens[*i], "|") == 0 && !t_a->quoted[*i])
	{
		if (!handle_pipe(cmd, t_a, *i))
		{
			free_command(cmd);
			return (0);
		}
		*i = t_a->t_tot;
	}
	return (1);
}
