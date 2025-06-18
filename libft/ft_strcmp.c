/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:13:59 by zmurie            #+#    #+#             */
/*   Updated: 2024/09/26 10:24:28 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

// int	main(void)
// {
// 	int a;
// 	int res;
// 	char s1[] = "abd";
// 	char s2[] = "abdc";
// 	a = ft_strcmp(s1, s2);
// 	printf("%d\n", a);
// 	//res = strcmp(s1, s2);
// 	//printf("%d\n", res);

// 	return(0);
// }
