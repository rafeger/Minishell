/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:39:34 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/01 14:39:36 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	cleanup_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	execute(t_data *data, t_command *cmd)
{
	char	*pathname;
	char	**commande;

	commande = ft_split(cmd->args, ' ');
	pathname = get_pathname(commande[0], data->env);
	if (!pathname)
	{
		perror(pathname);
		data->exit = 127;
		cleanup_split(commande);
		return ;
	}
	if (access(pathname, X_OK))
	{
		perror(pathname);
		data->exit = 126;
		cleanup_split(commande);
		return ;
	}
	if (execve(pathname, commande, data->env) == -1)
	{
		perror("execve");
		free(pathname);
		cleanup_split(commande);
		exit(EXIT_FAILURE);
	}
}

void	child_process(t_data *data, t_command *cmd, int *pipefd)
{

	dup2(data->command->infile, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(data->command->infile);
	close(pipefd[0]);
	close(pipefd[1]);
	execute(data, cmd);
}

void	parent_process(t_data *data, t_command *cmd, int *pipefd)
{
	close(pipefd[1]);
	dup2(fd, STDOUT_FILENO);
	dup2(pipefd[0], STDIN_FILENO);
	close(fd);
	close(pipefd[0]);
	close(pipefd[1]);
	execute(argv[3], envp);
}

int	main(t_data *data)
{
	int		    *pipefd;
    t_command   *tmp_cmd;

	tmp_cmd = data->command;
    if (is_one_builtin(data))
		return (do_builtin);
	while (tmp_cmd)
	{
		if (pipe(pipefd) == -1)
			return (1);
		g_signal = fork();
		if (g_signal == -1)
			exit(1);
		if (g_signal == 0)
			child_process(data, tmp_cmd, pipefd);
		parent_process(data, tmp_cmd, pipefd);
	}
	return (0);
}