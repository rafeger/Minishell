/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:01:39 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:10:03 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_lstsize(t_list *lst)
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

/*#include <stdio.h>

int	main(void)
{
	t_list	*node1 = ft_lstnew("First");
	t_list	*node2 = ft_lstnew("Second");
	t_list	*node3 = ft_lstnew("Third");

	// Construction de la liste
	node1->next = node2;
	node2->next = node3;

	// Calcul de la taille de la liste
	printf("Taille de la liste : %d\n", ft_lstsize(node1));

	return (0);
}*/
