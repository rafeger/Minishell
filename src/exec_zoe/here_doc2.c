/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:24:45 by zmurie            #+#    #+#             */
/*   Updated: 2025/07/01 18:24:47 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
}

int	check_signal_callback(void)
{
	if (g_sig == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

void	setup_heredoc_signals(struct sigaction *old_sa)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, old_sa);
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	rl_event_hook = check_signal_callback;
	rl_catch_signals = 0;
}
