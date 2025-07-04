
#include "../../include/minishell.h"

void	ensure_stdin_tty(void)
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

int	has_eof_been_reached(void)
{
	char	buffer[1];
	int		bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, 0);
	return (bytes_read == 0);
}
