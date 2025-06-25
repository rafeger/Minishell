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

int	check_is_in_env(char *var, t_env **env)
{
    t_env *current;
    t_env *prev;
    
    if (!var || !env || !*env)
        return (0);
    
    current = *env;
    prev = NULL;
    
    while (current)
    {
        if (ft_strncmp(var, current->key, ft_strlen(var)) == 0 && 
            ft_strlen(var) == ft_strlen(current->key))
		{
			remove_env_var(env, current->key);
			return (0);
		}			
		current = current->next;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_shell_data *shell_data)
{
	int		i;

	i = 0;
	while (cmd->args[i])
	{
		check_is_in_env(cmd->args[i], &shell_data->env);
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