/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:00:43 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/03 17:00:45 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	clean_spaces_loop(const char *str, char *result, int *i, int *j)
{
	int	space;

	space = 0;
	while (str[*i])
	{
		if (str[*i] == ' ' || str[*i] == '\t')
		{
			space = 1;
			(*i)++;
		}
		else
		{
			if (space && *j > 0)
				result[(*j)++] = ' ';
			result[(*j)++] = str[(*i)++];
			space = 0;
		}
	}
}

char	*clean_spaces(const char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	result = NULL;
	i = 0;
	j = 0;
	len = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	clean_spaces_loop(str, result, &i, &j);
	if (j > 0 && result[j - 1] == ' ')
		j--;
	result[j] = '\0';
	return (result);
}
