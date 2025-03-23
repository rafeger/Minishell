/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:20:44 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/20 13:32:33 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_puthex(unsigned int nbr, int uppercase)
{
	const char	*hex_digits;
	int			len;

	len = 0;
	if (uppercase)
		hex_digits = "0123456789ABCDEF";
	else
		hex_digits = "0123456789abcdef";
	if (nbr >= 16)
		len += ft_puthex(nbr / 16, uppercase);
	len += ft_putchar(hex_digits[nbr % 16]);
	return (len);
}
