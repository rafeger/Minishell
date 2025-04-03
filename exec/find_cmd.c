#include "../minishell.h"

static void	print_command_not_found(char *cmd)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

static void	cleanup_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

static char	*construct_and_check_path(char *path, char *cmd)
{
	char	*start_path;
	char	*exec;

	start_path = ft_strjoin(path, "/");
	exec = ft_strjoin(start_path, cmd);
	free(start_path);
	if (access(exec, F_OK) == 0)
		return (exec);
	free(exec);
	return (NULL);
}

char	*get_pathname(char *cmd, char **envp)
{
	char	**paths;
	char	*exec;
	int		i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		exec = construct_and_check_path(paths[i], cmd);
		if (exec)
		{
			cleanup_split(paths);
			return (exec);
		}
		i++;
	}
	cleanup_split(paths);
	print_command_not_found(cmd);
	return (NULL);
}

