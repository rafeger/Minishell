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

static size_t	get_len_extrait(const char *s, unsigned int start, size_t len)
{
	size_t	len_extrait;

	len_extrait = 0;
	while (s[start + len_extrait] && len_extrait < len)
		len_extrait++;
	return (len_extrait);
}

static void	slash(char *dest, const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
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
	len_extrait = get_len_extrait(s, start, len);
	extrait = malloc(sizeof(char) * (len_extrait + 1));
	if (!extrait)
		return (NULL);
	slash(extrait, s, start, len_extrait);
	return (extrait);
}


static void	clean_spaces_loop(const char *str, char *result, int *i, int *j)
{
    int	space;

    space = 0;
    while (str[*i])
    {
        if (str[*i] == ' ' || str[*i] == '\t')
        {
            space = 1;
            (*i)++;
        }
        else
        {
            if (space && *j > 0)
                result[(*j)++] = ' ';
            result[(*j)++] = str[(*i)++];
            space = 0;
        }
    }
}

static char	*clean_spaces(const char *str)
{
    char	*result;
    int		i;
    int		j;
    int		len;

    result = NULL;
    i = 0;
    j = 0;
    len = 0;
    if (!str)
        return (NULL);
    len = ft_strlen(str);
    result = malloc(len + 1);
    if (!result)
        return (NULL);
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    clean_spaces_loop(str, result, &i, &j);
    if (j > 0 && result[j - 1] == ' ')
        j--;
    result[j] = '\0';
    return (result);
}


char	*get_value(char *str)
{
    int	len;
    int	i;
    char *raw;
    char *cleaned;

    len = 0;
    i = 1;
    while (str[i - 1] != '=')
        i++;
    if (str[i] == '\0')
        return (NULL);
    while (str[i + len])
    {
        if (str[i + len] == ';')
            break ;
        len++;
    }
    raw = substr_value(str, i, len);
    cleaned = clean_spaces(raw);
    free(raw);
    return (cleaned);
}

int	syntax_error(char c)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}
