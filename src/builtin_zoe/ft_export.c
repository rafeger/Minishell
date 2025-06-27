/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:17:02 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/24 11:17:04 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static char	*get_value(char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 1;
	while (str[i - 1] != '=')
		i++;
	if (str[i] == '\0')
		return (NULL);
	while (str[i + len])
		len++;
	return (ft_substr(str, i, len));
}

static void	len_key(char *str, int *len, char stop)
{
	while (str[*len] != stop)
			*len++;
}

static char	*get_key(char *str, bool *concat)
{
	int		len;
	int		i;
	char	*key;

	i = 0;
	len = 0;
	if (*concat == true)
		len_key(str, &len, '+');
	else
		len_key(str, &len, '=');
	key = malloc(sizeof(char *) * len + 1);
	if (!key)
		exit(EXIT_FAILURE);
	while (i < len)
	{
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

static int	create_new_env_var(char *str, t_env *env)
{
	bool	concat;
	t_env	*tmp_env;
	t_env	*new_node;

	concat = false;
	if (!env || check_valid_name_var(str) || name_var_exist(str, env, &concat))
		return (1);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (1);
	new_node->key = get_key(str, &concat);
	new_node->value = get_value(str);
	new_node->next = NULL;
	new_node->prev = NULL;
	if (!(env))
	{
		env = new_node;
		return (0);
	}
	tmp_env = env;
	while (tmp_env->next)
		tmp_env = tmp_env->next;
	tmp_env->next = new_node;
	new_node->prev = tmp_env;
	return (0);
}

int	ft_export(t_cmd *cmd, t_shell_data *shell_data)
{
	int	i;

	if (!cmd->args[1])
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		create_new_env_var(cmd->args[i], shell_data->env);
		i++;
	}
	return (0);
}

/*int	main(void)
{
	t_env	**env;
	char	**str = malloc(sizeof(char *)*2);
	//char	*a = "OLDPWD";
	//int		i = 0;

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
	str[0] = "export";
	str[1] = "JE_MANGE=du_pain";
	ft_export(str, env);
	// while(env[i])
	// {
	// 	printf("%s\n", env[i]->str);
	// 	i++;
	// }
	t_env *tmp = env[0];
	while(tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}	
	return (0);
}*/