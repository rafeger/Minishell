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
#include "../../include/minishell.h"

static int	check_number(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else 
			return (1);
	}
	return (0);
}

void	ft_exit(t_cmd *cmd, t_shell_data *data)
{
	int	status;

	status = 0;
	if (cmd->args[1] && check_number(cmd->args[1]))
	{
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		data->last_exit_status = 2;
		status = data->last_exit_status;

	}
	else if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("exit", 2);
		ft_putendl_fd("bash: exit: too many arguments", 2);
		return ;
	}
	else if (!cmd->args[1])
		ft_putendl_fd("exit", 2);
	else
	{
		ft_putendl_fd("exit", data->last_exit_status);
		data->last_exit_status= ft_atoi(cmd->args[1]) % 256;
		status = data->last_exit_status;
	}
	ft_cleanup_shell(&data);
	exit(status);
}
