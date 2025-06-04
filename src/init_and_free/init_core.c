#include "../../minishell.h"

/*
 * Initializes a new command structure with all fields set to NULL/0.
 * Allocates memory for the structure and sets default values for all member
 * variables.
 * Includes initialization of file descriptors, heredoc settings, and quote
 * tracking.
 * Returns NULL if allocation fails, otherwise returns initialized structure.
*/
t_cmd	*cmd_init(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->n_quoted = 0;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->fd_info = NULL;
	cmd->tty_backup = -1;
	cmd->has_heredoc = 0;
	cmd->heredoc_fd = -1;
	cmd->has_next = 0;
	cmd->quoted = 0;
	cmd->arg_quoted = NULL;
	return (cmd);
}

/*
 * Initializes file descriptor information for a command.
 * Only allocates if fd_info doesn't already exist.
 * Sets all file descriptors to -1 as default invalid value.
 * Essential for proper pipe and redirection handling.
*/
void	fd_info_init(t_cmd *cmd)
{
	if (!cmd->fd_info)
	{
		cmd->fd_info = malloc(sizeof(t_fd_info));
		if (!cmd->fd_info)
			return ;
		cmd->fd_info->stdin_backup = -1;
		cmd->fd_info->stdout_backup = -1;
		cmd->fd_info->pipe_fd[0] = -1;
		cmd->fd_info->pipe_fd[1] = -1;
	}
}
