/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:19:36 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:19:37 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_blanks(const char *str, int idx)
{
	while (str[idx] && ft_isspace(str[idx]))
		idx++;
	return (idx);
}

int	is_it_in_q(const char *str, int idx)
{
	int		pos;
	int		in_quotes;
	char	quote_type;

	pos = 0;
	in_quotes = 0;
	quote_type = 0;
	while (pos <= idx)
	{
		if ((str[pos] == '\'' || str[pos] == '"') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = str[pos];
		}
		else if (str[pos] == quote_type && in_quotes)
		{
			in_quotes = 0;
			quote_type = 0;
		}
		pos++;
	}
	return (in_quotes);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
}

void	print_syntax_error_char(char c)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putchar_fd(c, 2);
	ft_putendl_fd("'", 2);
}

void	print_newline_error(void)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd("syntax error near unexpected token 'newline'", 2);
}
