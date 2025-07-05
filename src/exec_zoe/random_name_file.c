/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_name_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:38:10 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/05 11:38:12 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static int	get_random_int(void)
{
	int				fd;
	unsigned int	rand;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (-1);
	if (read(fd, &rand, sizeof(rand)) != sizeof(rand))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (rand);
}

static void	int_to_hex(unsigned int num, char *hex)
{
	int	i;
	int	val;

	i = 7;
	while (i >= 0)
	{
		val = num & 0xF;
		if (val < 10)
			hex[i] = '0' + val;
		else
			hex[i] = 'a' + val - 10;
		num >>= 4;
		i--;
	}
	hex[8] = '\0';
}

static int	copy_string(char *dest, const char *src, int start_pos)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[start_pos + i] = src[i];
		i++;
	}
	return (start_pos + i);
}

static void	build_filename(char *filename, const char *hex)
{
	int	pos;

	pos = 0;
	pos = copy_string(filename, "/tmp/heredoc_", pos);
	pos = copy_string(filename, hex, pos);
	pos = copy_string(filename, ".tmp", pos);
	filename[pos] = '\0';
}

char	*generate_random_filename(void)
{
	char	*filename;
	char	hex[9];
	int		rand_int;

	filename = malloc(26);
	if (!filename)
		return (NULL);
	rand_int = get_random_int();
	if (rand_int == -1)
		return (free(filename), NULL);
	int_to_hex(rand_int, hex);
	build_filename(filename, hex);
	return (filename);
}
