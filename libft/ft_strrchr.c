/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:04:00 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 12:52:06 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			len;
	unsigned char	uc;

	uc = (unsigned char)c;
	len = ft_strlen(s);
	while (len)
	{
		if (s[len] == uc)
			return ((char *)&s[len]);
		len--;
	}
	if (s[len] == uc)
		return ((char *)&s[len]);
	return (NULL);
}

/*#include <stdio.h>
int	main(void)
{
	char s[] = "Bonjour";
	//printf("%s\n", ft_strrchr(s, 'o'));
	printf("%s\n", ft_strrchr(s, 't'));
	return (0);
}*/
