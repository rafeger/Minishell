#include "../../include/minishell.h"

t_ta	*init_token_array(void)
{
	t_ta	*ta;

	ta = malloc(sizeof(t_ta));
	if (!ta)
		return (NULL);
	ta->tokens = malloc(sizeof(char *) * INITIAL_CAPACITY);
	ta->current_token = malloc(sizeof(char) * INITIAL_BUFFER_SIZE);
	if (!ta->tokens || !ta->current_token)
	{
		free(ta->tokens);
		free(ta->current_token);
		free(ta);
		return (NULL);
	}
	ta->token_capacity = INITIAL_BUFFER_SIZE;
	ta->token_index = 0;
	ta->token_count = 0;
	ta->inside_quotes = 0;
	ta->quote_char = '\0';
	ta->trailing_space = 0;
	return (ta);
}

void	add_token(t_ta *ta)
{
	if (ta->token_index == 0 && !ta->trailing_space)
		return ;
	ta->current_token[ta->token_index] = '\0';
	ta->tokens[ta->token_count] = ft_strdup(ta->current_token);
	ta->token_count++;
	ta->token_index = 0;
	ta->trailing_space = 0;
}

void	handle_quotes(t_ta *ta, char c)
{
	if (ta->inside_quotes && ta->quote_char == c)
	{
		ta->inside_quotes = 0;
		ta->quote_char = '\0';
		if (ta->token_index == 0)
		{
			ta->current_token[0] = '\0';
			add_token(ta);
		}
	}
	else if (!ta->inside_quotes)
	{
		ta->inside_quotes = 1;
		ta->quote_char = c;
	}
	else
		ta->current_token[ta->token_index++] = c;
}

void	handle_special_char(t_ta *ta, char c)
{
	add_token(ta);
	ta->current_token[0] = c;
	ta->current_token[1] = '\0';
	ta->tokens[ta->token_count] = ft_strdup(ta->current_token);
	ta->token_count++;
}

void	process_char(t_ta *ta, char c)
{
	if (c == '"' || c == '\'')
		handle_quotes(ta, c);
	else if (c == ' ' || c == '\t')
		add_token(ta);
	else if (ft_strchr("<>|$", c))
		handle_special_char(ta, c);
	else
		ta->current_token[ta->token_index++] = c;
}

void	process_input(t_ta *ta, const char *input)
{
	while (*input)
	{
		process_char(ta, *input);
		input++;
	}
	add_token(ta);
}

t_ta	*lexer(const char *input)
{
	t_ta	*ta;

	ta = init_token_array();
	if (!ta)
		return (NULL);
	process_input(ta, input);
	return (ta);
}
