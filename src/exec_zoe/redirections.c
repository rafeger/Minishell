/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:08:15 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 18:08:18 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	input_redirection(t_shell_data *data, t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		print_error_opening_file(redir->file);
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_cleanup_shell(&data);
		rl_clear_history();
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

static void	output_redirection(t_shell_data *data, t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error_opening_file(redir->file);
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_cleanup_shell(&data);
		rl_clear_history();
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

static void	append_redirection(t_shell_data *data, t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error_opening_file(redir->file);
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_cleanup_shell(&data);
		rl_clear_history();
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

static void	here_document(t_cmd *cmd, t_shell_data *data)
{
	if (cmd->heredoc_fd == -1)
		return ;
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(cmd->heredoc_fd);
		ft_cleanup_shell(&data);
		rl_clear_history();
		exit(1);
	}
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
}

void	redirections(t_shell_data *data, t_cmd *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			input_redirection(data, redir);
		else if (redir->type == REDIR_OUT)
			output_redirection(data, redir);
		else if (redir->type == APPEND)
			append_redirection(data, redir);
		else if (redir->type == HERE_DOC)
			here_document(cmd, data);
		redir = redir->next;
	}
}
