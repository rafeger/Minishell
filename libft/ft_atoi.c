/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:23:11 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/11 16:53:03 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	i;
	int	signe;
	int	nombre;

	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	signe = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			signe = -1;
		i++;
	}
	nombre = 0;
	while (nptr[i] > 47 && nptr[i] < 58)
	{
		nombre = (nptr[i] - '0') + nombre * 10;
		i++;
	}
		nombre = signe * nombre;
	return (nombre);
}
