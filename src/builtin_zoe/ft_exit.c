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

static void	error_not_number(char *str)
{
	ft_putendl_fd("exit", 2);
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static int	check_number2(char *str, int i, int is_negative)
{
	unsigned long	number;
	unsigned long	digit;

	number = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			digit = str[i] - '0';
			if (number > LONG_MAX / 10)
				return (1);
			if (number == LONG_MAX / 10)
			{
				if (!is_negative && digit > LONG_MAX % 10)
					return (1);
				if (is_negative && digit > (unsigned long)LONG_MAX % 10 + 1)
					return (1);
			}
			number = number * 10 + digit;
			i++;
		}
		else
			return (1);
	}
	return (0);
}

static int	check_number(char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	if (str == NULL)
		return (0);
	is_negative = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			is_negative = 1;
		i++;
	}
	return (check_number2(str, i, is_negative));
}

void	ft_exit(t_cmd *cmd, t_shell_data *data)
{
	int	status;

	status = 0;
	if (cmd->args[1] && check_number(cmd->args[1]))
	{
		error_not_number(cmd->args[1]);
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
		data->last_exit_status = ft_atoi(cmd->args[1]) % 256;
		status = data->last_exit_status;
	}
	ft_cleanup_shell(&data);
	exit(status);
}
