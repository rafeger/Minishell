/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:49:11 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 19:49:13 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static void	cleanup(t_shell_data *data, char *pathname,
	char **tab_env, int need_free_pathname)
{
	if (need_free_pathname)
		free(pathname);
	if (tab_env)
		free(tab_env);
	ft_cleanup_shell(&data);
	rl_clear_history();
}

static int	handle_directory_error(char **commande, t_shell_data *data)
{
	struct stat	file_stat;

	if (!stat(commande[0], &file_stat))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(commande[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(126);
	}
	return (0);
}

static char	*get_command_path(char **commande, t_env *env, int *need_free)
{
	char	*pathname;

	if (access(commande[0], F_OK) == 0)
	{
		pathname = commande[0];
		*need_free = 0;
	}
	else
	{
		pathname = get_pathname(commande[0], env);
		*need_free = 1;
	}
	return (pathname);
}

void	execute(t_shell_data *data, t_cmd *cmd)
{
	char	*pathname;
	char	**commande;
	char	**tab_env;
	int		need_free_pathname;

	need_free_pathname = 0;
	commande = cmd->args;
	handle_directory_error(commande, data);
	pathname = get_command_path(commande, data->env, &need_free_pathname);
	if (!pathname)
	{
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(127);
	}
	tab_env = convert_list_to_tab_str(data->env);
	if (execve(pathname, commande, tab_env) == -1)
	{
		perror("execve");
		cleanup(data, pathname, tab_env,
			need_free_pathname);
		exit(EXIT_FAILURE);
	}
}
