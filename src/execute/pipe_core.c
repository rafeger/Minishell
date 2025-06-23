
#include "../../include/minishell.h"

/*
 * Executes series of piped commands.
 * Sets up piping between commands.
 * Manages file descriptors and process creation.
 * Coordinates entire pipeline execution process.
*/
static void	execute_piped_commands(t_cmd *cmd, t_shell_data *sd)
{
	t_cmd		*current;
	int			input_fd;
	int			*pipe_fds;

	current = cmd;
	input_fd = STDIN_FILENO;
	while (current)
	{
		pipe_fds = NULL;
		current->has_next = (current->next != NULL);
		if (current->heredoc_fd != -1)
			input_fd = current->heredoc_fd;
		if (current->has_next)
			pipe_fds = current->fd_info->pipe_fd;
		current->pid = fork_and_execute(current, sd, input_fd, pipe_fds);
		if (pipe_fds)
			input_fd = pipe_fds[0];
		current = current->next;
	}
}

/*
 * Processes exit status of child processes.
 * Handles normal exits and signal terminations.
 * Updates shell status based on last command's result.
 * Special handling for SIGQUIT in pipeline.
*/
static void	handle_pid_value(pid_t wpid, int status, t_shell_data *sd, t_cmd *cmd)
{
	if (wpid == cmd->pid)
	{
		if (WIFEXITED(status))
			sd->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sd->last_exit_status = WTERMSIG(status) + 128;
			if (WTERMSIG(status) == SIGQUIT)
			{
				ft_putendl_fd("Quit (core dumped)", 2);
				sd->sig_quit_flag = 1;
			}
		}
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT && \
			sd->sig_quit_flag == 0)
		sd->sig_quit_flag = 1;
}

/*
 * Waits for all child processes in pipeline.
 * Collects exit statuses from all commands.
 * Manages signal handling during wait.
 * Updates shell status based on last command result.
*/
static void	wait_for_children(t_shell_data *shell_data, int child_count)
{
	int		status;
	int		i;
	pid_t	wpid;
	t_cmd	*last_cmd;

	i = 0;
	last_cmd = shell_data->cmd;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	while (i < child_count)
	{
		status = 0;
		wpid = waitpid(-1, &status, 0);
		if (wpid == -1)
			break ;
		handle_pid_value(wpid, status, shell_data, last_cmd);
		i++;
	}
	shell_data->sig_quit_flag = 0;
}

/*
 * Sets up and processes heredocs for entire pipeline.
 * Prepares all heredocs before execution begins.
 * Returns 1 on heredoc error, 0 on success.
*/
static int	handle_pipeline_heredocs(t_cmd *cmd, t_shell_data *sd)
{
	t_cmd		*current;
	t_redirect	*redir;

	current = cmd;
	while (current)
	{
		if (current->redirects)
		{
			redir = current->redirects;
			while (redir)
			{
				if (handle_single_heredoc(current, redir, sd))
					return (1);
				redir = redir->next;
			}
		}
		current = current->next;
	}
	return (0);
}

/*
 * Main pipeline execution coordinator.
 * Manages complete pipeline lifecycle:
 * - Heredoc setup
 * - Pipe creation
 * - Command execution
 * - Process synchronization
 * - Resource cleanup
*/
void	execute_pipeline(t_cmd *cmd, t_shell_data *shell_data)
{
	int		child_count;
	t_cmd	*current;

	child_count = 0;
	current = cmd;
	while (current)
	{
		child_count++;
		current = current->next;
	}
	if (handle_pipeline_heredocs(cmd, shell_data))
	{
		cleanup_command_fds(cmd);
		return ;
	}
	create_all_pipes(cmd);
	execute_piped_commands(cmd, shell_data);
	wait_for_children(shell_data, child_count);
	cleanup_all_pipes(cmd);
	cleanup_pipeline_fds(cmd);
}
