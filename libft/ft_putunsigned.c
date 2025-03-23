/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:19:25 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/20 13:32:23 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_putunsigned(unsigned int nbr)
{
	int	len;

	len = 0;
	if (nbr >= 10)
		len += ft_putunsigned(nbr / 10);
	len += ft_putchar((nbr % 10) + '0');
	return (len);
}
