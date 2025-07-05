#include "../../include/minishell.h"

volatile sig_atomic_t	g_sig = 0;

void	refresh_exit_status(t_shell_data *shdata)
{
    if (g_sig == SIGINT)
        shdata->last_exit_status = 130;
    g_sig = 0;
}

void	handle_sigint(int sig)
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

void	setup_signals(void)
{
    struct sigaction	sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}