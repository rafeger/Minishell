/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:36:02 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/20 13:27:49 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_handle_format(char c, va_list args)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_putchar(va_arg(args, int));
	else if (c == 's')
		len += ft_putstr(va_arg(args, char *));
	else if (c == 'p')
	len += ft_putpointer(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		len += ft_putnbr(va_arg(args, int));
	else if (c == 'u')
		len += ft_putunsigned(va_arg(args, unsigned int));
	else if (c == 'x')
		len += ft_puthex(va_arg(args, unsigned int), 0);
	else if (c == 'X')
		len += ft_puthex(va_arg(args, unsigned int), 1);
	else if (c == '%')
		len += ft_putchar('%');
	return (len);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total_len;

	total_len = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			total_len += ft_handle_format(*format, args);
		}
		else
			total_len += ft_putchar(*format);
		format++;
	}
	va_end(args);
	return (total_len);
}
