#include "../../include/minishell.h"
static char	*read_user_input(void)
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
	if (*line)
		add_history(line);
	return (line);
}

static int	shell_main_loop(t_shell_data *shell)
{
	char	*input;

	while (1)
	{
		input = read_user_input();
		if (!input)
		{
			if (!has_eof_been_reached())
				continue ;
			break ;
		}
		if (g_sig == SIGINT)
			refresh_exit_status(shell);
		if (ft_strlen(input) > 0)
			process_shell_input(input, shell);
		else
			free(input);
	}
	free(input);
	rl_clear_history();
	return (shell->last_exit_status);
}

// Add this function to your main.c or shell setup
static void setup_signal_handlers(void)
{
    struct sigaction sa;
    
    // Ignore SIGPIPE to handle broken pipes gracefully
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
}

// Modify your main function to call this
int main(int ac, char *av[], char **envp)
{
    t_shell_data    *shell;
    int             exit_status;

    (void)ac;
    (void)av;
    
    setup_signal_handlers(); // Add this line
    
    if (setup_shell(&shell, envp))
        return (1);
    exit_status = shell_main_loop(shell);
    ft_cleanup_shell(&shell);
    return (exit_status);
}
