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
	int			new_fd;

	cmd = data->cmd;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == HERE_DOC)
			{
				new_fd = heredoc(redir->file, data);
				if (new_fd == -1)
					return (-1);
				if (cmd->heredoc_fd != -1)
					close(cmd->heredoc_fd);
				cmd->heredoc_fd = new_fd;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
