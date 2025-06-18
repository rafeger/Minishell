
#include "../../include/minishell.h"

/*
 * Checks if command is wrapped in single quotes.
 * Helper function for quote detection.
*/
static int	is_single_quoted(const char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (cmd[0] == '\'' && cmd[ft_strlen(cmd) - 1] == '\'')
		return (1);
	return (0);
}

/*
 * Checks if command is wrapped in double quotes.
 * Helper function for quote detection.
*/
static int	is_double_quoted(const char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (cmd[0] == '"' && cmd[ft_strlen(cmd) - 1] == '"')
		return (1);
	return (0);
}

/*
 * Checks if command is from invalid expansion.
 * Verifies if expanded variable results in invalid command.
 * Takes quote context into account.
 * Returns 1 if command is invalid expanded variable, 0 otherwise.
*/
int	is_expanded_invalid_cmd(const char *name, int quoted, t_shell_data *sd)
{
	char	*env_var;
	char	*env_value;
	int		i;

	i = 0;
	if (!name || !quoted)
		return (0);
	while (name[i] && (ft_isalnum(name[i]) || name[i] == '_' || name[i] == '$'))
		i++;
	env_var = ft_substr(name, 1, i - 1);
	if (!env_var)
		return (0);
	env_value = get_env_value(sd->env, env_var);
	free(env_var);
	if (!env_value)
		return (1);
	if (!name[i])
		return (quoted);
	return (0);
}

/*
 * Handles execution of invalid expanded commands.
 * Different handling for single and double quoted expansions.
 * Updates shell ecit status appropriately.
 * Displays relevant error message based on quote context.
*/
void	handle_expanded_invalid_cmd(t_cmd *cmd, t_shell_data *sd)
{
	if (is_single_quoted(cmd->name))
	{
		write(2, "minishell: ", 11);
		write(2, cmd->name + 1, ft_strlen(cmd->name) - 1);
		write(2, ": command not found\n", 20);
		sd->last_exit_status = 127;
		return ;
	}
	if (is_double_quoted(cmd->name))
	{
		write(2, "minishell: : command not found\n", 31);
		sd->last_exit_status = 127;
		return ;
	}
	else
	{
		sd->last_exit_status = 0;
		return ;
	}
}
