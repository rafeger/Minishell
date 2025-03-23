/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:16:19 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:18:04 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
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

	// Obtenir le dernier élément
	t_list	*last = ft_lstlast(node1);
	if (last)
		printf("Dernier élément : %s\n", (char *)last->content);
	else
		printf("La liste est vide.\n");

	return (0);
}*/
