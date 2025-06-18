
#include "../../include/minishell.h"

static int	handle_unset_error(char *arg, int has_exclamation)
{
	if (has_exclamation)
	{
		write(2, "minishell: unset: '", 19);
		write(2, ft_strchr(arg, '!'), ft_strlen(ft_strchr(arg, '!')));
		write(2, "': event not found\n", 19);
	}
	else
	{
		write(2, "minishell: unset: '", 19);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
	}
	return (1);
}

static int	is_valid_unset_identifier(const char *str, int *has_exclamation)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_' && str[0] != '$')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] == '!')
		{
			*has_exclamation = 1;
			return (0);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_unset(t_cmd *cmd, t_shell_data *shell_data)
{
	int	i;
	int	has_exclamation;

	i = 1;
	while (i < cmd->arg_count)
	{
		has_exclamation = 0;
		if (!is_valid_unset_identifier(cmd->args[i], &has_exclamation))
		{
			shell_data->last_exit_status = handle_unset_error(cmd->args[i], \
					has_exclamation);
			return ;
		}
		remove_env_var(&shell_data->env, cmd->args[i]);
		i++;
	}
	shell_data->last_exit_status = 0;
}
