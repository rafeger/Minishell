
#include "../../include/minishell.h"

static void	print_command_error(t_cmd *cmd, int is_permission)
{
	write(2, "minishell: ", 11);
	write(2, cmd->name, ft_strlen(cmd->name));
	if (is_permission)
		write(2, ": Permission denied\n", 20);
	else
		write(2, ": command not found\n", 21);
}

static void	cleanup_on_err(t_cmd *cmd, t_shell_data *sd, int is_permission)
{
	while (cmd && cmd->prev)
		cmd = cmd->prev;
	cleanup_heredocs(cmd);
	cleanup_command_fds(cmd);
	cleanup_all_pipes(cmd);
	ft_cleanup_shell(&sd);
	if (is_permission)
		exit(126);
	else
		exit(127);
}

/*
 * Checks for invalid redirection in command arguments.
 * Validates argument syntax.
 * Reports errors for invalid redirections.
 * Returns appropriate exit status.
*/
int	check_redirect_arg_error(char **args, t_shell_data *shell_data)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '<' || args[i][0] == '>')
		{
			write(2, "minishell:", 10);
			write(2, &args[0], 1);
			write(2, ": ", 2);
			write(2, &args[i], 1);
			write(2, ": No such file or directory\n", 28);
			shell_data->last_exit_status = 2;
			exit (1);
		}
		i++;
	}
	return (shell_data->last_exit_status);
}

/*
 * Handles command execution errors.
 * Differntiats between permission and not found errors.
 * Displays appropriate error messages.
 * Manages shell cleanup and exit status.
*/
void	handle_command_error(t_cmd *cmd, t_shell_data *sd, int is_permission)
{
	print_command_error(cmd, is_permission);
	cleanup_on_err(cmd, sd, is_permission);
}

/*
 * Validates file permissions for command execution.
 * Special handling for ./ commands.
 * Checks execute permissions and file type.
 * Reports permission errors appropriately.
*/
void	check_file_permissions(char *path, t_cmd *cmd, t_shell_data *sd)
{
	struct stat	path_stat;

	if (cmd->name[0] == '.' && cmd->name[1] == '/')
	{
		if (stat(cmd->name, &path_stat) == 0)
		{
			if (!S_ISREG(path_stat.st_mode) || access(cmd->name, X_OK) != 0)
			{
				free(path);
				handle_command_error(cmd, sd, 1);
			}
		}
	}
}
