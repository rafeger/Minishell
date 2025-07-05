/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:55:53 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/05 09:55:55 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	*realloc_new_allocation(size_t newsize)
{
	void	*newptr;

	newptr = malloc(newsize);
	if (newptr)
		ft_memset(newptr, 0, newsize);
	return (newptr);
}

void	*ft_realloc(void *ptr, size_t oldsize, size_t newsize)
{
	void	*newptr;
	size_t	copysize;

	if (!ptr)
		return (realloc_new_allocation(newsize));
	if (newsize == 0)
	{
		free(ptr);
		return (NULL);
	}
	newptr = malloc(newsize);
	if (!newptr)
		return (NULL);
	copysize = oldsize;
	if (newsize < oldsize)
		copysize = newsize;
	ft_memcpy(newptr, ptr, copysize);
	if (newsize > oldsize)
		ft_memset((char *)newptr + oldsize, 0, newsize - oldsize);
	free(ptr);
	return (newptr);
}
