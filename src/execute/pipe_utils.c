
#include "../../include/minishell.h"

/*
 * Saves copies of standard input/output.
 * Essential for restoring after redirection.
*/
void	backup_fds(t_fd_info *fd_info)
{
	fd_info->stdin_backup = dup(STDIN_FILENO);
	fd_info->stdout_backup = dup(STDOUT_FILENO);
}

/*
 * Restores original standard input/output.
 * Handles heredoc cases separately.
 * Closes backup descriptors after restoration.
*/
void	restore_fds(t_fd_info *fd_info, t_cmd *cmd)
{
	if (!fd_info)
		return ;
	if (!cmd->has_heredoc && fd_info->stdin_backup != -1)
	{
		dup2(fd_info->stdin_backup, STDIN_FILENO);
		close(fd_info->stdin_backup);
	}
	if (fd_info->stdout_backup != -1)
	{
		dup2(fd_info->stdout_backup, STDOUT_FILENO);
		close(fd_info->stdout_backup);
	}
}
