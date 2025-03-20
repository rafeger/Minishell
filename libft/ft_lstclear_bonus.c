/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:27:29 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:41:40 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst && *lst && del)
	{
		while (*lst)
		{
			temp = (*lst)->next;
			del((*lst)->content);
			free(*lst);
			*lst = temp;
		}
		*lst = NULL;
	}
}

/*#include <stdio.h>

void	del_content(void *content)
{
	// Fonction de suppression du contenu (ici, une chaîne de caractères)
	free(content);
}

int	main(void)
{
	// Création de quelques éléments de la liste
	t_list *node1 = ft_lstnew(strdup("First"));
	t_list *node2 = ft_lstnew(strdup("Second"));
	t_list *node3 = ft_lstnew(strdup("Third"));

	// Liaison des éléments
	node1->next = node2;
	node2->next = node3;

	// Affichage avant suppression
	t_list *current = node1;
	while (current)
	{
		printf("Content: %s\n", (char *)current->content);
		current = current->next;
	}

	// Suppression de tous les éléments de la liste
	ft_lstclear(&node1, del_content);

	// Vérification si la liste est vide après la suppression
	if (node1 == NULL)
		printf("\nListe après suppression: NULL\n");

	return 0;
}*/
