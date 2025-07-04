/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfranke <pfranke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 02:03:55 by pfranke           #+#    #+#             */
/*   Updated: 2025/03/13 02:04:57 by pfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dollars.h"

int	dl_getsize(t_dl *dl)
{
	int	size;

	size = 0;
	if (dl->str == NULL || dl->env == NULL)
		return (0);
	dl->i[0] = 0;
	dl->i[2] = -1;
	dl->i[3] = -1;
	while (dl->str[dl->i[0]])
	{
		handle_quotes(dl);
		if (dl->str[dl->i[0]] == '$' && dl->i[2] == -1)
			size += handle_dollar(dl);
		else
		{
			size++;
			dl->i[0]++;
		}
	}
	return (size);
}

void	append_and_update(t_dl *dl, char *src)
{
	ft_strlcat(dl->new, src, dl->total_size + 1);
	dl->i[1] += ft_strlen(src);
}
