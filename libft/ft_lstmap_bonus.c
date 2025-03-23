/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:36:08 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 17:39:27 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_lstadd_back2(t_list **lst, void *content)
{
	t_list	*new_node;
	t_list	*temp;

	new_node = ft_lstnew(content);
	if (!new_node)
		return (0);
	if (*lst == NULL)
	{
		*lst = new_node;
	}
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
	return (1);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		if (!ft_lstadd_back2(&new_lst, f(lst->content)))
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (new_lst);
}

/*#include <stdio.h>

void	*double_content(void *content)
{
	int *new_content = malloc(sizeof(int));
	if (new_content)
		*new_content = *(int *)content * 2;
	return (new_content);
}

void	del(void *content)
{
	free(content);
}

int	main(void)
{
	// Création de quelques éléments de la liste
	t_list *node1 = ft_lstnew(malloc(sizeof(int)));
	*(int *)node1->content = 1;
	t_list *node2 = ft_lstnew(malloc(sizeof(int)));
	*(int *)node2->content = 2;
	t_list *node3 = ft_lstnew(malloc(sizeof(int)));
	*(int *)node3->content = 3;

	// Liaison des éléments
	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

	t_list *new_lst = ft_lstmap(node1, double_content, del);

	// Affichage des éléments de la nouvelle liste
	t_list *temp = new_lst;
	while (temp)
	{
		printf("%d\n", *(int *)temp->content);
		temp = temp->next;
	}

	// Libération de la mémoire
	ft_lstclear(&node1, free);
	ft_lstclear(&new_lst, free);

	return 0;
}*/
