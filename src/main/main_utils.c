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

int	setup_heredocs(t_shell_data *data)
{
	t_cmd		*cmd;
	t_redirect	*redir;

	cmd = data->cmd;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == HERE_DOC)
			{
				cmd->heredoc_fd = heredoc(redir->file, data);
				if (cmd->heredoc_fd == -1)
				{
					if (data->last_exit_status == 130)
						return (-1);
					return (-1);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
