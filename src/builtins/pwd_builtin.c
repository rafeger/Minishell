
#include "../../include/minishell.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	*pwd;
	char	*arg;

	arg = cmd->args[1];
	if ((arg && arg[0] == '-' && arg[1] && arg[1] != '-') || \
			(arg && arg[0] == '-' && arg[1] == '-' && arg[2]))
	{
		write(2, "minishell: pwd: -", 17);
		write(2, &arg[1], 1);
		write(2, ": invalid option\n", 17);
		return (2);
	}
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		write(2, pwd, ft_strlen(pwd));
		write(2, "\n", 1);
		free(pwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
