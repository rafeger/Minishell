#include "../../include/minishell.h"

t_cmd	*cmd_initialisation(void)
{
    t_cmd	*cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    ft_memset(cmd, 0, sizeof(t_cmd));
    cmd->tty_backup = -1;
    cmd->heredoc_fd = -1;
    
    return (cmd);
}

// int	fd_info_init(t_cmd *cmd)
// {
//     if (!cmd)
//         return (0);
//     if (cmd->fd_info)
//         return (1);
//     cmd->fd_info = malloc(sizeof(t_fd_info));
//     if (!cmd->fd_info)
//         return (0);
//     cmd->fd_info->stdin_backup = -1;
//     cmd->fd_info->stdout_backup = -1;
//     cmd->fd_info->pipe_fd[0] = -1;
//     cmd->fd_info->pipe_fd[1] = -1;
//     return (1);
// }
