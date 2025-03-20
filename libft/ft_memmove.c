/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:14:39 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/12 18:26:06 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	int					i;

	d = dest;
	s = src;
	if (d == s || n == 0)
		return (dest);
	if (s < d)
	{
		while (n--)
			d[n] = s[n];
	}
	else
	{
		i = 0;
		while (n--)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}
