/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:16:45 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/24 11:16:47 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

int	check_is_in_env(char *str, t_list *env)
{
	t_list	*env_tmp;

	env_tmp = env;
	while (env_tmp)
	{
		if (!ft_strncmp(str[i], env_tmp->str, ft_strlen(str[i])))
			delete_env_var(pos)
		env_tmp = env_tmp->next;
	}
}

int	ft_unset(char **str, t_list *env)
{
	int		i;
	int		*pos;
	t_list	*env_tmp;

	i = 0;
	while (str[i])
	{
		env_tmp = env;
		while (env_tmp)
		{
			while (!ft_strncmp(str[i], env_tmp->str, ft_strlen(str[i])))
				env_tmp = env_tmp->next;
			
		} 
	}
	return (0);
}