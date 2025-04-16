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

static void	execute(t_data *data, t_command *cmd)
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

static void	child_process(t_data *data, t_command *cmd, int *pipefd)
{
	close(pipefd[0]);
	if (cmd->infile >= 0)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile >= 0)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	close(pipefd[1]);
	execute(data, cmd);
}

static void	parent_process(t_data *data, t_command *cmd, int *pipefd)
{
	close(pipefd[1]);
	if (cmd->infile >= 0)
		close(cmd->infile);
	if (cmd->infile == -2)
		cmd->infile = pipefd[0];
	if (cmd->next->args && cmd->next->infile == -2)
		cmd->next->infile = pipefd[0];
	else
		close(pipefd[0]);
}

int	exec(t_data *data)
{
	int			*pipefd;
	t_command	*tmp_cmd;

	tmp_cmd = data->command;
	while (tmp_cmd)
	{
		if (pipe(pipefd) == -1)
			return (1);
		g_signal = fork();
		if (g_signal == -1)
			exit(1);
		if (is_builtin(tmp_cmd))
			do_builtin(data, tmp_cmd);
		if (g_signal == 0)
			child_process(data, tmp_cmd, pipefd);
		else
			parent_process(data, tmp_cmd, pipefd);
		tmp_cmd = tmp_cmd->next;
	}
	return (0);
}

int	main(void)
{
	t_env	**envi;
	t_data	*data;
	char	**str = malloc(sizeof(char *)*2);

	envi = malloc(4 * sizeof(t_env));
	envi[0] = malloc(sizeof(t_env));
	envi[1] = malloc(sizeof(t_env));
	envi[2] = malloc(sizeof(t_env));
	envi[0]->prev = NULL;
	envi[0]->str ="USER=zmurie";
	envi[0]->next = envi[1];
	envi[1]->prev = envi[0];
	envi[1]->str = "OLDPWD=/home/zmurie";
	envi[1]->next = envi[2];
	envi[2]->prev = envi[1];
	envi[2]->str = "QT_IM_MODULE=ibus";
	envi[2]->next = NULL;
	str[0] = "export";
	str[1] = "JE_MANGE=du_pain";
	data->env = envi;
	data->command = 
	exec(data);
	return (0);
}