
#include "../../include/minishell.h"

/*
 * Opens file with specified flags and permissions.
 * Handles error reporting for file operations.
 * Takes into account pipeline context for error display.
 * Returns file descriptor or -1 on error.
*/
static int	open_file(char *file, int flags, int mode, t_cmd *cmd)
{
	int		fd;
	char	msg[256];

	(void)cmd;
	fd = open(file, flags, mode);
	if (fd == -1)
	{
		ft_strcpy(msg, "minishell: ");
		ft_strcat(msg, file);
		if (errno == ENOENT)
			ft_strcat(msg, ": No such file or directory\n");
		else if (errno == EACCES)
			ft_strcat(msg, ": Permission denied\n");
		write(2, msg, ft_strlen(msg));
		return (-1);
	}
	return (fd);
}

/*
 * Handles input redirection (<).
 * Opens and sets up file for input.
 * Configures stdin redirection.
 * Returns 0 on success, 1 on error.
*/
static int	handle_input_redirect(char *file, t_cmd *cmd)
{
	int	fd;

	fd = open_file(file, O_RDONLY, 0, cmd);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/*
 * Manages output redirection (> and >>).
 * Handles both truncate and append modes.
 * Sets up appropriate file permissions and flags.
 * Returns 0 on success, 1 on error.
*/
static int	handle_output_redirect(char *file, int append, t_cmd *cmd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open_file(file, flags, 0644, cmd);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	return (close(fd));
}

/*
 * Processes heredoc redirections only (<<).
 * Sets up all heredocs in command.
 * Essential for proper heredoc handling order.
 * Returns 0 on success, 1 on error.
*/
int	apply_heredocs_only(t_redirect *redirects, t_shell_data *sd)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		if (current->type == 2)
		{
			if (process_heredoc(current, sd))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

/*
 * Applies all non-heredoc redirections.
 * Processes input and output redirections in order.
 * Manages file descriptor setup.
 * Returns 0 on success, 1 on error.
*/
int	apply_other_redirs(t_redirect *redirects, t_cmd *cmd)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		if (current->type == 0 && handle_input_redirect(current->file, cmd))
			return (1);
		else if ((current->type == 1 || current->type == 3) && \
				handle_output_redirect(current->file, current->type == 3, cmd))
		{
			cleanup_command_fds(cmd);
			return (1);
		}
		current = current->next;
	}
	return (0);
}
