/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:57:13 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/05 09:57:17 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = ft_strlen(src);
	while (*src != '\0')
		*dest++ = *src++;
	*dest = '\0';
	return (dest - i);
}
