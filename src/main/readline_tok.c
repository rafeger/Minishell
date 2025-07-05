/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tok.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:52:49 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 14:52:51 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static int	handle_empty_expansion(t_shell_data *shell_data)
{
	shell_data->last_exit_status = 0;
	return (1);
}

t_ta	*tokenize_expanded_input(char *expanded_input, t_shell_data *shell_data)
{
	t_ta	*t_a;
	int		is_empty;

	is_empty = (expanded_input[0] == '\0');
	t_a = lexer(expanded_input, shell_data);
	if (!t_a && is_empty)
	{
		handle_empty_expansion(shell_data);
		return (NULL);
	}
	return (t_a);
}

// print_expand_debug(input, expanded_input);
// print_token_list(t_a);
t_cmd	*cmd_prep(char *input, t_shell_data *shell_data)
{
	char	*expanded_input;
	t_ta	*t_a;
	t_cmd	*cmd;

	expanded_input = expand_variables(input, shell_data);
	if (!expanded_input)
		return (NULL);
	t_a = tokenize_expanded_input(expanded_input, shell_data);
	free(expanded_input);
	if (!t_a)
		return (NULL);
	cmd = parse_tokens(t_a);
	free_tokenarray(t_a);
	return (cmd);
}

int	setup_command_heredocs(t_shell_data *sd, t_cmd *cmd)
{
	sd->cmd = cmd;
	if (setup_heredocs(sd) == -1)
	{
		free_command(cmd);
		sd->cmd = NULL;
		return (0);
	}
	return (1);
}

void	exec_cmd(t_cmd *cmd, t_shell_data *sd)
{
	if (cmd)
	{
		if (setup_command_heredocs(sd, cmd))
			execute_commands(sd);
	}
}
