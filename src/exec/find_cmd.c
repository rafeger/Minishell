/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:38:17 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/14 15:38:20 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

static void	print_command_not_found(char *cmd)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

static void	cleanup_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

static char	*construct_and_check_path(char *path, char *cmd)
{
	char	*start_path;
	char	*exec;

	start_path = ft_strjoin(path, "/");
	exec = ft_strjoin(start_path, cmd);
	free(start_path);
	if (access(exec, F_OK) == 0)
		return (exec);
	free(exec);
	return (NULL);
}

char	*get_pathname(char *cmd, t_env *env)
{
	char	**paths;
	char	*exec;
	int		i;
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env && ft_strnstr(tmp_env->value, "PATH", 4) == 0)
		tmp_env = tmp_env->next;
	paths = ft_split(tmp_env->value + 5, ':');
	i = 0;
	while (paths[i])
	{
		exec = construct_and_check_path(paths[i], cmd);
		if (exec)
		{
			cleanup_split(paths);
			return (exec);
		}
		i++;
	}
	cleanup_split(paths);
	print_command_not_found(cmd);
	return (NULL);
}

