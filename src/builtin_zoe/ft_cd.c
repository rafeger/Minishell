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
#include "../../include/minishell.h"

static t_env	*find_env_var(t_env *env, const char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void	set_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*var;
	t_env	*new_var;

	var = find_env_var(*env, key);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
	else
	{
		new_var = malloc(sizeof(t_env));
		if (!new_var)
			return ;
		new_var->key = ft_strdup(key);
		new_var->value = ft_strdup(value);
		new_var->next = *env;
		*env = new_var;
	}
}

static void	update_pwd_vars(t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;
	t_env	*pwd_var;

	old_pwd = NULL;
	new_pwd = NULL;
	pwd_var = find_env_var(*env, "PWD");
	if (pwd_var && pwd_var->value)
		old_pwd = ft_strdup(pwd_var->value);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("getcwd");
		if (old_pwd)
			free(old_pwd);
		return ;
	}
	if (old_pwd)
	{
		set_env_var(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	set_env_var(env, "PWD", new_pwd);
	free(new_pwd);
}

static char	*get_cd_target_path(t_cmd *cmd, t_shell_data *shell_data)
{
	t_env	*home_var;

	if (cmd->arg_count == 1)
	{
		home_var = find_env_var(shell_data->env, "HOME");
		if (!home_var || !home_var->value)
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home_var->value);
	}
	else if (cmd->arg_count > 2)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		return (NULL);
	}
	else
		return (cmd->args[1]);
}

int	ft_cd(t_cmd *cmd, t_shell_data *shell_data)
{
	char	*path;

	path = get_cd_target_path(cmd, shell_data);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("bash: cd: ", 2);
		perror(path);
		return (1);
	}
	update_pwd_vars(&shell_data->env);
	return (0);
}
