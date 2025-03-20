/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:16:06 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/20 13:32:13 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_put_hex_rec(unsigned long nbr)
{
	const char	*hex_digits;
	int			len;

	hex_digits = "0123456789abcdef";
	len = 0;
	if (nbr >= 16)
		len += ft_put_hex_rec(nbr / 16);
	len += ft_putchar(hex_digits[nbr % 16]);
	return (len);
}

int	ft_putpointer(void *ptr)
{
	unsigned long	address;
	int				len;

	len = 0;
	if (ptr == NULL)
		return (ft_putstr("(nil)"));
	address = (unsigned long)ptr;
	len += ft_putstr("0x");
	len += ft_put_hex_rec(address);
	return (len);
}
