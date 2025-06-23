
#include "../../include/minishell.h"

/*
 * Sets up pipe file descriptors for command.
 * Configures output redirection through pipe.
 * Closes unused pipe ends.
 * Essential for pipeline communication setup.
*/
static void	handle_pipe_fds(int *pipe_fd)
{
	if (!pipe_fd)
		return ;
	if (pipe_fd[1] != STDOUT_FILENO)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fd[1]);
	}
	close(pipe_fd[0]);
}

/*
 * Configures input file descriptor for command.
 * Redirects stdin if needed.
 * Closes original fd after duplication.
 * Ensures proper input handling for commands.
*/
static void	handle_input_fd(int input_fd)
{
	if (input_fd == STDIN_FILENO)
		return ;
	if (dup2(input_fd, STDIN_FILENO) == -1)
		exit(1);
	close(input_fd);
}

/*
 * Main fork and execution corredination function.
 * Creates child process and sets up I/O.
 * Manages pipe and input/output configuration.
 * Returns pid of created process or 0 on failure.
*/
int	fork_and_execute(t_cmd *cmd, t_shell_data *sd, int input_fd, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		handle_pipe_fds(pipe_fd);
		handle_input_fd(input_fd);
		execute_child_process(cmd, sd);
	}
	if (pid < 0)
	{
		perror("fork");
		return (0);
	}
	if (!pipe_fd && WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		ft_putendl_fd("Quit (core dumped)", 2);
	if (pipe_fd && pipe_fd[1] != STDOUT_FILENO)
		close(pipe_fd[1]);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (pid);
}
