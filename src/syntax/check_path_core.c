#include "../../include/minishell.h"

/**
 * @brief Categorizes a given command string based on whether it represents a dot command.
 *
 * This function examines the input command string to determine if it's a special
 * dot command ('.') or a double-dot command ('..'). It helps in identifying
 * commands that require specific handling due to their filesystem implications.
 *
 * @param cmd The command string to evaluate.
 * @return Returns 1 if the command is a single dot ('.').
 * @return Returns 2 if the command is a double dot ('..') or multiple dots (e.g., '...').
 * @return Returns 0 if the command is neither a single dot nor a double/multiple dot command.
 */
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
	// Check for commands that start with '/' or '.' but are not just dots
	if (cmd[0] != '/' && cmd[0] != '.')
		return (0);
	return (0);
}

/**
 * @brief Verifies if a given string consists solely of characters typically found in file paths.
 *
 * This function checks if the input string contains only path-related characters
 * such as slashes ('/'), dots ('.'), and whitespace. It also requires that
 * the string contains at least one dot or slash to be considered a path.
 * This helps in pre-validating user input that is expected to be a directory path.
 *
 * @param input The string to be checked.
 * @return Returns 1 if the string contains only path characters and meets the minimum path criteria.
 * @return Returns 0 otherwise.
 */
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

/**
 * @brief Manages the display of error messages and updates shell status for special dot commands.
 *
 * This function differentiates between single dot ('.') and double dot ('..')
 * commands and outputs the appropriate error messages to standard error.
 * It also sets the shell's last exit status according to the type of dot command encountered.
 * This ensures consistent error reporting for these specific built-in command variations.
 *
 * @param cmd The command string that was identified as a dot command.
 * @param shell_data A pointer to the shell's data structure to update the exit status.
 */
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

/**
 * @brief Determines if the given input string corresponds to an existing directory path.
 *
 * This function first checks if the input string is composed entirely of valid
 * path characters. If it is, it then uses `stat` to check the actual filesystem
 * entry. If the path exists and points to a directory, an error message is
 * printed to standard error, and the function returns true.
 * This is crucial for preventing commands from attempting to execute directories.
 *
 * @param input The string representing a potential path.
 * @return Returns 1 if the input is a valid path to a directory, and an error message is displayed.
 * @return Returns 0 if the input is not a directory or is an invalid path.
 */
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
			ft_putendl_fd(": Is a directory", 2);
			return (1);
		}
	}
	return (0);
}
