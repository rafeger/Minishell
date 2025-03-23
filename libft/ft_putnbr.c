/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:45:29 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/20 13:29:01 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_putnbr(int n)
{
	char	*num;
	int		len;

	num = ft_itoa(n);
	if (!num)
		return (0);
	len = ft_putstr(num);
	free(num);
	return (len);
}

/*int	main(void)
{
	ft_putnbr(14129);
	write(1, "\n", 1);
	ft_putnbr(0X2A);
	return (0);
}*/
