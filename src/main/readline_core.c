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

void	clear_current_command(t_shell_data *shell_data)
{
    if (shell_data->cmd)
    {
        free_command(shell_data->cmd);
        shell_data->cmd = NULL;
    }
}

static void	set_syntax_error_status(t_shell_data *shell_data)
{
    shell_data->last_exit_status = 2;
}

int	syntax_checker(char *input, t_shell_data *shell_data)
{
    if (shell_syntax_check(input) != 0)
    {
        set_syntax_error_status(shell_data);
        free(input);
        return (1);
    }
    return (0);
}