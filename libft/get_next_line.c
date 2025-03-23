/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:40:26 by zmurie            #+#    #+#             */
/*   Updated: 2024/12/10 19:37:12 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*return_line(char *stash)
{
	int		i;
	int		j;
	char	*line;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = malloc((i + 2) * sizeof(char));
	else
		line = malloc((i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		line[j] = stash[j];
		j++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	int		i;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = ft_strdup(stash + i + 1);
	if (!new_stash)
	{
		free(stash);
		return (NULL);
	}
	free(stash);
	return (new_stash);
}

static char	*read_to_stash(int fd, char *stash)
{
	char *(buf) = malloc((BUFFER_SIZE + 1) * sizeof(char));
	int (bytes_read);
	if (!buf)
		return (NULL);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = '\0';
		stash = ft_strjoin2(stash, buf);
		if (!stash)
		{
			free(buf);
			return (NULL);
		}
		if (ft_strchr(stash, '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes_read < 0)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash || !*stash)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	line = return_line(stash);
	stash = update_stash(stash);
	return (line);
}
