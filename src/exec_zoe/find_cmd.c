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
#include "../../include/minishell.h"

static void	print_command_not_found(char *cmd)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	perror(" ");
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

static char	*search_cmd_in_paths(char **paths, char *cmd)
{
	char	*exec;
	int		i;

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
	return (NULL);
}

char	*get_pathname(char *cmd, t_env *env)
{
	t_env	*tmp_env;
	char	**paths;
	char	*result;

	tmp_env = env;
	while (tmp_env && ft_strnstr(tmp_env->key, "PATH", 4) == 0)
		tmp_env = tmp_env->next;
	if (!tmp_env)
	{
		print_command_not_found(cmd);
		return (NULL);
	}
	paths = ft_split(tmp_env->value + 5, ':');
	if (!paths)
		return (NULL);
	result = search_cmd_in_paths(paths, cmd);
	if (!result)
		print_command_not_found(cmd);
	return (result);
}
