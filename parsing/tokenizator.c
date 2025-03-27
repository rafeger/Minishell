#include "minishell.h"
#include <string.h>


void	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new = malloc(sizeof(t_token));
	if (!new)
		return;
	new->type = type;
	new->value = strdup(value); //utiliser ft_strdup
	new->next = NULL;
	if (*tokens == NULL)
		*tokens = new;
	else
	{
		t_token *temp = *tokens;
		while(temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

int	extract_word(t_token **tokens, char *input, int i)
{
	int	j;
	char *word;

	j = i;
	while (input[j] && input[j] != ' ' && input[j] != '\t' && input[j] != '>' && input[j] != '<')
		j++;
	word = ft_strndup(input, i, j); //copier de input[i] a input[j]
	add_token(tokens, WORD, word);
	free(word);
	return (j);
}


t_token	*lexer(char	*input)
{
	int	i;
	int	quote_nb;

	i = 0;
	t_token *tokens;
	tokens = NULL;


	quote_nb = check_quotes(input);
	if (quote_nb % 2 != 0)
		error("Syntax error, quote is unclosed !\n");
	while (input[i])
	{
		if ((input[i] == ' ') || (input[i] == '\t'))
			handle_whitespace(input, &i);
		else if (input[i] == '|')
			handle_pipe(tokens, input, &i);
		else if (input[i] == '>' || input[i] == '<')
			handle_redir(tokens, input, &i);
		else if (input[i] == '"' || input[i] == '\'')
			handle_quotes(tokens, input, &i, input[i]);
		else
			i = extract_word(&tokens, input, i);
	}
	return tokens;
}

//tests.c

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: Type=%d, Value=%s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}


int main(int ac, char *av[])
{
	(void) ac;

	int i;
	i = 0;
	printf("\n Test %d : \"%s\"\n", i + 1, av[1]);
	t_token *tokens = lexer(av[1]);  // Appelle ton tokenizer
	print_tokens(tokens);
	i++;
    return 0;
}

