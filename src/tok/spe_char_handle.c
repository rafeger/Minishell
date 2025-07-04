/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spe_char_handle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:58:45 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 09:58:46 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_spe_tok(t_ta *lx, char **input)
{
	size_t	oldsize;

	if (lx->tokensize < 2)
	{
		oldsize = lx->tokensize + 1;
		lx->tokensize = 2;
		lx->token = ft_realloc(lx->token, oldsize * sizeof(char),
				(lx->tokensize + 1) * sizeof(char));
		if (!lx->token)
			return ;
	}
	if ((**input == '<' && *(*input + 1) == '<')
		|| (**input == '>' && *(*input + 1) == '>'))
	{
		lx->token[1] = **input;
		lx->token[2] = '\0';
		(*input)++;
	}
	else
		lx->token[1] = '\0';
	add_token_new(lx, lx->token);
}

void	handle_spe_char(t_ta *lx, char **input)
{
	char	*spe;

	spe = "<>|$";
	if (lx->second_quote)
	{
		lx->token[lx->tokenindex++] = **input;
		return ;
	}
	if (ft_strchr(spe, **input))
	{
		if (lx->tokenindex > 0)
		{
			lx->token[lx->tokenindex] = '\0';
			add_token_new(lx, lx->token);
			lx->tokenindex = 0;
		}
		lx->token[0] = **input;
		handle_spe_tok(lx, input);
	}
}
