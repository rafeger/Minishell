/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:23 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/01 15:03:25 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") \
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") \
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "pwd") \
		||!ft_strcmp(cmd, "unset"))
		return (1);
	return (0);
}

void	do_builtin(t_data *data, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		ft_cd(cmd->args, data->env);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		ft_echo(cmd->args);
	else if (!ft_strcmp(cmd->args[0], "env"))
		ft_env(data->env);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		ft_exit(data, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "export"))
		ft_export(cmd->args, &data->env);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->args[0], "unset"))
		ft_unset(cmd->args, &data->env);
}
