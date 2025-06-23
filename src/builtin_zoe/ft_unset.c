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
#include "../../include/minishell.h"

void	delete_env_var(t_env *tmp_env)
{
	if (tmp_env->prev == NULL && tmp_env->next == NULL)
		return ;
	else if (tmp_env->prev == NULL)
	{
		tmp_env->next->prev = tmp_env->prev;
		return ;
	}
	else if (tmp_env->next == NULL)
	{
		tmp_env->prev->next = tmp_env->next;
		return ;
	}
	tmp_env->prev->next = tmp_env->next;
	tmp_env->next->prev = tmp_env->prev;
}

int	check_is_in_env(char *var, t_env *env)
{
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (!ft_strncmp(var, tmp_env->value, ft_strlen(var)))
		{
			delete_env_var(tmp_env);
			tmp_env->value = NULL;
			tmp_env->key = NULL;
			return (0);
		}			
		tmp_env = tmp_env->next;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_shell_data *shell_data)
{
	int		i;

	i = 0;
	while (cmd->args[i])
	{
		check_is_in_env(cmd->args[i], shell_data->env);
		i++;
	}
	return (0);
}

/*int	main(void)
{
	t_env	**env;
	char	**str = malloc(sizeof(char *)*2);
	char	*a = "OLDPWD";
	int		i = 0;

	env = malloc(4 * sizeof(t_env));
	env[0] = malloc(sizeof(t_env));
	env[1] = malloc(sizeof(t_env));
	env[2] = malloc(sizeof(t_env));
	env[0]->prev = NULL;
	env[0]->str ="USER=zmurie";
	env[0]->next = env[1];
	env[1]->prev = env[0];
	env[1]->str = "OLDPWD=/home/zmurie";
	env[1]->next = env[2];
	env[2]->prev = env[1];
	env[2]->str = "QT_IM_MODULE=ibus";
	env[2]->next = NULL;
	str[0] = a;
	str[1] = NULL;
	ft_unset(str, env);
	while(env[i])
	{
		printf("%s\n", env[i]->str);
		i++;
	}
	t_env *tmp = env[0];
	while(tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}	
	return (0);
}*/