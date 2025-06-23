#include "../../include/minishell.h"

/*
 * Handles SIGINT (Ctrl+C) signals during heredoc input.
 * Updates global signal state and manages readline behavior.
 * Ensures proper terminal display after interruption using ioctl.
*/
static void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	rl_done = 1;
	if (waitpid(-1, NULL, WNOHANG) == -1)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ioctl(STDIN_FILENO, TIOCSTI, "");
	}
}

/*
 * Sets up signal handling for heredoc operations.
 * Configures SIGINT handler specifically for heredoc context.
 * Returns 0 on successful setup.
*/
int	setup_signal_handlers(struct sigaction *sa_new, struct sigaction *sa_old)
{
	sa_new->sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_new->sa_mask);
	sa_new->sa_flags = 0;
	sigaction(SIGINT, sa_new, sa_old);
	return (0);
}

/*
 * Checks if the current input line matches the heredoc delimiter.
 * Simple utility function to improve code readability (no).
 * Returns 1 if line matches delimiter, 0 otherwise.
*/
static int	check_delimiter(char *line, char *delimiter)
{
	if (!ft_strcmp(line, delimiter))
		return (1);
	return (0);
}

/*
 * Processes a single line of heredoc input.
 * Handles variable expansion if needed and writes to heredoc file.
 * Returns 1 if delimiter is found, 0 to continue reading.
*/
static int	process_input_line(char *line, t_heredoc_data *her_data)
{
	if (check_delimiter(line, her_data->delimiter))
	{
		free(line);
		return (1);
	}
	if (her_data->expand)
		line = expand_variables(line, her_data->sd);
	write(her_data->fd, line, ft_strlen(line));
	write(her_data->fd, "\n", 1);
	free(line);
	return (0);
}

/*
 * Processes a single line of heredoc input.
 * Handles variable expansion if needed and writes to heredoc file.
 * Returns 1 if delimiter is found, 0 to continue reading.
*/
int	handle_heredoc_input(t_heredoc_data *her_data, struct sigaction *sa_old)
{
	char	*line;

	while (1)
	{
		if (g_sig == SIGINT)
		{
			sigaction(SIGINT, sa_old, NULL);
			return (-1);
		}
		line = readline("> ");
		if (!line || g_sig == SIGINT)
		{
			sigaction(SIGINT, sa_old, NULL);
			return (-1);
		}
		if (process_input_line(line, her_data))
			return (0);
	}
	return (0);
}
