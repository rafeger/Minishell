/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:50:49 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 15:49:47 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*extrait;
	size_t			len_extrait;
	unsigned int	i;
	size_t			s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	len_extrait = 0;
	while (s[start + len_extrait] != '\0' && len_extrait < len)
		len_extrait++;
	extrait = malloc(sizeof(char) * (len_extrait + 1));
	if (extrait == NULL)
		return (NULL);
	i = 0;
	while (i < len_extrait)
	{
		extrait[i] = s[start + i];
		i++;
	}
	extrait[i] = '\0';
	return (extrait);
}

/*int	main(void)
{
	char s[] = "Bonjour";
	printf("%s\n", ft_substr(s, 12, 5));
	return (0);
}*/
