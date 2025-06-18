
#include "../../include/minishell.h"

/*
 * Displays all exported variables in declare format.
 * Lists variables in current environment.
 * Used when export called without arguments.
*/
static void	print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		write(2, "declare -x ", 11);
		write(2, current->key, ft_strlen(current->key));
		write(2, "=\"", 2);
		write(2, current->value, ft_strlen(current->value));
		write(2, "\"\n", 2);
		current = current->next;
	}
}

/*
 * Processes individual export value.
 * Handles variable creation or display.
 * Validates syntax and handles errors.
 * Returns appropriate status code.
*/
/* k = key, v = value, fa = full_arg, sd = shell_data */
static int	handle_export_value(char *k, char *v, char *fa, t_shell_data *sd)
{
	if (!k)
	{
		print_export_list(sd->env);
		return (0);
	}
	if (check_exclamation_export(k) || (v && check_exclamation_export(v)))
	{
		export_event_error(fa);
		return (2);
	}
	if (!is_valid_identifier(k))
	{
		export_error(k, fa, sd);
		return (1);
	}
	if (v)
		set_env_value(&sd->env, k, v);
	return (0);
}

/*
 * Validates export argument format.
 * Checks for invalid starting characters.
 * Returns 0 on success, 1 on error.
*/
static int	validate_export_arg(char *arg, char **fa, t_shell_data *shell_data)
{
	if (arg[0] == '=')
	{
		export_error(arg, arg, shell_data);
		return (1);
	}
	*fa = ft_strdup(arg);
	if (!*fa)
		return (1);
	return (0);
}

/*
 * Processes single export argument.
 * Handles variable assignment and validation.
 * Returns appropriate status code.
*/
static int	process_export_arg(char *arg, t_shell_data *shell_data)
{
	char	*key;
	char	*value;
	char	*full_arg;
	int		result;

	if (validate_export_arg(arg, &full_arg, shell_data))
		return (1);
	key = arg;
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
		if (!*value && shell_data->cmd->arg_count > 2)
		{
			value = shell_data->cmd->args[2];
			if (ft_strcmp(value, "$") == 0)
				value = "$";
		}
	}
	result = handle_export_value(key, value, full_arg, shell_data);
	free(full_arg);
	return (result);
}

/*
 * Main export builtin function.
 * Handles export command with or without arguments.
 * Manages variable creation and modification.
 * Returns appropriate exit status.
*/
int	builtin_export(t_cmd *cmd, t_shell_data *shell_data)
{
	int	i;
	int	result;

	i = 1;
	shell_data->last_exit_status = 0;
	if (cmd->arg_count == 1)
	{
		print_export_list(shell_data->env);
		return (0);
	}
	while (i < cmd->arg_count)
	{
		result = process_export_arg(cmd->args[i], shell_data);
		if (result == 2)
			shell_data->last_exit_status = 2;
		else if (result == 1)
			shell_data->last_exit_status = 1;
		i++;
	}
	return (shell_data->last_exit_status);
}
