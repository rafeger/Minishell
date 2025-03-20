/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:53:10 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/15 16:25:28 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (*little == '\0')
		return ((char *)big);
	j = 0;
	while (j < len && big[j])
	{
		i = 0;
		while (little[i] && big[j] && big[j] == little[i] && j < len)
		{
			i++;
			j++;
		}
		if (little[i] == '\0')
			return ((char *)&big[j - i]);
		j = j - i + 1;
	}
	return (NULL);
}
