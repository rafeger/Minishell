/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 09:53:58 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/17 09:54:03 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	ft_envsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	**convert_list_to_tab_str(t_env *env)
{
	char	**tab_str;
	t_env	*tmp_env;
	int		i;

	tab_str = malloc(sizeof(char *) * (ft_envsize(env) + 1));
	if (!tab_str)
		return (NULL);
	tmp_env = env;
	i = 0;
	if (tmp_env->value)
	{
		tab_str[i] = tmp_env->value;
		i++;
	}
	while (tmp_env->next)
	{
		tmp_env = tmp_env->next;
		tab_str[i] = tmp_env->value;
		i++;
	}
	tab_str[i] = NULL;
	return (tab_str);
}
char *generate_random_filename(void)
{
	int				fd;
	char			*filename;
	unsigned int	rand;
	const char		*prefix = "/tmp/heredoc_";
	char			hex[9];
	int				i;

	filename = malloc(26);
	if (!filename)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (free(filename), NULL);
	if (read(fd, &rand, sizeof(rand)) != sizeof(rand))
		return (close(fd), free(filename), NULL);
	close(fd);
	for (i = 7; i >= 0; --i)
	{
		int val = rand & 0xF;
		hex[i] = (val < 10) ? ('0' + val) : ('a' + val - 10);
		rand >>= 4;
	}
	hex[8] = '\0';
	for (i = 0; prefix[i]; i++)
		filename[i] = prefix[i];
	for (int j = 0; j < 8; j++)
		filename[i++] = hex[j];
	filename[i++] = '.';
	filename[i++] = 't';
	filename[i++] = 'm';
	filename[i++] = 'p';
	filename[i] = '\0';

	return (filename);
}
