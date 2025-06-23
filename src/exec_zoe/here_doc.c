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

static void	write_in_heredoc(int fd, char *delim, t_shell_data *data)
{
	char	*buf;
	(void)data;

	while(1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			ft_putstr_fd("bash: warning: here-document delimited by EOF\n", 2);
			break;
		}
		if (ft_strcmp(buf, delim))
			break;
		if (!ft_strchr(buf, '$'))
			buf = expand_variables(buf, data);
		ft_putstr_fd(buf, fd);
		ft_putstr_fd("\n", fd);
	}
	free(buf);
	close(fd);

}

int	heredoc(char *delim, t_shell_data *data)
{
	int	fd;
	fd = open("heredoc.tmp", O_WRONLY, O_CREAT, O_TRUNC);
	if (!fd)
		return (-1);
	write_in_heredoc(fd, delim, data);
	if (unlink("heredoc.tmp"))
		return (-1);
	return (fd);
}


