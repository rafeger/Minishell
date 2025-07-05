/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:06:32 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 10:06:33 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

void	add_arg(t_cmd *cmd, char *arg, int quoted)
{
    if (cmd->arg_count > 0 && should_merge_arguments(cmd->args[cmd->arg_count - 1], arg))
    {
        merge_with_last_argument(cmd, arg);
        return ;
    }
    if (!expand_argument_storage(cmd))
        return ;
    store_new_argument(cmd, arg, quoted);
    update_command_name_if_needed(cmd, arg, quoted);
}
