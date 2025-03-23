/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 16:50:46 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 12:38:08 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	nbr_mots(const char *s, char c)
{
	int	nbr;
	int	mot;

	nbr = 0;
	mot = 0;
	while (*s)
	{
		if (*s != c && !mot)
		{
			mot = 1;
			nbr++;
		}
		else if (*s == c)
		{
			mot = 0;
		}
		s++;
	}
	return (nbr);
}

static char	*mot_dup(const char *debut, size_t len)
{
	char	*mot;

	mot = malloc(len + 1);
	if (!mot)
		return (NULL);
	ft_memcpy(mot, debut, len);
	mot[len] = '\0';
	return (mot);
}

static int	mot_suivant(const char **s, char c, const char **debut, int *len)
{
	*len = 0;
	while (**s == c)
		(*s)++;
	*debut = *s;
	while (**s && **s != c)
	{
		(*s)++;
		(*len)++;
	}
	return (*len > 0);
}

static int	split_mots(const char *s, char c, char **mots)
{
	int			i;
	const char	*debut;
	int			len;

	i = 0;
	while (*s)
	{
		if (mot_suivant(&s, c, &debut, &len))
		{
			mots[i] = mot_dup(debut, len);
			if (!mots[i])
			{
				while (i--)
					free(mots[i]);
				free(mots);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**mots;
	int		mots_nbr;

	if (!s)
		return (NULL);
	mots_nbr = nbr_mots(s, c);
	mots = malloc((mots_nbr + 1) * sizeof(char *));
	if (!mots)
		return (NULL);
	if (split_mots(s, c, mots))
		return (NULL);
	mots[mots_nbr] = NULL;
	return (mots);
}

/*int main(void)
{
    char str8[] = "Je mange une glace";
    char sep = ' ';
    char **split_result = ft_split(str8, sep);

    printf("\nft_split\n");
    for (int i = 0; split_result[i] != NULL; i++) {
        printf("%s\n", split_result[i]);
        free(split_result[i]); // Free each word
    }
    free(split_result); // Free the array of strings
}*/
