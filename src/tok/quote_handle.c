/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:57:40 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 09:57:41 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	quote_state(t_ta *lx, char c)
{
	int	was_quoted;

	if (is_quote_char(c))
	{
		if (!lx->in_q)
		{
			lx->in_q = 1;
			lx->quotechar = c;
			lx->second_quote = 1;
		}
		else if (c == lx->quotechar)
		{
			was_quoted = lx->in_q;
			lx->in_q = 0;
			lx->quotechar = '\0';
			lx->second_quote = was_quoted;
		}
		else
			lx->token[lx->tokenindex++] = c;
	}
	else
		lx->token[lx->tokenindex++] = c;
}

void	handle_quotes(t_ta *lx, char **input)
{
	int	was_in_q;

	if ((!lx->in_q) && ((**input == '"' && *(*input + 1) == '"')
			|| (**input == '\'' && *(*input + 1) == '\'')))
	{
		if (*(*input + 2) && ft_isspace(*(*input + 2)))
			lx->trailing_space = 1;
		*input += 1;
		return ;
	}
	was_in_q = lx->in_q;
	quote_state(lx, **input);
	if (was_in_q && !lx->in_q && *(*input + 1)
		&& is_special_char(*(*input + 1)))
		finalize_token(lx);
	if (!lx->in_q && *(*input + 1) && ft_isspace(*(*input + 1)))
		lx->trailing_space = 1;
}

int	check_unclosed_quotes(t_ta *t_array, t_shell_data *shell_data)
{
	if (t_array->in_q)
	{
		write(2, "minishell: syntax error: unclosed quote\n", 40);
		if (shell_data)
			shell_data->last_exit_status = 2;
		return (1);
	}
	return (0);
}

t_ta	*clean_lexer(t_ta *t_array)
{
	free_tokenarray(t_array);
	return (NULL);
}
