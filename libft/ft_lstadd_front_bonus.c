/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stadd_front_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:47:17 by zmurie            #+#    #+#             */
/*   Updated: 2024/11/18 16:57:46 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

/*#include <stdio.h>

t_list	*ft_lstnew(void *content);
int	main(void)
{
	t_list	*head = NULL;
	t_list	*new_node1 = ft_lstnew("Hello");
	t_list	*new_node2 = ft_lstnew("World");

	ft_lstadd_front(&head, new_node1);
	ft_lstadd_front(&head, new_node2);

	// Affichage de la liste chaînée
	while (head)
	{
		printf("%s -> ", (char *)head->content);
		head = head->next;
	}
	printf("NULL\n");

	return (0);
}*/
