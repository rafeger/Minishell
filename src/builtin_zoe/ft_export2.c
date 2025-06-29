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

int	check_valid_value(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')')
			return (syntax_error(str[i]));
		else if (str[i] == '&' || str[i] == ';')
		{
			ft_putstr_fd(&str[i + 1], 2);
			ft_putstr_fd(": command not found\n", 2);
			if (str[i] == '&')
				return (1);
		}
		else if (str[i] == '!')
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(&str[i], 2);
			ft_putstr_fd(": event not found\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	not_valid_id(char *str)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static void	update_env_value(t_env *env, char *str, bool concat)
{
	char	*tmp_value;
	char	*tmp_value2;

	if (concat && env->value)
	{
		tmp_value = env->value;
		tmp_value2 = get_value(str);
		env->value = ft_strjoin(tmp_value, tmp_value2);
		if (tmp_value2)
			free(tmp_value2);
		if (tmp_value)
			free(tmp_value);
	}
	else
	{
		if (env->value)
			free(env->value);
		env->value = get_value(str);
	}
}

static int	update_env_var(char *str, t_env *env, size_t len_name, bool *concat)
{
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if ((ft_strncmp(str, tmp_env->key, len_name)) == 0
			&& ft_strlen(tmp_env->key) == len_name)
			break ;
		tmp_env = tmp_env->next;
	}
	update_env_value(tmp_env, str, *concat);
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
