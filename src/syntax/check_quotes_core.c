#include "../../include/minishell.h"

/*
 * Determines if character at given index is within quotes.
 * Tracks quote state from start of input up to index.
 * Returns 1 if inside quotes, 0 if outside.
 * Handles both single and double quotes.
*/
int	check_if_quotes(char *input, int *i)
{
	int		j;
	int		in_quote;
	char	quote_type;

	j = 0;
	in_quote = 0;
	quote_type = 0;
	while (j <= *i)
	{
		if ((input[j] == '\'' || input[j] == '"') && !in_quote)
		{
			in_quote = 1;
			quote_type = input[j];
		}
		else if (input[j] == quote_type && in_quote)
			in_quote = 0;
		j++;
	}
	return (in_quote);
}
