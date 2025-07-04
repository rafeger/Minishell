/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfranke <pfranke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 02:02:50 by pfranke           #+#    #+#             */
/*   Updated: 2025/04/02 16:43:55 by pfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dollars.h"

void	handle_variable(t_dl *dl)
{
	char	*name;
	char	*value;

	name = get_variable_name(dl);
	if (name)
	{
		value = ft_getenv(dl->env, name);
		if (value)
			append_and_update(dl, value);
		free(name);
	}
}

void	handle_special(t_dl *dl)
{
	char	*tmp;

	if (dl->str[dl->i[0]] == '$')
		tmp = ft_itoa(12345);
	else
		tmp = ft_itoa(dl->data->status);
	if (tmp)
	{
		append_and_update(dl, tmp);
		free(tmp);
	}
	dl->i[0]++;
}

void	handle_dollar_init(t_dl *dl)
{
	dl->i[0]++;
	if (ft_isalnum(dl->str[dl->i[0]]) || dl->str[dl->i[0]] == '_')
		handle_variable(dl);
	else if (dl->str[dl->i[0]] == '$' || dl->str[dl->i[0]] == '?')
		handle_special(dl);
	else
	{
		dl->new[dl->i[1]++] = '$';
		dl->new[dl->i[1]] = '\0';
	}
}

void	process_string(t_dl *dl)
{
	while (dl->str[dl->i[0]])
	{
		handle_quotes(dl);
		if (dl->str[dl->i[0]] == '$' && dl->i[2] == -1)
			handle_dollar_init(dl);
		else
		{
			dl->new[dl->i[1]++] = dl->str[dl->i[0]++];
			dl->new[dl->i[1]] = '\0';
		}
	}
}

char	*dl_init(char *str, t_env *env, t_data *data)
{
	t_dl	dl;

	(void)env;
	if (data->e_first == NULL)
		return (str);
	dl.env = data->e_first;
	dl.str = str;
	dl.data = data;
	dl.i[0] = 0;
	dl.i[1] = 0;
	dl.i[2] = -1;
	dl.i[3] = -1;
	dl.total_size = dl_getsize(&dl);
	dl.i[0] = 0;
	dl.new = malloc(sizeof(char) * (dl.total_size + 1));
	if (!dl.new)
		return (NULL);
	dl.new[0] = '\0';
	process_string(&dl);
	dl.new[dl.i[1]] = '\0';
	free(str);
	return (dl.new);
}
