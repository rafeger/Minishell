/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_cmd_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:08:02 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:08:03 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	*alloc_quoted(int cap)
{
	int	*quoted;

	quoted = malloc(sizeof(int) * cap);
	if (!quoted)
		return (NULL);
	ft_memset(quoted, 0, sizeof(int) * cap);
	return (quoted);
}

void	tokenarray_init_second(t_ta *t_a)
{	
	if (!t_a)
		return ;
	t_a->tokenindex = 0;
	t_a->in_q = 0;
	t_a->quotechar = '\0';
	t_a->token = NULL;
	t_a->tokensize = 0;
	t_a->trailing_space = 0;
	t_a->second_quote = 0;
	t_a->t_tot = 0;
	t_a->cap = 10;
}

t_ta	*tokenarray_init(void)
{
	t_ta	*t_a;

	t_a = malloc(sizeof(t_ta));
	if (!t_a)
		return (NULL);
	ft_memset(t_a, 0, sizeof(t_ta));
	t_a->tokens = malloc(sizeof(char *) * 10);
	if (!t_a->tokens)
	{
		free(t_a);
		return (NULL);
	}
	t_a->quoted = alloc_quoted(10);
	if (!t_a->quoted)
	{
		free(t_a->tokens);
		free(t_a);
		return (NULL);
	}
	tokenarray_init_second(t_a);
	return (t_a);
}

t_cmd	*cmd_initialisation(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->tty_backup = -1;
	cmd->heredoc_fd = -1;
	return (cmd);
}
