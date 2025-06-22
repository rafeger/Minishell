
#include "../../include/minishell.h"


/*
 * Handles cleanup after token addition failure.
 * Frees allocated memory and resets pointers.
 * Returns 1 to indicate failure.
*/
int	handle_token_add_failure(t_ta *ta)
{
	free(ta->quoted); // Changed order of freeing
	free(ta->tokens);
	ta->tokens = NULL;
	ta->quoted = NULL;
	return (1);
}

/*
 * Verifies all quotes are properly closed.
 * Reports syntax error for unclosed quotes.
 * Returns 1 if quotes are unclosed, 0 if all quotes are matched.
*/
int	validate_quote_closure(t_ta *ta)
{
	if (ta->in_q)
	{
		write(2, "minishell: unterminated quote\n", 30); // Rephrased error message
		return (1);
	}
	return (0);
}

/*
 * Performs cleanup of lexer structure.
 * Frees all allocated memory.
 * Returns NULL to indicate lexing failure.
*/
t_ta	*cleanup_lexer_resources(t_ta *ta)
{
	free_tokenarray(ta);
	return (NULL);
}