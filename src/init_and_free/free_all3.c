/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:25:42 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:25:43 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_command_resources(t_cmd *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->tty_backup != -1)
		close(cmd->tty_backup);
}

static void	cleanup_command_data(t_cmd *cmd)
{
	if (cmd->c_name)
		free(cmd->c_name);
	if (cmd->args)
		free_command_args(cmd);
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	if (cmd->arg_quoted)
		free(cmd->arg_quoted);
}

static void	free_single_command(t_cmd *cmd)
{
	cleanup_command_resources(cmd);
	cleanup_command_data(cmd);
	free(cmd);
}

void	free_command(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		next = current->next;
		free_single_command(current);
		current = next;
	}
}
