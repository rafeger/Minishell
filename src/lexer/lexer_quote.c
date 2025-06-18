#include "../../include/minishell.h"


/*
 * Checks if input consists only of quote characters.
 * Returns 1 if string contains only quotes, 0 otherwise.
*/
int	is_only_quotes(const char *input)
{
	if (!input || !*input)
		return (0);
	while (*input)
	{
		if (*input != '"' && *input != '\'')
			return (0);
		input++;
	}
	return (1);
}

/*
 * Creates special token array for empty quoted string.
 * Returns NULL on allocation failure.
 * Used for handling "" or '' inputs.
*/
t_ta	*create_special_empty_token(t_ta *ta)
{
	ta->tokens[0] = ft_strdup("\"\"");
	if (!ta->tokens[0])
	{
		free_tokenarray(ta);
		return (NULL);
	}
	ta->count = 1;
	ta->quoted[0] = 1;
	return (ta);
}

/*
 * Processes empty quotes case.
 * Updates input pointer and trailing space flag.
*/
void	handle_empty_quotes(t_ta *ta, char **input)
{
	(*input)++;
	if (**input && !ft_isspace(**input))
		ta->trailing_space = 0;
}

/*
 * Coordinates quote processing in input.
 * Handles empty quotes and quote pairs.
 * Manages trailing space after quotes.
*/
void	process_quotes(t_ta *ta, char **input)
{
	if ((!ta->inquotes) && ((**input == '"' && *(*input + 1) == '"') || \
				(**input == '\'' && *(*input + 1) == '\'')))
	{
		if (*(*input + 2) && ft_isspace(*(*input + 2)))
			ta->trailing_space = 1;
		*input += 1;
		return ;
	}
	handle_quotes(ta, *input);
	if (!ta->inquotes && *(*input + 1) && ft_isspace(*(*input + 1)))
		ta->trailing_space = 1;
}

/*
 * Core quote handling function.
 * Manages quote state tracking.
 * Handles nested quotes and quote character inclusion.
 * Essential for maintaining proper quote context during tokenization.
*/
void	handle_quotes(t_ta *ta, char *input)
{
	int	was_quoted;

	if ((*input == '"' || *input == '\''))
	{
		if (!ta->inquotes)
		{
			ta->inquotes = 1;
			ta->quotechar = *input;
			ta->second_quote = 1;
		}
		else if (*input == ta->quotechar)
		{
			was_quoted = ta->inquotes;
			ta->inquotes = 0;
			ta->quotechar = '\0';
			ta->second_quote = was_quoted;
		}
		else
			ta->token[ta->tokenindex++] = *input;
	}
	else
		ta->token[ta->tokenindex++] = *input;
}
