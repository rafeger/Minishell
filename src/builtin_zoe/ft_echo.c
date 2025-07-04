/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:42:30 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/14 11:42:32 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static bool	check_new_line(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (i == (int)ft_strlen(str))
			return (true);
	}
	return (false);
}

static int	safe_write_str(const char *str, size_t len)
{
	if (write(1, str, len) == -1)
	{
		perror("echo: write error");
		return (1);
	}
	return (0);
}

static int	print_args(t_cmd *cmd, int start_idx, int nbr_arg)
{
	int		i;
	size_t	len;

	i = start_idx;
	while (cmd->args[i])
	{
		len = ft_strlen(cmd->args[i]);
		if (safe_write_str(cmd->args[i], len))
			return (1);
		if (i != nbr_arg)
		{
			if (safe_write_str(" ", 1))
				return (1);
		}
		i++;
	}
	return (0);
}

int	ft_echo(t_cmd *cmd)
{
	int		i;
	int		nbr_arg;
	bool	new_line;

	nbr_arg = 0;
	new_line = false;
	while (cmd->args[nbr_arg + 1])
		nbr_arg++;
	i = 0;
	while (cmd->args[++i] && check_new_line(cmd->args[i]))
		new_line = true;
	if (print_args(cmd, i, nbr_arg))
		return (1);
	if (!new_line)
	{
		if (safe_write_str("\n", 1))
			return (1);
	}
	return (0);
}
