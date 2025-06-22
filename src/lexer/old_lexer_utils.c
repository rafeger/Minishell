// #include "../../include/minishell.h"


// /*
//  * Handles cleanup after token addition failure.
//  * Frees allocated memory and resets pointers.
//  * Returns 1 to indicate failure.
// */
// int	add_token_failed(t_ta *ta)
// {
// 	free(ta->tokens);
// 	free(ta->quoted);
// 	ta->tokens = NULL;
// 	ta->quoted = NULL;
// 	return (1);
// }

// /*
//  * Verifies all quotes are properly closed.
//  * Reports syntax error for unclosed quotes.
//  * Returns 1 if quotes are unclosed, 0 if all quotes are matched.
// */
// int	check_unclosed_quotes(t_ta *ta)
// {
// 	if (ta->in_q)
// 	{
// 		write(2, "minishell: syntax error: unclosed quote\n", 40);
// 		return (1);
// 	}
// 	return (0);
// }

// /*
//  * Performs cleanup of lexer structure.
//  * Frees all allocated memory.
//  * Returns NULL to indicate lexing failure.
// */
// t_ta	*clean_lexer(t_ta *ta)
// {
// 	free_tokenarray(ta);
// 	return (NULL);
// }