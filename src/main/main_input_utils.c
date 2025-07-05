#include "../../include/minishell.h"

static int	setup_terminal_fd(void)
{
    int	fd;

    fd = open("/dev/tty", O_RDONLY);
    if (fd == -1)
        exit(1);
    if (dup2(fd, STDIN_FILENO) == -1)
        exit(1);
    close(fd);
    return (1);
}

void	ensure_stdin_tty(void)
{
    if (isatty(STDIN_FILENO) == 0)
        setup_terminal_fd();
}

int	has_eof_been_reached(void)
{
    char	buffer[1];
    int		bytes_read;

    bytes_read = read(STDIN_FILENO, buffer, 0);
    return (bytes_read == 0);
}

static int	should_add_to_history(char *line)
{
    return (line && *line);
}

char	*read_user_input(void)
{
    char	*line;

    ensure_stdin_tty();
    line = readline(PROMPT);
    if (!line)
    {
        if (read(STDIN_FILENO, NULL, 0) == 0)
            printf("exit\n");
        return (NULL);
    }
    if (should_add_to_history(line))
        add_history(line);
    return (line);
}