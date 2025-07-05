/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:06:38 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:06:39 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_cmd	*setup_pipe(t_ta *new_ta, t_ta *t_a, int idx, char **stok)
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

static int	handle_pipe_token(t_cmd *cmd, t_ta *t_a, int *i)
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

static void	handle_redirect(t_cmd *cmd, t_ta *t_a, int *i)
{
	int	redir_kind;
	int	file_token_idx;

	redir_kind = get_redirect_type(t_a->tokens[*i]);
	if (redir_kind >= 0)
	{
		file_token_idx = *i + 1;
		while (file_token_idx < t_a->t_tot
			&& ft_strcmp(t_a->tokens[file_token_idx], " ") == 0)
			file_token_idx++;
		if (file_token_idx < t_a->t_tot && t_a->tokens[file_token_idx])
		{
			add_redirect(cmd, redir_kind, t_a->tokens[file_token_idx],
				t_a->quoted[file_token_idx]);
			*i = file_token_idx + 1;
		}
		else
			(*i)++;
	}
}

static void	handle_empty_token(t_cmd *cmd, t_ta *t_a, int *i)
{
	char	*space_content;

	if ((*i + 1) < t_a->t_tot && ft_strcmp(t_a->tokens[*i + 1], " ")
		== 0 && !t_a->quoted[*i + 1])
	{
		space_content = ft_strdup(t_a->tokens[*i + 1]);
		if (space_content)
		{
			add_argument(cmd, space_content, t_a->quoted[*i + 1]);
			free(space_content);
		}
	}
	else
	{
		add_argument(cmd, "", t_a->quoted[*i]);
	}
}

static int	process_token(t_cmd *cmd, t_ta *t_a, int *i)
{
	if (!handle_pipe_token(cmd, t_a, i))
		return (0);
	if (*i >= t_a->t_tot)
		return (1);
	if (ft_strcmp(t_a->tokens[*i], " ") == 0 && !t_a->quoted[*i] && !cmd->c_name)
	{
		(*i)++; 
		return (1);
	}
	if (get_redirect_type(t_a->tokens[*i]) >= 0 && !t_a->quoted[*i])
	{
		handle_redirect(cmd, t_a, i);
	}
	else if (ft_strcmp(t_a->tokens[*i], "") == 0)
	{
		handle_empty_token(cmd, t_a, i);
		(*i)++; 
	}
	else
	{
		add_argument(cmd, t_a->tokens[*i], t_a->quoted[*i]);
		(*i)++; 
	}
	return (1);
}

t_cmd	*parse_tokens(t_ta *t_a)
{
	t_cmd	*cmd;
	int		token_pos;

	if (!t_a || !t_a->tokens)
		return (NULL);
	cmd = cmd_initialisation(); 
	if (!cmd)
		return (NULL);
	token_pos = 0;
	while (token_pos < t_a->t_tot && ft_strcmp(t_a->tokens[token_pos], "") == 0)
		token_pos++;
	while (token_pos < t_a->t_tot)
	{
		if (!process_token(cmd, t_a, &token_pos))
		{
			free_command(cmd); 
			return (NULL);
		}
	}
	if (!cmd->c_name && cmd->arg_count == 0 && cmd->redirects == NULL)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
