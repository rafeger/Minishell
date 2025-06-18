
#include "../../include/minishell.h"

/*
 * Verifies if stdin is properly configured as a terminal.
 * If not, attempts to reopen /dev/tty and configures it as stdin.
 * Essential for proper readline functionality.
*/
void	check_and_restore_stdin(void)
{
	int	fd;

	if (isatty(STDIN_FILENO) == 0)
	{
		fd = open("/dev/tty", O_RDONLY);
		if (fd == -1)
			exit(1);
		if (dup2(fd, STDIN_FILENO) == -1)
			exit(1);
		close(fd);
	}
}

/*
 * Tests if end-of-file has been reached on stdin.
 * Performs a zero-length read to check strem status.
 * Returns 1 if EOF is detected, 0 otherwise.
*/
int	is_eof_reached(void)
{
	char	buffer[1];
	int		bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, 0);
	return (bytes_read == 0);
}
