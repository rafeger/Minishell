
#include "../../include/minishell.h"

/*
 * Extracts exclamation mark portion from string.
 * Used for error message formatting.
 * Returns allocated string with relevant portion.
*/
static char	*get_exclamation_part(const char *str)
{
	char	*temp;
	char	*dollar;
	char	*excl;

	excl = ft_strchr(str, '!');
	if (!excl)
		return (NULL);
	dollar = ft_strchr(excl, '$');
	if (!dollar)
		return (ft_strdup(excl));
	temp = ft_strndup(excl, dollar - excl);
	return (temp);
}

/*
 * Handles event-related export errors.
 * Formats and displays appropriate error message.
 * Specific to history expansion errors.
*/
void	export_event_error(char *full_arg)
{
	char	*excl_part;

	write(2, "minishell: ", 11);
	excl_part = get_exclamation_part(full_arg);
	if (excl_part)
	{
		write(2, excl_part, ft_strlen(excl_part));
		free(excl_part);
	}
	write(2, ": event not found\n", 18);
}
