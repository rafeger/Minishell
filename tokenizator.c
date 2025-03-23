#include "minishell.h"
#include <string.h>

char	*ft_strndup(char *input, int start, int end)
{
	int	i;

	i = 0;
	char	*word;
	word = malloc((end - start) * sizeof(char));
	while(start <= end)
	{
		word[i] = input[start];
		i++;
		start++;
	}
	return (word);
}

void	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new = malloc(sizeof(t_token));
	if (!new)
		return;
	
new->type = type;
	new->value = strdup(value); //utiliser ft_strdup
	new->next = NULL;
	if (!tokens)
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

	j = i;
	while (input[i] && input[i] != ' ' && input[i] != 
		'\t' && input[i] != '>' && input[i] != '<')
		j++;
	char *word;
	word = ft_strndup(input, i, j); //copier de input[i] a input[j]
	add_token(tokens, WORD, word);
	free(word);

	return (i);
}



// a RACCOURCIR
t_token	*lexer(char	*input)
{
	int	i;

	i = 0;
	t_token *tokens;
	tokens = NULL;

	while (input[i])
	{
		if ((input[i] == ' ') || (input[i] == '\t'))
			i++;
		else if (input[i] == '|')
		{
			add_token(&tokens, PIPE, "|");
			i++;
		}
		else if (input[i] == '>')
		{
			if (input[i = 1] == '>')
			{
				add_token(&tokens, REDIR_APP, ">>");
				i += 2;
			}
			else 
			{
				add_token(&tokens, REDIR_OUT, ">");
				i++;
			}
		}
		else if (input[i] == '<')
		{
			if (input[i+1] == '<')
			{
				add_token(&tokens, HEREDOC, "<<");
				i += 2;
			}
			else
			{
				add_token(&tokens, REDIR_OUT, ">");
				i++;
			}
		}
		else
			i += extract_word(&tokens, input, i);
	}
	return tokens;
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: Type=%d, Value=%s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}

int	main(int ac, char *av[])
{
	char *input;
	input = av[1];
	t_token *tokens = lexer(input);
	print_tokens(tokens);
	return 0;
}