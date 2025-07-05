/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:26:56 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:26:57 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

char	*get_var_name(t_expand_dollar *ed)
{
	int	start;

	start = ed->pos[0];
	while (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
		ed->pos[0]++;
	return (substr_range(ed->input, start, ed->pos[0]));
}

void	expand_special(t_expand_dollar *ed)
{
	char	*tmp;

	tmp = NULL;
	if (ed->input[ed->pos[0]] == '$')
		tmp = ft_itoa(getpid());
	else
		tmp = ft_itoa(ed->shell->last_exit_status);
	if (tmp)
	{
		append_str(ed, tmp);
		free(tmp);
	}
	ed->pos[0]++;
}

void	expand_var(t_expand_dollar *ed)
{
	char	*name;
	char	*value;

	name = get_var_name(ed);
	value = NULL;
	if (name)
	{
		value = find_env_val(ed->env, name);
		if (value)
			append_str(ed, value);
		free(name);
	}
}
