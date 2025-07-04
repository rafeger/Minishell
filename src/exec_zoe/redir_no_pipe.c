/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_no_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:03 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/04 13:02:22 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	print_error_opening_file(char *name_file)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(name_file, 2);
	perror("");
	return (1);
}

static int	handle_file_redirection(t_redirect *redir)
{
	int	fd;

	if (redir->type == APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else
		return (0);
	if (fd == -1)
		return (print_error_opening_file(redir->file));
	close(fd);
	return (0);
}

int	just_redir(t_shell_data *data, t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == HERE_DOC && data->cmd->heredoc_fd != -1)
		{
			close(data->cmd->heredoc_fd);
			data->cmd->heredoc_fd = -1;
		}
		else
		{
			if (handle_file_redirection(redir) != 0)
				return (1);
		}
		redir = redir->next;
	}
	close_heredoc_fds(data->cmd);
	data->last_exit_status = 0;
	return (1);
}
