/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:49:57 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 19:50:00 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	is_builtin_no_fork(char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	handle_builtin_child(t_shell_data *data, t_cmd *cmd)
{
	int	exit_status;

	if (!ft_strcmp(cmd->args[0], "exit") || !ft_strcmp(cmd->args[0], "export"))
	{
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(0);
	}
	exit_status = do_builtin(data, cmd);
	ft_cleanup_shell(&data);
	rl_clear_history();
	exit(exit_status);
}

int	handle_fork_error(t_cmd *cmd, int pipe_fd[2], t_shell_data *data)
{
	perror("fork");
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	close_heredoc_fds(data->cmd);
	return (1);
}

void	wait_for_last_command(t_cmd *cmd, pid_t pid, int *status,
	t_shell_data *data)
{
	pid_t	wait_result;

	if (!cmd->next)
	{
		wait_result = waitpid(pid, status, 0);
		if (wait_result > 0)
		{
			if (WIFEXITED(*status))
				data->last_exit_status = WEXITSTATUS(*status);
			else if (WIFSIGNALED(*status))
				data->last_exit_status = 128 + WTERMSIG(*status);
		}
		else
		{
			data->last_exit_status = 130;
		}
	}
}
