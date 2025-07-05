#include "../../include/minishell.h"

void	merge_with_last_argument(t_cmd *cmd, char *arg)
{
	char	*combined_arg;
	int		last_index;

	last_index = cmd->arg_count - 1;
	combined_arg = ft_strjoin(cmd->args[last_index], arg);
	if (!combined_arg)
		return ;
	free(cmd->args[last_index]);
	cmd->args[last_index] = combined_arg;
}

int	expand_argument_storage(t_cmd *cmd)
{
	cmd->args = ft_realloc(cmd->args, sizeof(char *) * (cmd->arg_count + 1),
			sizeof(char *) * (cmd->arg_count + 2));
	cmd->arg_quoted = ft_realloc(cmd->arg_quoted, sizeof(int) *
			(cmd->arg_count + 1), sizeof(int) * (cmd->arg_count + 2));
	return (cmd->args && cmd->arg_quoted);
}

void	store_new_argument(t_cmd *cmd, char *arg, int quoted)
{
	cmd->args[cmd->arg_count] = ft_strdup(arg);
	cmd->arg_quoted[cmd->arg_count] = quoted;
	if (!cmd->args[cmd->arg_count])
	{
		cmd->args[cmd->arg_count] = NULL;
		return ;
	}
	cmd->args[cmd->arg_count + 1] = NULL;
	cmd->arg_count++;
}

void	update_command_name_if_needed(t_cmd *cmd, char *arg, int quoted)
{
	if (!cmd->c_name)
	{
		cmd->c_name = ft_strdup(arg);
		cmd->quoted = quoted;
	}
}
