/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:57:17 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 09:57:18 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	lex_step(t_ta *lx, char **input)
{
	ensure_token_capacity(lx);
	if (is_quote_char(**input) || lx->in_q)
		handle_quotes(lx, input);
	else if (**input == ' ' || **input == '\t')
		finalize_token(lx);
	else if (!lx->in_q && is_special_char(**input))
		handle_spe_char(lx, input);
	else
		lx->token[lx->tokenindex++] = **input;
	if (!lx->in_q && *(*input + 1) && is_special_char(*(*input + 1)))
		finalize_token(lx);
}

void	lex_input(t_ta *lx, char *input)
{
	while (*input)
	{
		lex_step(lx, &input);
		input++;
	}
	finalize_token(lx);
}

t_ta	*lexer(char *input, t_shell_data *shell_data)
{
	t_ta	*lx;

	lx = tokenarray_init();
	if (!lx)
		return (NULL);
	if (is_only_quotes(input))
		return (create_special_empty_token(lx));
	lex_input(lx, input);
	if (check_unclosed_quotes(lx, shell_data))
		return (clean_lexer(lx));
	if (lx->tokenindex > 0)
		lx->token[lx->tokenindex] = '\0';
	if (lx->tokenindex > 0 && add_token_new(lx, lx->token))
		return (clean_lexer(lx));
	if (!lx->t_tot)
		return (clean_lexer(lx));
	return (lx);
}

int	is_only_quotes(const char *input)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			quotes++;
		else if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (quotes > 0);
}

t_ta	*create_special_empty_token(t_ta *t_array)
{
	t_array->tokens[0] = ft_strdup("");
	t_array->quoted[0] = 1;
	t_array->t_tot = 1;
	return (t_array);
}
