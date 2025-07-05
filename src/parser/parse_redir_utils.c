/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:22:19 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 13:22:20 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"	

void	attach_to_redirect_list(t_cmd *cmd, t_redirect *new_redirect)
{
	t_redirect	*tail;

	if (!cmd->redirects)
		cmd->redirects = new_redirect;
	else
	{
		tail = find_list_tail(cmd->redirects);
		tail->next = new_redirect;
	}
}

void	cmd_add_redirect(t_cmd *cmd, int type, char *file, int quoted)
{
	t_redirect	*new_redirect;

	new_redirect = create_redirect_node(type, file, quoted);
	if (!new_redirect)
		return ;
	attach_to_redirect_list(cmd, new_redirect);
}
