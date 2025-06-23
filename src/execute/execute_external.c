
#include "../../include/minishell.h"

/*
 * Executes external command with resolved path.
 * Converts environment list to array format.
 * Handles execve call and error cases.
 * Performs cleanup and exit on execution failure.
*/
static void	execute_with_path(char *path, t_cmd *cmd, t_shell_data *sd)
{
	char	**env_array;

	env_array = env_list_to_array(sd->env);
	if (!env_array)
	{
		free(path);
		ft_cleanup_shell(&sd);
		exit(1);
	}
	if (execve(path, cmd->args, env_array) == -1)
	{
		if (errno == EACCES || errno == ENOEXEC)
		{
			free(path);
			free_env_array(env_array);
			handle_command_error(cmd, sd, 1);
		}
		free(path);
		free_env_array(env_array);
		ft_cleanup_shell(&sd);
		exit(126);
	}
}

/*
 * Processes exit status from external command.
 * Handles both normal exits and signal terminations.
 * Updates shell status appropriately.
 * Special handling for SIGQUIT signal.
*/
static void	handle_ext_pid_value(pid_t pid, int wait_status, t_shell_data *sd)
{
	waitpid(pid, &wait_status, 0);
	if (WIFSIGNALED(wait_status))
	{
		if (WTERMSIG(wait_status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", 2);
			sd->last_exit_status = WTERMSIG(wait_status) + 128;
		}
	}
	else
		sd->last_exit_status = WEXITSTATUS(wait_status);
}

/*
 * Main external command execution handler.
 * Resolves command path and validates existence.
 * Creates child process for execution.
 * Manages command status and error reporting.
*/
void	handle_external_command(t_cmd *cmd, t_shell_data *shell_data)
{
	char	*path;
	pid_t	pid;
	int		wait_status;

	wait_status = 0;
	path = find_command_path(cmd->name, shell_data->env);
	if (!path)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->name, ft_strlen(cmd->name));
		if (cmd->name[0] == '.' || cmd->name[0] == '/')
			write(2, ": No such file or directory\n", 28);
		else
			write(2, ": command not found\n", 21);
		shell_data->last_exit_status = 127;
		return ;
	}
	free(path);
	pid = fork_and_execute(cmd, shell_data, STDIN_FILENO, NULL);
	if (pid > 0)
		handle_ext_pid_value(pid, wait_status, shell_data);
}

/*
 * Core function for executing external commands.
 * Validates command arguments.
 * Checks file permissions and path validity.
 * Performs actual command execution.
*/
void	execute_external(t_cmd *cmd, t_shell_data *shell_data)
{
	char	*path;

	check_redirect_arg_error(cmd->args, shell_data);
	path = find_command_path(cmd->name, shell_data->env);
	if (!path)
		handle_command_error(cmd, shell_data, 0);
	check_file_permissions(path, cmd, shell_data);
	execute_with_path(path, cmd, shell_data);
	free(path);
}
