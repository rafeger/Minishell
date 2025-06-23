
#include "../../include/minishell.h"

/*
 * Creates new pipe for command.
 * Initializes fd_info if needed.
 * Returns 1 on pipe creation failure, 0 on success.
*/
static int	create_pipe_for_cmd(t_cmd *cmd)
{
	if (!cmd->fd_info)
		fd_info_init(cmd);
	if (pipe(cmd->fd_info->pipe_fd) == -1)
		return (-1);
	return (0);
}

/*
 * Creates pipes for all commands in pipeline.
 * Initializes file descriptor information.
 * Essential for setting up pipeline communication.
*/
void	create_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		fd_info_init(cmd);
		if (cmd->next)
			create_pipe_for_cmd(cmd);
		cmd = cmd->next;
	}
}
