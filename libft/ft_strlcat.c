/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:43:17 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/11 18:23:12 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	unsigned int	dst_len;
	unsigned int	src_len;
	size_t			i;
	size_t			j;

	dst_len = 0;
	src_len = 0;
	i = 0;
	j = 0;
	while (dst[dst_len] != '\0')
		dst_len++;
	while (src[src_len] != '\0')
		src_len++;
	if (siz <= dst_len)
		return (siz + src_len);
	j = dst_len;
	while (src[i] != '\0' && j < (siz - 1))
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (dst_len + src_len);
}
