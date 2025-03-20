/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:29:47 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 12:48:33 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	else
	{
		i = 0;
		while (s1[i] && s1[i] == s2[i] && i < n - 1)
			i++;
		if (ft_isascii(s1[i]) && ft_isascii(s2[i]))
			return (s1[i] - s2[i]);
		if (ft_isascii(s1[i]))
			return (-1);
		else
			return (1);
	}
}
