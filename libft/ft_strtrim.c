/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:36:10 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/25 10:45:14 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	dans_le_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		debut;
	int		fin;
	char	*strim;
	int		i;

	debut = 0;
	while (s1[debut] && dans_le_set(s1[debut], set))
		debut++;
	fin = ft_strlen(s1);
	while (s1[fin - 1] && dans_le_set(s1[fin - 1], set))
		fin--;
	if (debut > fin)
		debut = 0;
	strim = malloc(sizeof(char) * (fin - debut + 1));
	if (strim == NULL)
		return (NULL);
	i = 0;
	while (debut <= fin - 1)
	{
		strim[i] = s1[debut];
		i++;
		debut++;
	}
	strim[i] = '\0';
	return (strim);
}
