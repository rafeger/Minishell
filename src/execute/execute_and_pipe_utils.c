
#include "../../include/minishell.h"

/*
 * Checks if command is part of a pipeline.
 * Returns 1 if command has a next command, 0 otherwise.
*/
int	is_pipeline(t_cmd *cmd)
{
	return (cmd && cmd->next);
}

/*
 * Determines if command is a builtin.
 * Checks against list of builtin commands.
 * Returns 1 if command is builtin, 0 otherwise.
*/
int	is_builtin(char *cmd_name)
{
	return (ft_strcmp(cmd_name, "cd") == 0 || \
		ft_strcmp(cmd_name, "echo") == 0 || \
		ft_strcmp(cmd_name, "env") == 0 || \
		ft_strcmp(cmd_name, "exit") == 0 || \
		ft_strcmp(cmd_name, "export") == 0 || \
		ft_strcmp(cmd_name, "pwd") == 0 || \
		ft_strcmp(cmd_name, "unset") == 0);
}
