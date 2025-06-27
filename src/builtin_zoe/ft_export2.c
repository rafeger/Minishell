/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:59:59 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/28 13:00:01 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static int	not_valid_id(char *str)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	check_valid_name_var(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (not_valid_id(str));
	if (!ft_strchr(str, '=' ))
		return (1);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (0);
		else if (!ft_isalnum(str[i]) && str[i] != '_')
			return (not_valid_id(str));
		i++;
	}
	return (0);
}

static int	update_env_var(char *str, t_env *env, size_t len_name, bool *concat)
{
	t_env	*tmp_env;
	char	*tmp_value;

	tmp_env = env;
	while (tmp_env)
	{
		if ((ft_strncmp(str, tmp_env->key, len_name)) == 0
			&& ft_strlen(tmp_env->key) == len_name)
			break ;
		tmp_env = tmp_env->next;
	}
	if (concat && tmp_env->value)
	{
		tmp_value = tmp_env->value;
		tmp_env->value = ft_strjoin(tmp_value, &str[len_name + 2]);
		free(tmp_value);
	}
	else
	{
		if (tmp_env->value)
			free(tmp_env->value);
		tmp_env->value = ft_strdup(&str[len_name + 1]);
	}
	return (1);
}

int	name_var_exist(char *str, t_env *env, bool *concat)
{
	size_t		len_name;
	t_env		*tmp_env;

	len_name = 0;
	while (str[len_name] != '=')
		len_name++;
	if (str[len_name - 1] == '+')
	{
		*concat = true;
		len_name--;
	}
	tmp_env = env;
	while (tmp_env)
	{
		if ((ft_strncmp(str, tmp_env->key, len_name)) == 0
			&& ft_strlen(tmp_env->key) == len_name)
			return (update_env_var(str, env, len_name, concat));
		tmp_env = tmp_env->next;
	}
	return (0);
}
