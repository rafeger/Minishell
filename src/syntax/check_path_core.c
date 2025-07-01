#include "../../include/minishell.h"

int	is_dot_command(const char *cmd)
{
	int	len;

	if (!cmd)
		return (0);
	len = ft_strlen(cmd);
	if (len == 1 && cmd[0] == '.')
		return (1);
	if (len >= 2 && cmd[0] == '.' && cmd[1] == '.')
	{
		int i = 2;
		while (i < len && cmd[i] == '.')
			i++;
		if (i == len)
			return (2);
	}
	if (cmd[0] != '/' && cmd[0] != '.')
		return (0);
	return (0);
}

int	is_only_path_chars(char *input)
{
	int	idx;
	int	found_path_char;

	idx = 0;
	found_path_char = 0;
	if (!input)
		return (0);
	while (input[idx])
	{
		if (input[idx] != '/' && input[idx] != '.' && !ft_isspace(input[idx]))
			return (0);
		if (input[idx] == '/' || input[idx] == '.')
			found_path_char = 1;
		idx++;
	}
	return (idx > 0 && found_path_char);
}

void	handle_dot_command(const char *cmd, t_shell_data *shell_data)
{
	int	command_category;

	command_category = is_dot_command(cmd);
	if (command_category == 1)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		shell_data->last_exit_status = 2;
	}
	else if (command_category == 2)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		shell_data->last_exit_status = 127;
	}
}

int	check_directory_path(char *input)
{
	struct stat	file_info;

	if (!is_only_path_chars(input))
		return (0);
	if (stat(input, &file_info) == 0)
	{
		if (S_ISDIR(file_info.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(input, 2);
			ft_putendl_fd(": is a directory", 2);
			return (1);
		}
	}
	return (0);
}
