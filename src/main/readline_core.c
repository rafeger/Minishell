/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:59:47 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:59:53 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static void	clear_current_command(t_shell_data *shell_data)
{
	if (shell_data->cmd)
	{
		free_command(shell_data->cmd);
		shell_data->cmd = NULL;
	}
}

static int	syntax_checker(char *input, t_shell_data *shell_data)
{
	if (shell_syntax_check(input) != 0)
	{
		shell_data->last_exit_status = 2;
		free(input);
		return (1);
	}
	return (0);
}

// print_expand_debug(input, expanded_input);
// print_token_list(t_a);
static t_cmd	*cmd_prep(char *input, t_shell_data *shell_data)
{
	char	*expanded_input;
	t_ta	*t_a;
	t_cmd	*cmd;
	int		is_empty;

	expanded_input = expand_variables(input, shell_data);
	if (!expanded_input)
		return (NULL);
	is_empty = (expanded_input[0] == '\0');
	t_a = lexer(expanded_input, shell_data);
	free(expanded_input);
	if (!t_a && is_empty)
	{
		shell_data->last_exit_status = 0;
		return (NULL);
	}
	if (!t_a)
		return (NULL);
	cmd = parse_tokens(t_a);
	free_tokenarray(t_a);
	return (cmd);
}

static void	exec_cmd(t_cmd *cmd, t_shell_data *sd)
{
	if (cmd)
	{
		sd->cmd = cmd;
		if (setup_heredocs(sd) == -1)
		{
			free_command(cmd);
			sd->cmd = NULL;
			return ;
		}
		execute_commands(sd);
	}
}

// print_cmd_args(cmd);
void	process_shell_input(char *input, t_shell_data *shell_data)
{
	t_cmd	*cmd;

	if (!input)
		exit(0);
	if (syntax_checker(input, shell_data))
		return ;
	clear_current_command(shell_data);
	cmd = cmd_prep(input, shell_data);
	free(input);
	exec_cmd(cmd, shell_data);
}
