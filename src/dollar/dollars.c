/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgiroux <pgiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:31:27 by pfranke           #+#    #+#             */
/*   Updated: 2025/03/13 13:43:26 by pgiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dollars.h"

char	*makestrfromstartandend(char *str, int start, int end)
{
	char	*new;
	int		i;
	size_t	len;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	if (start < 0 || end < 0 || start > end
		|| start > (int)len || end > (int)len)
		return (NULL);
	new = malloc(sizeof(char) * ((end - start) + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (start < end)
	{
		new[i] = str[start];
		i++;
		start++;
	}
	new[i] = '\0';
	return (new);
}

void	handle_quotes(t_dl *dl)
{
	if (dl->str[dl->i[0]] == '\'' && dl->i[3] == -1)
		dl->i[2] *= -1;
	else if (dl->str[dl->i[0]] == '\"' && dl->i[2] == -1)
		dl->i[3] *= -1;
}

char	*get_variable_name(t_dl *dl)
{
	int	start;

	start = dl->i[0];
	while (ft_isalnum(dl->str[dl->i[0]]) || dl->str[dl->i[0]] == '_')
		dl->i[0]++;
	return (makestrfromstartandend(dl->str, start, dl->i[0]));
}

int	handle_dollar_special_size(t_dl *dl)
{
	int		size;
	char	*tmp;

	size = 0;
	if (dl->str[dl->i[0]] == '$')
		tmp = ft_itoa(getpid());
	else
		tmp = ft_itoa(dl->data->status);
	if (tmp)
	{
		size = ft_strlen(tmp);
		free(tmp);
	}
	dl->i[0]++;
	return (size);
}

int	handle_dollar(t_dl *dl)
{
	int		size;
	char	*name;
	char	*value;

	size = 0;
	dl->i[0]++;
	if (ft_isalnum(dl->str[dl->i[0]]) || dl->str[dl->i[0]] == '_')
	{
		name = get_variable_name(dl);
		if (name)
		{
			value = ft_getenv(dl->env, name);
			if (value)
				size = ft_strlen(value);
			free(name);
		}
	}
	else if (dl->str[dl->i[0]] == '$' || dl->str[dl->i[0]] == '?')
		size = handle_dollar_special_size(dl);
	else
		size = 1;
	return (size);
}
