
#include "../../include/minishell.h"

/*
 * Executes builtin command.
 * Handles all builtin types (cd, echo, env, etc.).
 * Manages file descriptor state.
 * Returns command's exit status.
*/
int	execute_builtin(t_cmd *cmd, t_shell_data *shell_data)
{
	if (cmd->prev || cmd->next)
		backup_fds(cmd->fd_info);
	if (ft_strcmp(cmd->name, "cd") == 0)
		shell_data->last_exit_status = builtin_cd(cmd, shell_data);
	else if (ft_strcmp(cmd->name, "echo") == 0)
		shell_data->last_exit_status = builtin_echo(cmd);
	else if (ft_strcmp(cmd->name, "env") == 0)
		builtin_env(shell_data);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		shell_data->last_exit_status = builtin_exit(cmd, shell_data);
	else if (ft_strcmp(cmd->name, "export") == 0)
		builtin_export(cmd, shell_data);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		shell_data->last_exit_status = builtin_pwd(cmd);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		builtin_unset(cmd, shell_data);
	if (cmd->prev || cmd->next)
	{
		cleanup_heredocs(shell_data->cmd);
		restore_fds(cmd->fd_info, cmd);
	}
	return (shell_data->last_exit_status);
}

/*
 * Redirects I/O for child process.
 * Sets up file descriptors according to command needs.
 * Handles heredoc and pipe cleanup on error.
 * Essential for proper I/O handling in command execution.
*/
static void	tiny_child_redirect(t_cmd *cmd, t_shell_data *shell_data)
{
	t_cmd	*current;

	current = cmd;
	if (apply_other_redirs(cmd->redirects, cmd))
	{
		if (cmd->fd_info)
		{
			if (cmd->fd_info->stdin_backup != -1)
				close(cmd->fd_info->stdin_backup);
			if (cmd->fd_info->stdout_backup != -1)
				close(cmd->fd_info->stdout_backup);
		}
		while (current)
		{
			if (current->fd_info && current->fd_info->pipe_fd[0] != -1)
				close(current->fd_info->pipe_fd[0]);
			current = current->prev;
		}
		ft_cleanup_shell(&shell_data);
		exit(1);
	}
}

/*
 * Handles command execution in child process.
 * Sets up signal handling for child.
 * Manages redirections and execution type.
 * Performs cleanup and exit after execution.
*/
void	execute_child_process(t_cmd *cmd, t_shell_data *shell_data)
{
	int					exit_status;
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	if (cmd->redirects)
		tiny_child_redirect(cmd, shell_data);
	if (is_builtin(cmd->name))
	{
		exit_status = execute_builtin(cmd, shell_data);
		ft_cleanup_shell(&shell_data);
		rl_clear_history();
		exit(exit_status);
	}
	execute_external(cmd, shell_data);
	ft_cleanup_shell(&shell_data);
	rl_clear_history();
	exit(1);
}
