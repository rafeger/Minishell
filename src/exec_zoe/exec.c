/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:09:47 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 18:09:49 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

pid_t	g_signal;

static void	child_process(t_shell_data *data, t_cmd *cmd,
	int pipe_in, int pipe_out)
{
	setup_pipes_and_redirections(cmd, pipe_in, pipe_out);
	redirections(data, cmd);
	if (is_builtin(cmd->args[0]))
		handle_builtin_child(data, cmd);
	else
		execute(data, cmd);
}

static int	execute_single_command(t_cmd *cmd, int *input_fd,
	t_shell_data *data, int *status)
{
	int		pipe_fd[2];
	int		output_fd;
	pid_t	pid;

	if (create_pipe_if_needed(cmd, pipe_fd, &output_fd))
	{
		close_heredoc_fds(data->cmd);
		return (1);
	}
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd, pipe_fd, data));
	if (pid == 0)
	{
		if (cmd->next)
			close(pipe_fd[0]);
		child_process(data, cmd, *input_fd, output_fd);
	}
	else
	{
		handle_parent_cleanup(cmd, pipe_fd, input_fd);
		wait_for_last_command(cmd, pid, status, data);
	}
	return (0);
}

static int	execute_command_loop(t_shell_data *data)
{
	t_cmd	*cmd;
	int		input_fd;
	int		status;

	cmd = data->cmd;
	input_fd = STDIN_FILENO;
	status = 0;
	while (cmd)
	{
		if (execute_single_command(cmd, &input_fd, data, &status))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

static int	handle_single_builtin_and_just_redir(t_shell_data *data, t_cmd *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	if (!cmd->args && redir)
		return (just_redir(data, redir));
	if (!cmd->next && is_builtin_no_fork(cmd->args[0]))
	{
		redirections(data, cmd);
		do_builtin(data, cmd);
		close_heredoc_fds(data->cmd);
		return (1);
	}
	return (0);
}

int	execute_commands(t_shell_data *data)
{
	int	input_fd;
	int	result;

	input_fd = STDIN_FILENO;
	if (handle_single_builtin_and_just_redir(data, data->cmd))
		return (0);
	result = execute_command_loop(data);
	while (wait(NULL) > 0)
		;
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	close_heredoc_fds(data->cmd);
	return (result);
}
