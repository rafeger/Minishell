
#include "../../include/minishell.h"

/*
 * Handles /bin/ command errors.
 * Displays appropriate error message for invalid bin paths.
 * Updates shell exit status to 127.
*/
void	handle_bin_error(t_cmd *cmd, t_shell_data *shell_data)
{
	write(2, "minishell: ", 11);
	write(2, cmd->name, ft_strlen(cmd->name));
	write(2, ": No such file or directory\n", 28);
	shell_data->last_exit_status = 127;
}

/*
 * Handles empty quotes command case.
 * Displays "command not found" error.
 * Sets exit status to 127.
*/
void	handle_empty_quotes_cmd(t_shell_data *shell_data)
{
	write(2, "minishell: : command not found\n", 31);
	shell_data->last_exit_status = 127;
}

/*
 * Processes special character commands (?, !, :).
 * Sets appropriate exit status based on character.
 * Handles error messages for special cases.
*/
void	handle_special_char_cmd(t_cmd *cmd, t_shell_data *shell_data)
{
	if (cmd->name[0] == '?')
	{
		write(2, "minishell: ", 11);
		write(2, cmd->name, ft_strlen(cmd->name));
		write(2, ": command not found\n", 20);
		shell_data->last_exit_status = 127;
	}
	if (ft_strcmp(cmd->name, "!") == 0)
		shell_data->last_exit_status = 1;
	if (ft_strcmp(cmd->name, ":") == 0)
		shell_data->last_exit_status = 0;
}

/*
 * Main invalid command detection function.
 * Checks for various invalid command types:
 * - Special characters
 * - Empty quotes
 * - Invalid bin paths
 * - Expanded variables
 * Returns 1 if command is invalid, 0 if valid.
*/
int	handle_invalid_command(t_cmd *cmd, t_shell_data *shell_data)
{
	if (!cmd || !cmd->name)
		return (0);
	if (ft_strcmp(cmd->name, "!") == 0 || ft_strcmp(cmd->name, ":") == 0 || \
			cmd->name[0] == '?')
	{
		handle_special_char_cmd(cmd, shell_data);
		return (1);
	}
	if (is_expanded_invalid_cmd(cmd->name, cmd->n_quoted, shell_data))
	{
		handle_expanded_invalid_cmd(cmd, shell_data);
		return (1);
	}
	if (ft_strcmp(cmd->name, "\"\"") == 0)
	{
		handle_empty_quotes_cmd(shell_data);
		return (1);
	}
	if (ft_strncmp(cmd->name, "/bin/", 5) == 0 && access(cmd->name, F_OK) == -1)
	{
		handle_bin_error(cmd, shell_data);
		return (1);
	}
	return (0);
}
