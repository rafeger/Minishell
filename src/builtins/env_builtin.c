
#include "../../include/minishell.h"

void	builtin_env(t_shell_data *shell_data)
{
	t_env	*env;

	env = shell_data->env;
	while (env)
	{
		write(2, env->key, ft_strlen(env->key));
		write(2, "=", 1);
		write(2, env->value, ft_strlen(env->value));
		write(2, "\n", 1);
		env = env->next;
	}
	shell_data->last_exit_status = 0;
}
