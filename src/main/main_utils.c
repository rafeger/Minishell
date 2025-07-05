/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:59:44 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:59:45 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_heredoc_redirect(t_cmd *cmd, t_redr *redir, t_shell_data *data)
{
    int	new_fd;

    new_fd = heredoc(redir->file, data);
    if (new_fd == -1)
        return (-1);
    if (cmd->heredoc_fd != -1)
        close(cmd->heredoc_fd);
    cmd->heredoc_fd = new_fd;
    return (0);
}

static int	process_redirect_list(t_cmd *cmd, t_shell_data *data)
{
    t_redr	*redir;

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == HERE_DOC)
        {
            if (handle_heredoc_redirect(cmd, redir, data) == -1)
                return (-1);
        }
        redir = redir->next;
    }
    return (0);
}

int	setup_heredocs(t_shell_data *data)
{
    t_cmd	*cmd;

    cmd = data->cmd;
    while (cmd)
    {
        if (process_redirect_list(cmd, data) == -1)
            return (-1);
        cmd = cmd->next;
    }
    return (0);
}