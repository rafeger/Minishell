/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:14:46 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/18 11:14:48 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

static void	change_pwd()
{
	
}

int	ft_cd(char **args)
{
	if (args && args[1] && args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		return (1);
    }
	if (!chdir(args[1]))
	{

		return (0);
	}
	else
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(chdir(args[1]), 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);	
	}
}