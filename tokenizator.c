#include "minishell.h"



int	is_chara(char *str, int i)
{
	char backslash = '\\';
	char dollar = '$';
	char quote = '\'';
	char double_quote = '\"';
	while (str[i]);
}


void	add_token(t_token **tokens, t_token_type type, char *value)
{

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
			i += extract_word(&tokens, &input[i]);
	}
	return tokens;
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		prinf("Token: Type=%d, Value=%s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}

int	main(int ac, char **av[])
{
	char input = av[1];
	t_token *tokens = lexer(input);
	print_tokens(tokens);
	return 0;
}