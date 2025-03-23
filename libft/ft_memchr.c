/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:28:45 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 12:49:24 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	unsigned char		uc;

	ptr = (const unsigned char *)s;
	uc = (unsigned char)c;
	while (n--)
	{
		if (*ptr == uc)
			return ((void *)ptr);
		ptr++;
	}
	return (NULL);
}
