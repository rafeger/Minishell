
#include "../../include/minishell.h"

/*
 * Reports export-related errors to stderr.
 * Handles different error types with appropriate message.
 * Updates shell exit status accordingly.
*/
void	export_error(char *identifier, char *arg, t_shell_data *sd)
{
	if (identifier[0] == '-')
	{
		write(2, "minishell: export: -", 20);
		if (identifier[1])
			write(2, &identifier[1], 1);
		write(2, ": invalid option\n", 17);
		sd->last_exit_status = 2;
		return ;
	}
	write(2, "minishell: export: '", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	sd->last_exit_status = 1;
}

/*
 * Checks for presence of exclamation mark in string.
 * Used for history expansion validation.
*/
int	check_exclamation_export(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '!')
			return (1);
		str++;
	}
	return (0);
}

/*
 * Validates first character of identifier.
 * Ensures it's a letter or underscore.
*/
int	is_valid_first_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

/*
 * Checks if character is valid for variable identifier.
 * Allows alphanumeric and underscore.
*/
int	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
 * Validates complete variable identifier.
 * Checks first character and subsequent characters.
 * Returns 1 if valid, 0 if invalid.
*/
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || !is_valid_first_char(str[0]))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!is_valid_identifier_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}
