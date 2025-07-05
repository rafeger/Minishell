/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:57:33 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 09:57:34 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

int	is_special_char(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '$');
}

void	ensure_token_capacity(t_ta *lx)
{
	size_t	oldsize;
	char	*tmp;

	oldsize = lx->tokensize + 1;
	if (lx->tokenindex == lx->tokensize)
	{
		if (lx->tokensize == 0)
			lx->tokensize = 1;
		else
			lx->tokensize *= 2;
		tmp = ft_realloc(lx->token, oldsize * sizeof(char),
				(lx->tokensize + 1) * sizeof(char));
		if (!tmp)
			return ;
		lx->token = tmp;
		lx->token[lx->tokenindex] = '\0';
	}
}

void	expand_token_array(t_ta *lx)
{
	int		new_cap;
	char	**new_tokens;
	int		*new_quoted;

	new_cap = lx->cap * 2;
	new_tokens = ft_realloc(lx->tokens, lx->cap * sizeof(char *),
			new_cap * sizeof(char *));
	new_quoted = ft_realloc(lx->quoted, lx->cap * sizeof(int),
			new_cap * sizeof(int));
	if (!new_tokens || !new_quoted)
		return ;
	lx->tokens = new_tokens;
	lx->quoted = new_quoted;
	lx->cap = new_cap;
}
