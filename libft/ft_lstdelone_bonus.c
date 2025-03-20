/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:22:59 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/19 09:51:30 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst && del)
	{
		del(lst->content);
		free(lst);
	}
}

/*void	del_content(void *content)
{
	// Fonction de suppression du contenu
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

	// Suppression de l'élément node2
	ft_lstdelone(node2, del_content);

	// Affichage après suppression
	printf("\nAprès suppression de node2:\n");
	current = node1;
	while (current)
	{
		printf("Content: %s\n", (char *)current->content);
		current = current->next;
	}

	// Libération de la mémoire restante
	ft_lstdelone(node1, del_content);
	ft_lstdelone(node3, del_content);

	return 0;
}*/
