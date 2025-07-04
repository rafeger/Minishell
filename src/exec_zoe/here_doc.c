/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:37:42 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/14 15:37:52 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static int	process_heredoc_line(char *buf, const char *delim, int fd,
	t_shell_data *data)
{
	if (g_sig == SIGINT)
	{
		free(buf);
		data->last_exit_status = 130;
		return (1);
	}
	if (!ft_strcmp(buf, delim))
	{
		free(buf);
		return (2);
	}
	buf = expand_variables(buf, data);
	ft_putstr_fd(buf, fd);
	ft_putstr_fd("\n", fd);
	free(buf);
	return (0);
}

static int	handle_heredoc_input(const char *delim, int fd, t_shell_data *data)
{
	char	*buf;
	int		status;

	while (1)
	{
		g_sig = 0;
		rl_done = 0;
		buf = readline("> ");
		if (!buf)
		{
			ft_putstr_fd("bash: warning: here-document delimited by", 2);
			ft_putstr_fd("end-of-file, wanted : ", 2);
			write(2, delim, ft_strlen(delim));
			ft_putstr_fd("\n", 2);
			break ;
		}
		status = process_heredoc_line(buf, delim, fd, data);
		if (status == 1)
			return (1);
		else if (status == 2)
			break ;
	}
	return (0);
}

static void	reset_heredoc_signals(struct sigaction *old_sa, int fd)
{
	sigaction(SIGINT, old_sa, NULL);
	rl_event_hook = NULL;
	rl_catch_signals = 1;
	close(fd);
}

static void	write_in_heredoc(int fd, char *delim, t_shell_data *data,
	char *filename)
{
	struct sigaction	old_sa;
	int					interrupted;

	setup_heredoc_signals(&old_sa);
	interrupted = handle_heredoc_input(delim, fd, data);
	reset_heredoc_signals(&old_sa, fd);
	if (interrupted)
		unlink(filename);
}

int	heredoc(char *delim, t_shell_data *data)
{
	int		fd;
	char	*filename;

	filename = generate_random_filename();
	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (-1);
	}
	write_in_heredoc(fd, delim, data, filename);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		unlink(filename);
		free(filename);
		return (-1);
	}
	unlink(filename);
	free(filename);
	return (fd);
}
