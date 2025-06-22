#include "../../include/minishell.h"

// /*
//  * Determines if character at given index is within quotes.
//  * Tracks quote state from start of input up to index.
//  * Returns 1 if inside quotes, 0 if outside.
//  * Handles both single and double quotes.
// */
// int	check_if_quotes(char *input, int *i)
// {
// 	int		j;
// 	int		in_quote;
// 	char	quote_type;

// 	j = 0;
// 	in_quote = 0;
// 	quote_type = 0;
// 	while (j <= *i)
// 	{
// 		if ((input[j] == '\'' || input[j] == '"') && !in_quote)
// 		{
// 			in_quote = 1;
// 			quote_type = input[j];
// 		}
// 		else if (input[j] == quote_type && in_quote)
// 			in_quote = 0;
// 		j++;
// 	}
// 	return (in_quote);
// }


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
