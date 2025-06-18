
#include "../../include/minishell.h"

void	cleanup_heredocs(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}

/*
 * Cleans up all file descriptors in command pipeline.
 * Frees fd_info structures throughout command chain.
 * Essential for preventing file descriptor leaks.
*/
void	cleanup_pipeline_fds(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->fd_info)
		{
			free(current->fd_info);
			current->fd_info = NULL;
		}
		current = current->next;
	}
}

/*
 * Cleans up all open file descriptors in command chain.
 * Handles heredoc and pipe file descriptors.
 * Essential for proper resources cleanup in pipeline execution.
*/
void	cleanup_command_fds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_fd != -1 && cmd->heredoc_fd != STDIN_FILENO)
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
		}
		if (cmd->fd_info)
		{
			if (cmd->fd_info->pipe_fd[0] != -1)
			{
				close(cmd->fd_info->pipe_fd[0]);
				cmd->fd_info->pipe_fd[0] = -1;
			}
			if (cmd->fd_info->pipe_fd[1] != -1)
				close(cmd->fd_info->pipe_fd[1]);
		}
		cmd = cmd->next;
	}
}

/*
 * Closes pipe file descriptors.
 * Resets descriptors to -1 after closing.
 * Used for single pipe cleanup.
*/
void	cleanup_pipe(t_fd_info *fd_info)
{
	if (fd_info->pipe_fd[0] != -1)
		close(fd_info->pipe_fd[0]);
	if (fd_info->pipe_fd[1] != -1)
		close(fd_info->pipe_fd[1]);
	fd_info->pipe_fd[0] = -1;
	fd_info->pipe_fd[1] = -1;
}

/*
 * Cleans up all pipes in command chain.
 * Ensures proper closure of all descriptors.
 * Prevents file descriptor leaks in pipeline.
*/
void	cleanup_all_pipes(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->fd_info)
			cleanup_pipe(current->fd_info);
		current = current->next;
	}
}
