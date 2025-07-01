#include "../../include/minishell.h"

int	check_if_quotes(char *input, int *i)
{
	int		current_pos;
	int		is_within_quotes;
	char	active_quote_type;

	current_pos = 0;
	is_within_quotes = 0;
	active_quote_type = 0;
	while (current_pos <= *i)
	{
		if ((input[current_pos] == '\'' || input[current_pos] == '"') && !is_within_quotes)
		{
			is_within_quotes = 1;
			active_quote_type = input[current_pos];
		}
		else if (input[current_pos] == active_quote_type && is_within_quotes)
		{
			is_within_quotes = 0;
			active_quote_type = 0; // Reset quote type
		}
		current_pos++;
	}
	return (is_within_quotes);
}
