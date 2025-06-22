#include "../../include/minishell.h"

/*
 * Checks if input consists only of quote characters.
 * Returns 1 if string contains only quotes, 0 otherwise.
*/
int	contains_only_quotes(const char *input)
{
	int	non_quote_found;

	non_quote_found = 0;
	if (!input || !*input)
		return (0);
	while (*input)
	{
		if (*input != '"' && *input != '\'')
			non_quote_found = 1;
		input++;
	}
	return (!non_quote_found);
}

/*
 * Creates special token array for empty quoted string.
 * Returns NULL on allocation failure.
 * Used for handling "" or '' inputs.
*/
t_ta	*create_empty_quoted_token(t_ta *ta)
{
	char	*empty_str_token;

	empty_str_token = ft_calloc(3, sizeof(char)); // Manually allocate and set
	if (!empty_str_token)
	{
		free_tokenarray(ta);
		return (NULL);
	}
	empty_str_token[0] = '"';
	empty_str_token[1] = '"';
	ta->tokens[0] = empty_str_token;
	ta->t_tot = 1;
	ta->quoted[0] = 1;
	return (ta);
}

/*
 * Processes empty quotes case.
 * Updates input pointer and trailing space flag.
*/
void	process_empty_quotes(t_ta *ta, char **input)
{
	(*input)++;
	if (**input != '\0' && !ft_isspace(**input)) // Changed condition slightly
		ta->trailing_space = 0;
}

/*
 * Coordinates quote processing in input.
 * Handles empty quotes and quote pairs.
 * Manages trailing space after quotes.
*/
void	manage_quote_processing(t_ta *ta, char **input)
{
	int	is_empty_pair;

	is_empty_pair = 0;
	if (!ta->in_q)
	{
		if ((**input == '"' && *(*input + 1) == '"') || \
			(**input == '\'' && *(*input + 1) == '\''))
			is_empty_pair = 1;
	}
	if (is_empty_pair)
	{
		if (*(*input + 2) && ft_isspace(*(*input + 2)))
			ta->trailing_space = 1;
		*input += 1;
		return ;
	}
	process_quote_character(ta, *input);
	if (!ta->in_q && *(*input + 1) && ft_isspace(*(*input + 1)))
		ta->trailing_space = 1;
}

/*
 * Core quote handling function.
 * Manages quote state tracking.
 * Handles nested quotes and quote character inclusion.
 * Essential for maintaining proper quote context during tokenization.
*/
void	process_quote_character(t_ta *ta, char *input)
{
	if (ta->in_q == 0) // Explicitly checking for 0
	{
		ta->in_q = 1;
		ta->quotechar = *input;
	}
	else if (*input == ta->quotechar) // Reordered condition
		ta->in_q = 0;
	if (ta->tokenindex > 0 && ta->in_q == 0 && ta->second_quote == 0) // More explicit check
		ta->second_quote = 1;
	else if (ta->tokenindex == 0 && ta->in_q == 0)
		ta->second_quote = 1;
	ta->token[ta->tokenindex++] = *input;
}