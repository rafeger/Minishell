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

static int	should_concat(char *prev_arg, char *curr_arg)
{
	if (!prev_arg || !curr_arg || !*prev_arg || !*curr_arg)
		return (0);
	if (prev_arg[0] == '$' && (curr_arg[0] == ':' || curr_arg[0] == '$'
			|| curr_arg[0] == '='))
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1]
		== ':' && curr_arg[0] == '$')
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1]
		== '$' && curr_arg[0] == '=')
		return (1);
	if (ft_isdigit(prev_arg[0]) && curr_arg[0] == '$')
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1] == '$')
		return (1);
	return (0);
}

static void	concat_argument(t_cmd *cmd, char *arg)
{
	char	*combined_arg;
	int		last_index;

	last_index = cmd->arg_count - 1;
	combined_arg = ft_strjoin(cmd->args[last_index], arg);
	if (!combined_arg)
		return ;
	free(cmd->args[last_index]);
	cmd->args[last_index] = combined_arg;
}

void	add_argument(t_cmd *cmd, char *arg, int quoted)
{
	if (cmd->arg_count > 0 && should_concat(cmd->args[cmd->arg_count - 1], arg))
	{
		concat_argument(cmd, arg);
		return ;
	}
	cmd->args = ft_realloc(cmd->args, sizeof(char *) * (cmd->arg_count + 1), \
			sizeof(char *) * (cmd->arg_count + 2));
	cmd->arg_quoted = ft_realloc(cmd->arg_quoted, sizeof(int) * \
			(cmd->arg_count + 1), sizeof(int) * (cmd->arg_count + 2));
	if (!cmd->args || !cmd->arg_quoted)
		return ;
	cmd->args[cmd->arg_count] = ft_strdup(arg);
	cmd->arg_quoted[cmd->arg_count] = quoted;
	if (!cmd->args[cmd->arg_count])
	{
		cmd->args[cmd->arg_count] = NULL;
		return ;
	}
	cmd->args[cmd->arg_count + 1] = NULL;
	cmd->arg_count++;
	if (!cmd->c_name)
	{
		cmd->c_name = ft_strdup(arg);
		cmd->quoted = quoted;
	}
}
