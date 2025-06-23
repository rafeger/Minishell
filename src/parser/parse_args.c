#include "../../include/minishell.h"

#include "../../include/minishell.h"

/**
 * @brief Checks if two consecutive arguments should be merged into one.
 *
 * This function defines rules for concatenating arguments, often seen in shell
 * expansions where a variable might be immediately followed by a colon, another
 * variable, or an equals sign. It also handles cases where a numeric value
 * is followed by a variable, or a dollar sign is at the end of the previous argument.
 *
 * @param prev_arg The preceding argument string.
 * @param curr_arg The current argument string.
 * @return Returns 1 if concatenation is required.
 * @return Returns 0 otherwise.
 */
static int	should_concat(char *prev_arg, char *curr_arg)
{
	if (!prev_arg || !curr_arg || !*prev_arg || !*curr_arg)
		return (0);
	if (prev_arg[0] == '$' && (curr_arg[0] == ':' || curr_arg[0] == '$' || \
				curr_arg[0] == '='))
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1] == ':' && curr_arg[0] == '$')
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1] == '$' && curr_arg[0] == '=')
		return (1);
	if (ft_isdigit(prev_arg[0]) && curr_arg[0] == '$')
		return (1);
	if (ft_strlen(prev_arg) > 0 && prev_arg[ft_strlen(prev_arg) - 1] == '$')
		return (1);
	return (0);
}

/**
 * @brief Merges the current argument with the last argument in the command structure.
 *
 * This function performs the actual concatenation of two argument strings.
 * It allocates new memory for the combined string, frees the old previous argument,
 * and updates the command's argument array. This is used when `should_concat`
 * determines that arguments should logically be treated as a single unit.
 *
 * @param cmd A pointer to the command structure where arguments are stored.
 * @param arg The current argument string to be concatenated.
 */
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

/**
 * @brief Appends a new argument to the command's argument list.
 *
 * This function manages the dynamic growth of the argument array for a command.
 * It first checks if the new argument should be concatenated with the previous one.
 * If not, it reallocates memory for the argument array and the corresponding
 * quoted status array, duplicates the argument string, stores its quoted status,
 * and updates the argument count. It also assigns the first argument as the
 * command's primary name.
 *
 * @param cmd A pointer to the command structure to which the argument will be added.
 * @param arg The argument string to add.
 * @param quoted An integer indicating whether the argument was originally quoted (1) or not (0).
 */
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
	if (!cmd->name)
	{
		cmd->name = ft_strdup(arg);
		cmd->quoted = quoted;
	}
}
