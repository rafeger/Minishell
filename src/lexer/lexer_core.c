#include "../../include/minishell.h"

/*
 * Processes a single character during tokenization.
 * Handles memory reallocation for token buffer when needed.
 * Manages different character types:
 * - Quotes (single and double)
 * - Whitespace
 * - Special characters (<, >, |, $)
 * - Regular characters
 * Core function of character-by-character tokenization process.
*/
void	process_char(t_ta *ta, char **input)
{
	size_t	oldsize;

	oldsize = ta->tokensize + 1;
	if (ta->tokenindex == ta->tokensize)
	{
		if (ta->tokensize == 0)
			ta->tokensize = 1;
		else
			ta->tokensize = ta->tokensize * 2;
		ta->token = ft_realloc(ta->token, oldsize * sizeof(char), \
				(ta->tokensize + 1) * sizeof(char));
		if (!ta->token)
			return ;
		ta->token[ta->tokenindex] = '\0';
	}
	if (**input == '"' || **input == '\'' || ta->in_q)
		process_quotes(ta, input);
	else if (**input == ' ' || **input == '\t')
		handle_token_end(ta);
	else if (!ta->in_q && ft_strchr("<>|$", **input))
		handle_special_chars(ta, input);
	else
		ta->token[ta->tokenindex++] = **input;
}

/*
 * Handles complete input string processing.
 * Iterates through entire input, processing each character.
 * Ensures final token is properly handled.
 * Main driver of lexical analysis process.
*/
void	process_input(t_ta *ta, char *input)
{
    while (*input)
    {
        process_char(ta, &input);
        input++;
    }
    handle_token_end(ta);
}

/*
 * Finalizes current token and adds it to token array.
 * Manages quote status and trailing space handling.
 * Resets token index and quote tracking for next token.
 * Called whenever a token boundary is reached.
*/
void	handle_token_end(t_ta *ta)
{
	int		was_quoted;

	was_quoted = ta->second_quote;
	if (ta->tokenindex > 0 || ta->trailing_space)
	{
		if (ta->t_tot >= ta->cap)
			resize_token_array(ta);
		ta->token[ta->tokenindex] = '\0';
		if (ta->trailing_space && ta->tokenindex == 0)
			handle_trailing_space(ta, was_quoted);
		else
		{
			ta->quoted[ta->t_tot] = was_quoted;
			add_token(ta, ta->token);
		}
		ta->tokenindex = 0;
		ta->second_quote = 0;
	}
}

/*
 * Adds new token to token array.
 * Handles dynamic resizing of token storage if needed.
 * Manages quote status tracking for token.
 * Returns 0 on success, 1 on allocation failure.
*/
int	add_token(t_ta *ta, char *token)
{
	char	**new_tokens;
	int		*new_quoted;
	size_t	oldsize;

	if (ta->t_tot == ta->cap)
	{
		oldsize = ta->cap;
		ta->cap *= 2;
		new_tokens = ft_realloc(ta->tokens, oldsize * sizeof(char *), \
				sizeof(char *) * ta->cap);
		new_quoted = ft_realloc(ta->quoted, oldsize * sizeof(int), \
				sizeof(int) * ta->cap);
		if (!new_tokens || !new_quoted)
			return (add_token_failed(ta));
		ta->tokens = new_tokens;
		ta->quoted = new_quoted;
	}
	ta->tokens[ta->t_tot] = ft_strdup(token);
	if (!ta->tokens[ta->t_tot])
		return (1);
	ta->quoted[ta->t_tot] = ta->second_quote;
	ta->t_tot++;
	return (0);
}

/*
 * Main lexical analysis function.
 * Coordinates complete tokenization process:
 * - Handles special case of quote-only input
 * - Processes input character by character
 * - Manages token creation and storage
 * - Handles cleanup in case of errors
 * Returns NULL if analysis fails, otherwise returns completed token array.
*/
t_ta	*lexer(char *input)
{
	t_ta	*ta;

	ta = tokenarray_init();
	if (!ta)
		return (NULL);
	if (is_only_quotes(input))
		return (create_special_empty_token(ta));
	process_input(ta, input);
	if (check_unclosed_quotes(ta))
		return (clean_lexer(ta));
	if (ta->tokenindex > 0)
		ta->token[ta->tokenindex] = '\0';
	if (ta->tokenindex > 0 && add_token(ta, ta->token))
		return (clean_lexer(ta));
	if (!ta->t_tot)
		return (clean_lexer(ta));
	return (ta);
}
