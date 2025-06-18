
#include "../../include/minishell.h"

/*
 * Checks if command has non-heredoc redirections.
 * Scans redirection list for input/output redirections.
 * Returns 1 if regular redirections found, 0 if only heredocs
 * if no redirections.
 * Used to determine execution strategy for builtin.
*/
static int	has_classic_redirs(t_redirect *redir)
{
	t_redirect	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->type != 2)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*
 * Manages builtin command execution.
 * Handles redirections appropriately for builtins.
 * Creates child process if necessary for redirections.
 * Updates shell exit status based on builtin result.
*/
static void	handle_builtin_execution(t_cmd *cmd, t_shell_data *sd)
{
	pid_t	pid;
	int		status;

	if (!has_classic_redirs(cmd->redirects))
	{
		sd->last_exit_status = execute_builtin(cmd, sd);
		if (cmd->fd_info)
			restore_fds(cmd->fd_info, cmd);
		return ;
	}
	else
	{
		pid = fork_and_execute(cmd, sd, STDIN_FILENO, NULL);
		if (pid > 0)
		{
			status = 0;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				sd->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				sd->last_exit_status = WTERMSIG(status) + 128;
		}
		if (cmd->fd_info)
			restore_fds(cmd->fd_info, cmd);
	}
}

/*
 * Handles execution of a single command.
 * Determines appropriate execution path:
 * - Pipeline handling
 * - Builtin execution
 * - External command execution
 * Manages command state and exit status.
*/
static void	handle_command_execution(t_cmd *cmd, t_shell_data *sd)
{
	if (is_pipeline(cmd))
	{
		execute_pipeline(cmd, sd);
		if (cmd->fd_info)
			restore_fds(cmd->fd_info, cmd);
		return ;
	}
	if (is_builtin(cmd->name))
	{
		handle_builtin_execution(cmd, sd);
		return ;
	}
	else if (cmd->name)
		handle_external_command(cmd, sd);
	else
		sd->last_exit_status = 0;
}

/*
 * Processes heredocs for parent process.
 * Sets up heredoc file descriptors.
 * Returns 1 on heredoc error, 0 on success.
 * Essential for proper heredoc handling before execution.
*/
static int	handle_heredocs_parent(t_cmd *cmd, t_shell_data *shell_data)
{
	if (cmd->redirects)
	{
		if (apply_heredocs_only(cmd->redirects, shell_data) != 0)
		{
			shell_data->last_exit_status = 0;
			return (1);
		}
	}
	return (0);
}

/*
 * Main command execution coordinator.
 * Validates command and handles special cases.
 * Sets up file descriptors and execution environment.
 * Manages complete command execution lifecycle.
*/
void	execute_commands(t_cmd *cmd, t_shell_data *shell_data)
{
	if (!cmd || !shell_data || handle_invalid_command(cmd, shell_data) || \
			handle_special_cases(cmd, shell_data))
		return ;
	fd_info_init(cmd);
	if (!cmd->fd_info)
		return ;
	if (!is_pipeline(cmd) && !is_builtin(cmd->name))
	{
		backup_fds(cmd->fd_info);
		if (handle_heredocs_parent(cmd, shell_data))
		{
			restore_fds(cmd->fd_info, cmd);
			return ;
		}
		handle_command_execution(cmd, shell_data);
		restore_fds(cmd->fd_info, cmd);
	}
	else
		handle_command_execution(cmd, shell_data);
}
