
#include "../../include/minishell.h"

/*
 * Creates unique temporary file name for heredoc.
 * Uses incremental counter for uniqueness.
 * Returns malloced string with file path.
*/
static char	*create_temp_file(void)
{
	static int	count;
	char		*name;
	char		*num;
	char		*temp;

	count = 0;
	num = ft_itoa(count++);
	if (!num)
		return (NULL);
	temp = ft_strjoin("/tmp/heredoc_", num);
	free(num);
	name = ft_strdup(temp);
	free(temp);
	return (name);
}

/*
 * Writes heredoc content to file descriptor.
 * Handles expansion if needed.
 * Processes input until delimiter is found.
 * Returns 0 on success, -1 on EOF or error.
*/
static int	write_heredoc_content(int fd, char *delimiter, int expand, \
				t_shell_data *sd)
{
	struct sigaction	sa_old;
	struct sigaction	sa_new;
	t_heredoc_data		her_data;
	int					result;

	her_data.fd = fd;
	her_data.delimiter = delimiter;
	her_data.expand = expand;
	her_data.sd = sd;
	setup_signal_handlers(&sa_new, &sa_old);
	result = handle_heredoc_input(&her_data, &sa_old);
	sigaction(SIGINT, &sa_old, NULL);
	return (result);
}

/*
 * Creates and writes heredoc temporary file.
 * Manages file creation and writing process.
 * Handles cleanup on failure.
 * Returns 0 on success, -1 on error.
*/
static int	create_and_write_heredoc(char *del, int eof_quoted, char **temp, \
				t_shell_data *sd)
{
	int	fd;

	*temp = create_temp_file();
	if (!*temp)
		return (-1);
	fd = open(*temp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(*temp);
		return (-1);
	}
	if (write_heredoc_content(fd, del, !eof_quoted, sd) == -1)
	{
		close(fd);
		unlink(*temp);
		free(*temp);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * Main heredoc handling function.
 * Coordinates complete heredoc setup:
 * - Creates temp file
 * - Handles content writing
 * - Manages file cleanup
 * Returns file descriptor for reading heredoc content.
*/
int	handle_heredoc(char *delimiter, int eof_quoted, t_shell_data *sd)
{
	t_heredoc_data	*data;
	int				ret_fd;

	data = malloc(sizeof(t_heredoc_data));
	if (!data)
		return (-1);
	if (create_and_write_heredoc(delimiter, eof_quoted, &data->temp, sd) == -1)
	{
		free(data);
		return (-1);
	}
	data->fd = open(data->temp, O_RDONLY);
	ret_fd = data->fd;
	if (data->fd == -1)
	{
		free(data->temp);
		free(data);
		return (-1);
	}
	unlink(data->temp);
	free(data->temp);
	free(data);
	return (ret_fd);
}
