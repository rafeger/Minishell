#include "../../include/minishell.h"

/*
 * Determines if command is a dot of double dot command.
 * Returns:
 * - 1 for single dot.
 * - 2 for double dot or multiple dots.
 * - 0 for other commands.
*/
int	is_dot_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, ".") == 0)
		return (1);
	if (cmd[0] == '.' && cmd[1] == '.' && !cmd[2])
		return (2);
	if (cmd[0] != '/' && cmd[0] != '.')
		return (0);
	while (*cmd == '.')
		cmd++;
	if (*cmd == '\0')
		return (2);
	return (0);
}

/*
 * Checks if input contains only path-related characters (/, ., spaces).
 * Requires at least one dot or slash.
 * Returns 1 if string contains only path chars, 0 otherwise.
 * Used to identify potential directory paths.
*/
int	is_only_path_chars(char *input)
{
	int	i;
	int	has_dot;

	i = 0;
	has_dot = 0;
	while (input[i])
	{
		if (input[i] != '/' && input[i] != '.' && !ft_isspace(input[i]))
			return (0);
		if (input[i] == '.')
			has_dot = 1;
		i++;
	}
	return (i > 0 && (input[0] == '/' || has_dot));
}

/*
 * Handles special dot command cases with appropriate error messages.
 * For single dot: displays usage error.
 * For double dot: displays command not found error.
 * Updates shell exit status accordingly.
*/
void	handle_dot_command(const char *cmd, t_shell_data *shell_data)
{
	int	type;

	type = is_dot_command(cmd);
	if (type == 1)
	{
		write(2, "minishell: .: filename argument required\n", 41);
		write(2, ".: usage: . filename [arguments]\n", 33);
		shell_data->last_exit_status = 2;
	}
	else if (type == 2)
	{
		write(2, "minishell: ", 11);
		write(2, (char *)cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20);
		shell_data->last_exit_status = 127;
	}
}

/*
 * Verifies if input represents a directory path.
 * Checks both path format and actual filesystem status.
 * Returns 1 and displays error if path is directory.
 * Returns 0 for non-directory paths or invalid paths.
*/
int	check_directory_path(char *input)
{
	struct stat	path_stat;

	if (!is_only_path_chars(input))
		return (0);
	if (stat(input, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(input, 2);
			ft_putendl_fd(": Is a directory", 2);
			return (1);
		}
	}
	return (0);
}
