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

int	write_in_heredoc(int fd, char *delim, t_data *data)
{
	//int		i;
	char	*buf;

	//i = 0;
	while(1)
	{




	}

}


int	heredoc(char *delim, t_data *data)
{
	int	fd;
	open("heredoc.tmp", O_WRONLY, O_CREAT, O_TRUNC);
	if (!fd)
		return (1);
	if (write_in_heredoc(fd, delim, data))
		return (1);
	if (unlink("heredoc.tmp"))
		return (1);
	return (0);
	


}


