/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 10:59:27 by zmurie            #+#    #+#             */
/*   Updated: 2025/06/29 10:59:29 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static void	ft_append_char(char *new_str, size_t *j, char c)
{
	new_str[*j] = c;
	(*j)++;
}

static void	ft_handle_single_quote(char *str, size_t *i, char *new_str,
	size_t *j, t_quote_state *state)
{
	if (*state == STATE_NONE)
		*state = STATE_IN_SINGLE_QUOTE;
	else if (*state == STATE_IN_SINGLE_QUOTE)
		*state = STATE_NONE;
	else
		ft_append_char(new_str, j, str[*i]);
	(*i)++;
}

static void	ft_handle_double_quote(char *str, size_t *i, char *new_str,
	size_t *j, t_quote_state *state)
{
	if (*state == STATE_NONE)
		*state = STATE_IN_DOUBLE_QUOTE;
	else if (*state == STATE_IN_DOUBLE_QUOTE)
		*state = STATE_NONE;
	else
		ft_append_char(new_str, j, str[*i]);
	(*i)++;
}

char	*shell_quote_removal(char *str)
{
	char			*new_str;
	size_t			i;
	size_t			j;
	size_t			len;
	t_quote_state	state;

	if (!str)
		return (NULL);
	len = strlen(str);
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	state = STATE_NONE;
	while (i < len)
	{
		if (str[i] == '\'')
			ft_handle_single_quote(str, &i, new_str, &j, &state);
		else if (str[i] == '\"')
			ft_handle_double_quote(str, &i, new_str, &j, &state);
		else
			ft_append_char(new_str, &j, str[i++]);
	}
	new_str[j] = '\0';
	return (new_str);
}
