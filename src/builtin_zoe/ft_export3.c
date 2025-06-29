/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:33:00 by zmurie            #+#    #+#             */
/*   Updated: 2025/06/29 12:33:02 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static size_t	compute_len_extrait(const char *s, unsigned int start, size_t len)
{
	size_t len_extrait = 0;

	while (s[start + len_extrait] && len_extrait < len)
		len_extrait++;
	return (len_extrait);
}

static void	copy_with_backslash(char *dest, const char *s, unsigned int start, size_t len_extrait)
{
	size_t i = 0;
	size_t j = 0;

	while (i < len_extrait)
	{
		if (s[start + j] == '\\' && s[start + j + 1] == '\\')
		{
			dest[i++] = s[start + j];
			j += 2;
		}
		else if (s[start + j] == '\\')
		{
			j++;
		}
		else
		{
			dest[i++] = s[start + j++];
		}
	}
	dest[i] = '\0';
}

static char	*substr_value(char const *s, unsigned int start, size_t len)
{
	char	*extrait;
	size_t	len_extrait;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	len_extrait = compute_len_extrait(s, start, len);
	extrait = malloc(sizeof(char) * (len_extrait + 1));
	if (!extrait)
		return (NULL);
	copy_with_backslash(extrait, s, start, len_extrait);
	return (extrait);
}

char	*get_value(char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 1;
	while (str[i - 1] != '=')
		i++;
	if (str[i] == '\0')
		return (NULL);
	while (str[i + len])
	{
		if (str[i] == ';')
		{
			len--;
			break;
		}
		len++;
	}
	return (ft_substr(str, i, len));
}


