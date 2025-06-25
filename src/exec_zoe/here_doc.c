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

	while(1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			ft_putstr_fd("bash: warning: here-document delimited by EOF\n", 2);
			break;
		}
		if (!ft_strcmp(buf, delim))
		{
			free(buf);
			break;
		}
		buf = expand_variables(buf, data);
		ft_putstr_fd(buf, fd);
		ft_putstr_fd("\n", fd);
		free(buf);
	}
	close(fd);
}

int heredoc(char *delim, t_shell_data *data)
{
    int fd;
    
    // Correction : utiliser les flags correctement combinés avec |
    fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)  // Correction : vérifier fd == -1, pas !fd
        return (-1);
    
    write_in_heredoc(fd, delim, data);
    close(fd);  // Fermer le fd d'écriture
    
    // Rouvrir le fichier en lecture
    fd = open("heredoc.tmp", O_RDONLY);
    if (fd == -1)
    {
        unlink("heredoc.tmp");  // Nettoyer en cas d'erreur
        return (-1);
    }
    
    unlink("heredoc.tmp");  // Supprimer le fichier (il reste accessible via fd)
    return (fd);
}


