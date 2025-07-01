/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:49:29 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 19:49:31 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	setup_pipes_and_redirections(t_cmd *cmd, int pipe_in, int pipe_out)
{
	(void)cmd;
	if (pipe_in != STDIN_FILENO)
	{
		dup2(pipe_in, STDIN_FILENO);
		close(pipe_in);
	}
	if (pipe_out != STDOUT_FILENO)
	{
		dup2(pipe_out, STDOUT_FILENO);
		close(pipe_out);
	}
}

int	create_pipe_if_needed(t_cmd *cmd, int pipe_fd[2], int *output_fd)
{
	*output_fd = STDOUT_FILENO;
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		*output_fd = pipe_fd[1];
	}
	return (0);
}

void	handle_parent_cleanup(t_cmd *cmd, int pipe_fd[2], int *input_fd)
{
	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*input_fd = pipe_fd[0];
	}
}

void	close_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_fd != -1)
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
		}
		cmd = cmd->next;
	}
}
