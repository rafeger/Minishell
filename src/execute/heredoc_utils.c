
#include "../../include/minishell.h"

/*
 * Processes a single heredoc redirection.
 * Sets up heredoc input handling.
 * Configures stdin redirection if needed.
 * Returns 1 on error, 0 on success.
*/
int	process_heredoc(t_redirect *current, t_shell_data *sd)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredoc(current->file, current->eof_quoted, sd);
	if (heredoc_fd == -1)
		return (1);
	if (!current->next || current->next->type != 2)
	{
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
		{
			close(heredoc_fd);
			return (1);
		}
	}
	close(heredoc_fd);
	return (0);
}

/*
 * Handles a single heredoc within a command.
 * Sets up heredoc file descriptor and tracking.
 * Updates command state for heredoc presence.
 * Returns 1 on error, 0 on success.
*/
int	handle_single_heredoc(t_cmd *current, t_redirect *redir, t_shell_data *sd)
{
	int	heredoc_fd;

	if (redir->type == 2)
	{
		if (current->has_heredoc)
			close(current->heredoc_fd);
		heredoc_fd = handle_heredoc(redir->file, redir->eof_quoted, sd);
		if (heredoc_fd == -1)
			return (1);
		current->heredoc_fd = heredoc_fd;
		current->has_heredoc = 1;
	}
	return (0);
}
