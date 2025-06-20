/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:30:00 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/15 17:25:21 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

// static char	*ft_strcat(char *dest, const char *src)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (dest[i] != '\0')
// 	{
// 		i++;
// 	}
// 	while (src[j] != '\0')
// 	{
// 		dest[i] = src[j];
// 		i++;
// 		j++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	char	*sfinal;

	len = ft_strlen(s1) + ft_strlen(s2);
	sfinal = malloc(sizeof(char) * (len + 1));
	if (sfinal == NULL)
		return (NULL);
	sfinal[0] = '\0';
	ft_strcat(sfinal, s1);
	ft_strcat(sfinal, s2);
	sfinal[len] = '\0';
	return (sfinal);
}

/*int	main(void)
{
	char s1[] = "Je mange ";
	char s2[] = "une glace";
	printf("%s\n", ft_strjoin(s1, s2));
}*/
