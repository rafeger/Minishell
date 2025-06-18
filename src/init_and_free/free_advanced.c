#include "../../include/minishell.h"

/*
 * Frees command argument array.
 * Handles both individual argument strings and the array itself.
 * Properly manages the arg_count for accurate cleanup.
*/
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

/*
 * Comprehensive command structure cleanup.
 * Frees all components recursively through command pipeline:
 * - Command name and arguments
 * - Redirections
 * - Quote tracking arrays
 * - File descriptors
 * - Pipeline connections
 * Handles cleanup of entire command chain.
*/
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
		if (current->name)
			free(current->name);
		if (current->args)
			free_command_args(current);
		if (current->redirects)
			free_redirects(current->redirects);
		if (current->arg_quoted)
			free(current->arg_quoted);
		if (current->fd_info)
			free(current->fd_info);
		if (current->tty_backup != -1)
			close(current->tty_backup);
		free(current);
		current = next;
	}
}

/*
 * Complete shell cleanup function.
 * Frees all shell components:
 * - Environment variables
 * - Command structures
 * - Shell data structure itself
 * Ensures proper cleanup of all allocated shell resources.
*/
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

/*
 * Cleans up environment variable linked list.
 * Frees each node including:
 * - Key strings
 * - Value strings
 * - Node structures
 * Handles the entire environment chain.
*/
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
