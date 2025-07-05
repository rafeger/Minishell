/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:52:37 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 14:52:38 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

int	process_input_line(char *input, t_shell_data *shell)
{
	if (g_sig == SIGINT)
		refresh_exit_status(shell);
	if (ft_strlen(input) > 0)
	{
		process_shell_input(input, shell);
		return (1);
	}
	free(input);
	return (1);
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
		process_input_line(input, shell);
	}
	free(input);
	rl_clear_history();
	return (shell->last_exit_status);
}

int	main(int ac, char *av[], char **envp)
{
	t_shell_data	*shell;
	int				exit_status;

	(void)ac;
	(void)av;
	if (setup_shell(&shell, envp))
		return (1);
	exit_status = shell_main_loop(shell);
	ft_cleanup_shell(&shell);
	return (exit_status);
}
