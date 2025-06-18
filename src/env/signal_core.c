#include "../../include/minishell.h"

volatile sig_atomic_t	g_sig = 0;

/*
 * Updates shell's exit status based on received signal.
 * Specifically handles SIGINT (Ctrl-C) with status 130.
 * Resets global signal flag after processing.
*/
void	update_exit_status(t_shell_data *shell_data)
{
	if (g_sig == SIGINT)
		shell_data->last_exit_status = 130;
	g_sig = 0;
}

/*
 * Handles SIGINT (Ctrl-C) signal.
 * - Updates global signal flag.
 * - Handles terminal display appropriately.
 * - Manages different contexts (prompt vs running command).
 * Essential for proper interrupt handling.
*/
void	sigint_handler(int sig)
{
	g_sig = sig;
	if (waitpid(-1, NULL, WNOHANG) == -1)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		ft_putchar_fd('\n', 1);
}

/*
 * Sets up signal handlers for shell:
 * - SIGINT (Ctrl-C) with custom handler.
 * - SIGQUIT (Ctrl-\) ignored.
 * Uses sigaction for reliable signal handling.
*/
void	init_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
