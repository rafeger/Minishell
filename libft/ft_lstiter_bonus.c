/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:32:45 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:34:25 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

/*#include <stdio.h>

void	print_content(void *content)
{
	// Supposons que le contenu soit une chaîne de caractères (char *)
	printf("%s\n", (char *)content);
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

	// Application de la fonction 'print_content' à chaque élément de la liste
	ft_lstiter(node1, print_content);

	// Libération de la mémoire (après utilisation)
	ft_lstclear(&node1, free);

	return 0;
}*/
