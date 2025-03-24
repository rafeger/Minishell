#include "minishell.h"
#include <string.h>

char	*ft_strndup(char *input, int start, int end)
{
	int	i;

	i = 0;
	char	*word;
	word = malloc((end - start + 1) * sizeof(char));
	while(start < end)
	{
		word[i] = input[start];
		i++;
		start++;
	}
	word[i] = '\0';
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

void	syntax_error(char *error)
{
	printf("\n Syntax Error !\n");
	if (error == '"')
		printf("The \" is not closed\n");
	if (error == "<")
		printf("< leads to nothing\n");
}

void	handle_quotes(t_token *tokens, char *input, int *i)
{
	
}

void	handle_whitespace(char *input, int *i)
{
	while ((input[(*i)] == ' ') || (input[(*i)] == '\t'))
		(*i)++;
}

void handle_pipe(t_token *tokens, char *input, int *i)
{
	add_token(&tokens, PIPE, "|");
	(*i)++;
}
 
void	handle_redir(t_token *tokens, char *input, int *i)
{
	if (input[(*i)] == '>')
	{	
		if (input[(*i) + 1] == '>')
		{	
			add_token(&tokens, REDIR_APP, ">>");
			(*i)++;
			(*i)++;
		}
		else
		{
			add_token(&tokens, REDIR_OUT, ">");
			(*i)++;
		}
	}
	else if (input[(*i)] == '<')
	{
		if (input[(*i)+1] == '<')
		{
			add_token(&tokens, HEREDOC, "<<");
			(*i) += 2;
		}
		else
		{
			add_token(&tokens, REDIR_OUT, "<");
			(*i)++;
		}
	}

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
			handle_whitespace(input, &i);
		else if (input[i] == '|')
			handle_pipe(tokens, input, &i);
		else if (input[i] == '>' || input[i] == '<')
			handle_redir(tokens, input, &i);
		else if (input[i] == '"')
			handle_quotes(tokens, input, &i);
		else
			i = extract_word(&tokens, input, i);
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


int main()
{
    char *inputs[] = {
        "ls -l",
        "echo 'hello world'",
        "cat < input.txt",
        "grep .c | wc -l",
        "echo \"text > file\" > output.txt",
        "ls | grep txt | wc -l",
        NULL
    };
	int i;
	i = 0;
    while (i < 6)
    {
        printf("\n Test %d : \"%s\"\n", i + 1, inputs[i]);
        t_token *tokens = lexer(inputs[i]);  // Appelle ton tokenizer
        print_tokens(tokens);
		i++;
    }
    return 0;
}

