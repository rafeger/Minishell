/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:19:44 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:21:11 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
}

/*#include <stdio.h>

t_list	*ft_lstnew(void *content);

int	main(void)
{
	t_list	*node1 = ft_lstnew("First");
	t_list	*node2 = ft_lstnew("Second");
	t_list	*node3 = ft_lstnew("Third");

	// Construction de la liste initiale
	node1->next = node2;

	// Affichage avant d'ajouter un élément à la fin
	t_list *current = node1;
	while (current)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}

	// Ajout de l'élément à la fin
	ft_lstadd_back(&node1, node3);

	// Affichage après l'ajout
	printf("\nAprès ajout à la fin :\n");
	current = node1;
	while (current)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}

	return (0);
}*/
