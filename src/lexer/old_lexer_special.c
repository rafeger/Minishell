#include "../../include/minishell.h"


/*
 * Handles spaces at end of tokens.
 * Creates appropriate token with space handling.
 * Manages quote status in space-terminated tokens.
*/
void	handle_trailing_space(t_ta *ta, int was_quoted)
{
	char	*tmp;

	if (ta->tokenindex == 0)
	{
		ta->token[0] = ' ';
		ta->token[1] = '\0';
		add_token(ta, ta->token);
	}
	else
	{
		tmp = ft_strjoin(ta->token, " ");
		if (tmp)
		{
			ta->quoted[ta->t_tot] = was_quoted;
			add_token(ta, tmp);
			free(tmp);
		}
	}
	ta->trailing_space = 0;
}

/*
 * Processes redirection and operator tokens.
 * Handles double operators (<< and >>).
 * Creates single token from consecutive operators.
 * Essential part of special character processing in tokenization.
*/
static void	process_special_token(t_ta *ta, char **input)
{
	if ((**input == '<' && *(*input + 1) == '<') || \
		(**input == '>' && *(*input + 1) == '>'))
	{
		ta->token[1] = **input;
		ta->token[2] = '\0';
		(*input)++;
	}
	else
		ta->token[1] = '\0';
	add_token(ta, ta->token);
}

/*
 * Processes special characters (>, <, |, $).
 * Creates appropriate tokens for redirections and pipes.
 * Handles consecutive special characters.
 * Core function for operator tokenization.
*/
void	handle_special_chars(t_ta *ta, char **input)
{
	char	*spe;

	spe = "<>|$";
	if (ta->second_quote)
	{
		ta->token[ta->tokenindex++] = **input;
		return ;
	}
	if (ft_strchr(spe, **input))
	{
		if (ta->tokenindex > 0)
		{
			ta->token[ta->tokenindex] = '\0';
			add_token(ta, ta->token);
			ta->tokenindex = 0;
		}
		ta->token[0] = **input;
		process_special_token(ta, input);
	}
}

/*
 * Expands token array cap.
 * Doubles current cap when needed.
 * Initializes new memory space.
 * Essential for dynamic token storage.
*/
void	resize_token_array(t_ta *ta)
{
	int		new_capacity;
	char	**new_tokens;
	int		*new_quoted;
	int		i;

	new_capacity = ta->cap * 2;
	new_tokens = ft_realloc(ta->tokens, ta->cap * sizeof(char *), \
			new_capacity * sizeof(char *));
	new_quoted = ft_realloc(ta->quoted, ta->cap * sizeof(int), \
			new_capacity * sizeof(int));
	if (!new_tokens || !new_quoted)
		return ;
	ta->tokens = new_tokens;
	ta->quoted = new_quoted;
	i = ta->cap;
	while (i < new_capacity)
	{
		ta->tokens[i] = NULL;
		ta->quoted[i] = 0;
		i++;
	}
	ta->cap = new_capacity;
}
