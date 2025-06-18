#include "../../include/minishell.h"


// /*
//  * Determines if two arguments should be concatenated.
//  * Chekcs specific patterns requiring concatenation:
//  * - Variables with special characters ($, :, =)
//  * - Numeric values with variables
//  * Returns 1 if concatenation needed, 0 otherwise.
// */
// int	should_concat(char *prev_arg, char *curr_arg)
// {
// 	if (!prev_arg || !curr_arg || !*prev_arg || !*curr_arg)
// 		return (0);
// 	if (prev_arg[0] == '$' && (curr_arg[0] == ':' || curr_arg[0] == '$' || \
// 				curr_arg[0] == '='))
// 		return (1);
// 	if (prev_arg[ft_strlen(prev_arg) - 1] == ':' && curr_arg[0] == '$')
// 		return (1);
// 	if (prev_arg[ft_strlen(prev_arg) - 1] == '$' && curr_arg[0] == '=')
// 		return (1);
// 	if (ft_isdigit(prev_arg[0]) && curr_arg[0] == '$')
// 		return (1);
// 	if (prev_arg[ft_strlen(prev_arg) - 1] == '$')
// 		return (1);
// 	return (0);
// }
static int	concat_check(const char *prev_token, const char *curr_token)
{
	size_t	prev_len;

	if (!prev_token || !curr_token || !*prev_token || !*curr_token)
		return (0);

	prev_len = ft_strlen(prev_token);
	if (prev_token[0] == '$' && (curr_token[0] == ':' 
		|| curr_token[0] == '$' || curr_token[0] == '='))
		return (1);
	if (prev_len > 0 && prev_token[prev_len - 1] == ':' && curr_token[0] == '$')
		return (1);
	if (prev_len > 0 && prev_token[prev_len - 1] == '$' && curr_token[0] == '=')
		return (1);
	if (ft_isdigit(prev_token[0]) && curr_token[0] == '$')
		return (1);
	if (prev_len > 0 && prev_token[prev_len - 1] == '$')
		return (1);

	return (0);
}


/*
 * Concatenates current argument with previous argument.
 * Handles memory management for concatenated string.
 * Updates command's argument array with new concatenated value.
*/
void	concat_argument(t_cmd *cmd, char *arg)
{
	char	*new_arg;
	int		last_idx;

	last_idx = cmd->arg_count - 1;
	new_arg = ft_strjoin(cmd->args[last_idx], arg);
	if (!new_arg)
		return ;
	free(cmd->args[last_idx]);
	cmd->args[last_idx] = new_arg;
}

/*
 * Adds new argument to command structure.
 * Handles memory reallocation for argument array growth.
 * Manages command name assignment for first argument.
 * Tracks quote status for each argument.
*/
void	add_argument(t_cmd *cmd, char *arg, int quoted)
{
	if (cmd->arg_count > 0 && concat_check(cmd->args[cmd->arg_count - 1], arg))
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
	if (!cmd->name)
	{
		cmd->name = ft_strdup(arg);
		cmd->quoted = quoted;
	}
}

void	add_argument(t_cmd *cmd, char *arg_val, int is_quoted)
{
	if (cmd->arg_count > 0 && concat_check(cmd->args[cmd->arg_count - 1], arg_val))
	{
		if (!s_try_concat_arg(cmd, arg_val))
			return ;
		return ;
	}
	if (!s_expand_arg_arrays(cmd))
	{
		return ;
	}
	cmd->args[cmd->arg_count] = ft_strdup(arg_val);
	if (!cmd->args[cmd->arg_count])
		return ;
	cmd->arg_quoted[cmd->arg_count] = is_quoted;
	cmd->arg_count++;
	if (!cmd->name)
	{
		cmd->name = ft_strdup(arg_val);
		if (!cmd->name)
			return ;
		cmd->quoted = is_quoted;
	}
}
