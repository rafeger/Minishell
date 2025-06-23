/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 09:59:00 by zmurie            #+#    #+#             */
/*   Updated: 2025/06/14 09:59:02 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	input_redirection(t_data *data, t_redirect *redir, int *fd)
{
		*fd = open(redir->name, O_RDONLY);
	if (*fd == -1)
		(perror("open"), all_free(data), exit(1));
	if (*fd != STDIN_FILENO)
		(dup2(*fd, STDIN_FILENO), close(*fd));
}

static void	output_redirection(t_data *data, t_redirect *redir, int *fd)
{
	*fd = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
		(perror("open"), all_free(data), exit(1));
	if (*fd != STDOUT_FILENO)
		(dup2(*fd, STDOUT_FILENO), close(*fd));
}

static void	append(t_data *data, t_redirect *redir, int *fd)
{
	*fd = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd == -1)
		(perror("open"), all_free(data), exit(1));
	if (*fd != STDOUT_FILENO)
		(dup2(*fd, STDOUT_FILENO), close(*fd));
}

static void	here_document(t_redirect *redir, int *fd)
{
	*fd = redir->fd;
	if (*fd != STDIN_FILENO)
		(dup2(*fd, STDIN_FILENO), close(*fd));
}

void redirections(t_data *data, t_cmd_exec *cmd)
{
    	t_redir	*redir;
	int		fd;

	redir = cmd->redir_first;
	while (redir)
	{
        if (redir->type == REDIR_IN)
			input_redirection(data, redir, &fd);
		else if (redir->type == REDIR_OUT)
			output_redirection(data, redir, &fd);
		else if (redir->type == APPEND)
			append(data, redir, &fd);
		else if (redir->type == HERE_DOC)
			here_document(redir, &fd);
		redir = redir->next;
	}
}
