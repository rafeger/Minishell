/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:31:49 by zmurie            #+#    #+#             */
/*   Updated: 2024/12/10 19:40:50 by zmurie           ###   ########.fr       */
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

char	*ft_strjoin2(char *s1, char const *s2)
{
	int		len;
	char	*sfinal;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	sfinal = malloc(sizeof(char) * (len + 1));
	if (sfinal == NULL)
		return (NULL);
	sfinal[0] = '\0';
	ft_strcat(sfinal, s1);
	ft_strcat(sfinal, s2);
	sfinal[len] = '\0';
	free(s1);
	return (sfinal);
}
