#include "minishell.h"

void	handle_quotes(t_token *tokens, char *input, int *i, char quote_type)
{
	if (quote_type == '"')
		handle_double_quotes(tokens, input, i);
	else if (quote_type == '\'')
		handle_single_quotes(tokens, input, i);
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