#include "../../include/minishell.h"

t_cmd	*cmd_initialisation(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->c_name = NULL;
	cmd->args = NULL;
	cmd->has_next = 0;
	cmd->quoted = 0;
	cmd->arg_quoted = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->fd_info = NULL;
	cmd->tty_backup = -1;
	cmd->heredoc_fd = -1;
	cmd->n_quoted = 0;
	cmd->redirects = NULL;
	cmd->arg_count = 0;
	cmd->has_heredoc = 0;
	return (cmd);
}

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
