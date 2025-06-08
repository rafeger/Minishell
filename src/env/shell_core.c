#include "../../minishell.h"

/*
 * Updates shell level (SHLVL) environment variable.
 * Takes current level and increments it.
 * Handles conversion and environment variable update.
 * Used to track nested shell depth.
*/
void	update_shlvl(t_env **env, int level)
{
	char	*new_level;

	new_level = ft_itoa(level);
	if (!new_level)
		return ;
	set_env_value(env, "SHLVL", new_level);
	free(new_level);
}

/*
 * Initializes or updates SHLVL environment variable.
 * If SHLVL doesn't exist, creates it with value 1.
 * If exists, increments current value.
 * Essential for proper shell nesting behavior.
*/
void	initialize_shlvl(t_env **env)
{
	char	*shlvl;
	int		level;

	level = 0;
	shlvl = get_env_value(*env, "SHLVL");
	if (!shlvl || !*shlvl)
		update_shlvl(env, 1);
	else
		level = ft_atoi(shlvl);
	update_shlvl(env, level + 1);
}

/*
 * Creates and initializes main shell data structure.
 * Sets up:
 * - Environment variables list.
 * - Command structure (initially NULL);
 * - Exit status tracking.
 * - Signal handling flags.
*/
t_shell_data	*init_shell_data(char **envp)
{
	t_shell_data	*shell_data;

	shell_data = malloc(sizeof(t_shell_data));
	if (!shell_data)
		return (NULL);
	shell_data->env = init_env(envp);
	shell_data->cmd = NULL;
	shell_data->last_exit_status = 0;
	shell_data->sig_quit_flag = 0;
	return (shell_data);
}

/*
 * Main shell initialization function that:
 * - Sets up signal handlers.
 * - Creates shell data structure.
 * - Initializes shell level.
 * Returns 0 on success, 1 on failure.
*/
int	initialize_shell(t_shell_data **shell, char **envp)
{
	init_signals();
	*shell = init_shell_data(envp);
	if (!*shell)
		return (1);
	initialize_shlvl(&(*shell)->env);
	return (0);
}
