/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:16:27 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/24 11:16:28 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	ft_env(t_shell_data *shell_data)
{
	t_env	*tmp;

	tmp = shell_data->env;
	while (tmp)
	{
		ft_printf("%s=", tmp->key);
		if (tmp->value)
			ft_printf("%s", tmp->value);
		ft_printf("\n");
		tmp = tmp->next;
	}
	return (0);
}
