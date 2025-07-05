#include "../../include/minishell.h"

static void	set_shell_level(t_env **env_head, int lvl)
{
	char	*lvl_str;

	lvl_str = ft_itoa(lvl);
	if (!lvl_str)
		return ;
	set_env_variable(env_head, "SHLVL", lvl_str);
	free(lvl_str);
}

static void	ensure_shlvl(t_env **env_head)
{
	char	*shlvl_val;
	int		lvl;

	lvl = 0;
	shlvl_val = find_env_val(*env_head, "SHLVL");
	if (!shlvl_val || !*shlvl_val)
		set_shell_level(env_head, 1);
	else
		lvl = ft_atoi(shlvl_val);
	set_shell_level(env_head, lvl + 1);
}

static t_shell_data	*create_shell_data(char **envp)
{
	t_shell_data	*shdata;

	shdata = malloc(sizeof(t_shell_data));
	if (!shdata)
		return (NULL);
	shdata->env = build_env_list(envp);
	shdata->cmd = NULL;
	shdata->last_exit_status = 0;
	shdata->sig_quit_flag = 0;
	return (shdata);
}

int	setup_shell(t_shell_data **sh, char **envp)
{
	setup_signals();
	*sh = create_shell_data(envp);
	if (!*sh)
		return (1);
	ensure_shlvl(&(*sh)->env);
	return (0);
}