#include "../../include/minishell.h"

/*
 * Determines how to process a character based on its type and current lexer state.
 * This is a helper for handle_char_tokenization.
*/
static void	decide_char_processing(t_ta *ta, char **input)
{
	if (**input == '"' || **input == '\'' || ta->in_q)
		manage_quote_processing(ta, input);
	else if (**input == ' ' || **input == '\t')
		handle_token_end(ta);
	else if (!ta->in_q && ft_strchr("<>|$", **input))
		handle_special_chars(ta, input);
	else
		ta->token[ta->tokenindex++] = **input;
}

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
void	handle_char_tokenization(t_ta *ta, char **input)
{
	size_t	current_size;
	char	*temp_token;

	current_size = ta->tokensize + 1;
	if (ta->tokenindex >= ta->tokensize)
	{
		if (ta->tokensize == 0)
			ta->tokensize = 2;
		else
			ta->tokensize *= 2;
		temp_token = malloc((ta->tokensize + 1) * sizeof(char));
		if (!temp_token)
		{
			free(ta->token);
			ta->token = NULL;
			return ;
		}
		if (ta->token)
		{
			ft_memcpy(temp_token, ta->token, current_size * sizeof(char));
			free(ta->token);
		}
		ta->token = temp_token;
	}
	decide_char_processing(ta, input);
}

/*
 * Handles complete input string processing.
 * Iterates through entire input, processing each character.
 * Ensures final token is properly handled.
 * Main driver of lexical analysis process.
*/
void	process_input(t_ta *ta, char **input)
{
	while (**input)
	{
		handle_char_tokenization(ta, input);
		(*input)++;
	}
	if (ta->tokenindex > 0 || ta->trailing_space)
		handle_token_end(ta);
}

/*
 * Processes the end of a token.
 * Adds the current token to the token array.
 * Resets token buffer for next token.
 * Manages trailing spaces as separate tokens.
*/
void	handle_token_end(t_ta *ta)
{
	if (ta->tokenindex > 0)
	{
		ta->token[ta->tokenindex] = '\0';
		append_token_to_array(ta, ta->token);
		ta->tokenindex = 0;
	}
	if (ta->trailing_space)
	{
		handle_trailing_space(ta, ta->second_quote);
		ta->trailing_space = 0;
	}
}

/*
 * Adds a processed token to the token array.
 * Handles dynamic resizing of the token array.
 * Copies token string and quote status tracking for token.
 * Returns 0 on success, 1 on allocation failure.
*/
int	append_token_to_array(t_ta *ta, char *token)
{
	char	**new_tokens;
	int		*new_quoted;
	int		i;

	if (ta->t_tot == ta->cap)
	{
		ta->cap = (ta->cap == 0) ? 4 : ta->cap * 2;
		new_tokens = malloc(sizeof(char *) * ta->cap);
		new_quoted = malloc(sizeof(int) * ta->cap);
		if (!new_tokens || !new_quoted)
		{
			if (new_tokens)
				free(new_tokens);
			if (new_quoted)
				free(new_quoted);
			return (handle_token_add_failure(ta));
		}
		i = 0;
		while (i < ta->t_tot)
		{
			new_tokens[i] = ta->tokens[i];
			new_quoted[i] = ta->quoted[i];
			i++;
		}
		free(ta->tokens);
		free(ta->quoted);
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
t_ta	*tokenize_input_string(char *input)
{
	t_ta	*ta;
	int		quotes_at_start;

	quotes_at_start = 0;
	ta = tokenarray_init();
	if (!ta)
		return (NULL);
	if (contains_only_quotes(input))
		quotes_at_start = 1;
	process_input(ta, &input);
	if (validate_quote_closure(ta))
		return (cleanup_lexer_resources(ta));
	if (ta->t_tot == 0)
	{
		if (quotes_at_start)
			return (create_empty_quoted_token(ta));
		cleanup_lexer_resources(ta);
		return (NULL);
	}
	return (ta);
}