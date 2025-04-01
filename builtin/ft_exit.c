/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:16:09 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/24 11:16:11 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	ft_exit(t_data *data, char **args)
{
	int	std_out;

	if (args[1] && ft_isnumber(args[1]))
	{
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	else if (args[1] && args[2])
	{
		ft_putendl_fd("exit", 2);
		ft_putendl_fd("bash: exit: too many arguments", 2);
		return ;
	}
	else if (!args[1])
		ft_putendl_fd("exit", 2);
	else
	{
		std_out = ft_atoi(args[1]) % 256;
		ft_putendl_fd("exit", std_out);
	}
	data->exit = 1;
}
