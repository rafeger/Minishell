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

void	ft_exit(t_cmd *cmd, t_shell_data *data)
{
	if (cmd->args[1] && ft_isnumber(cmd->args[1]))
	{
		data->last_exit_status = 2;
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
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
		data->last_exit_status= ft_atoi(cmd->args[1]) % 256;
		ft_putendl_fd("exit", data->last_exit_status);
	}
	free_all(data);
	exit(data->last_exit_status);
}
