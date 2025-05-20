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
#include "../../minishell.h"

void	write_in_heredoc(int fd, char *delim, t_data *data)
{
	char	*buf;

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
		ft_putstr_fd("%s\n", buf);
	}
	close(fd);

}


int	heredoc(char *delim, t_data *data)
{
	int	fd;
	open("heredoc.tmp", O_WRONLY, O_CREAT, O_TRUNC);
	if (!fd)
		return (1);
	write_in_heredoc(fd, delim, data);
	if (unlink("heredoc.tmp"))
		return (1);
	return (0);
	


}


