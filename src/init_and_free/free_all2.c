/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:07:55 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:07:56 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_command_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->args)
		return ;
	while (i < cmd->arg_count)
	{
		if (cmd->args[i])
			free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
}

void	ft_cleanup_shell(t_shell_data **shell)
{
	t_env	*current;
	t_env	*next;

	if (!shell || !*shell)
		return ;
	current = (*shell)->env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	if ((*shell)->cmd)
		free_command((*shell)->cmd);
	free(*shell);
	*shell = NULL;
}

void	ft_cleanup_env(t_env **env)
{
	t_env	*current;
	t_env	*next;

	if (!env || !*env)
		return ;
	current = *env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}
