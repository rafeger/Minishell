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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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
