/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:26:36 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/11 17:35:12 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	taille_nombre(int n)
{
	int	taille;

	taille = 0;
	if (n <= 0)
		taille++;
	while (n != 0)
	{
		n = n / 10;
		taille++;
	}
	return (taille);
}

char	*ft_itoa(int n)
{
	int (taille) = taille_nombre(n);
	char *(str) = malloc((taille + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[taille] = '\0';
	if (n == -2147483648)
	{
		str[--taille] = '8';
		n = n / 10;
	}
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	if (n == 0)
		str[0] = '0';
	while (taille > 0 && n != 0)
	{
		str[--taille] = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}	
