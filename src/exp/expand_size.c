/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:26:49 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:26:50 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	calc_special_size(t_expand_dollar *ed)
{
	int		size;
	char	*tmp;

	size = 0;
	if (ed->input[ed->pos[0]] == '$')
		tmp = ft_itoa(getpid());
	else
		tmp = ft_itoa(ed->shell->last_exit_status);
	if (tmp)
	{
		size = ft_strlen(tmp);
		free(tmp);
	}
	ed->pos[0]++;
	return (size);
}

static int	handle_alnum_variable(t_expand_dollar *ed)
{
	char	*name;
	char	*value;
	int		size;

	size = 0;
	name = get_var_name(ed);
	if (name)
	{
		value = find_env_val(ed->env, name);
		if (value)
			size = ft_strlen(value);
		free(name);
	}
	return (size);
}

int	calc_dollar_size(t_expand_dollar *ed)
{
	int	size;
	int	empty_quote_len;

	size = 0;
	ed->pos[0]++;
	empty_quote_len = is_empty_quotes(ed);
	if (empty_quote_len > 0)
	{
		ed->pos[0] += empty_quote_len;
		return (0);
	}
	if (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
		size = handle_alnum_variable(ed);
	else if (ed->input[ed->pos[0]] == '$' || ed->input[ed->pos[0]] == '?')
		size = calc_special_size(ed);
	else
		size = 1;
	return (size);
}

int	calc_total_size(t_expand_dollar *ed)
{
	int	size;

	size = 0;
	if (ed->input == NULL || ed->env == NULL)
		return (0);
	ed->pos[0] = 0;
	ed->pos[2] = -1;
	ed->pos[3] = -1;
	while (ed->input[ed->pos[0]])
	{
		update_quotes(ed);
		if (ed->input[ed->pos[0]] == '$' && ed->pos[2] == -1)
			size += calc_dollar_size(ed);
		else
		{
			size++;
			ed->pos[0]++;
		}
	}
	return (size);
}
