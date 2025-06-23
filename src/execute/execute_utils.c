
#include "../../include/minishell.h"

/*
 * Checks if command path matches home directory.
 * Updates shell status and displays error if match found.
 * Used to prevent direct execution of home directory.
*/
static void	handle_home_directory(t_cmd *cmd, t_shell_data *shell_data)
{
	char	*path;

	shell_data->last_exit_status = 0;
	path = get_env_value(shell_data->env, "HOME");
	if (path && ft_strcmp(cmd->name, path) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->name, ft_strlen(cmd->name));
		write(2, ": Is a directory\n", 17);
		shell_data->last_exit_status = 126;
		return ;
	}
}

/*
 * Handles special command cases.
 * Checks for:
 * - Home directory execution
 * - Dot commands (. and ..)
 * - Directory path execution
 * Returns 1 if special case handled, 0 otherwise.
*/
int	handle_special_cases(t_cmd *cmd, t_shell_data *shell_data)
{
	handle_home_directory(cmd, shell_data);
	if (shell_data->last_exit_status == 126)
		return (1);
	if (is_dot_command(cmd->name))
	{
		handle_dot_command(cmd->name, shell_data);
		return (1);
	}
	if (check_directory_path(cmd->name))
	{
		shell_data->last_exit_status = 126;
		return (1);
	}
	return (0);
}
