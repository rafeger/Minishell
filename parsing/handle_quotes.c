#include "minishell.h"

void	handle_single_quotes(t_token *tokens, char *input, int *i)
{
	char quote_type;
	char *text;
	int	start;
	int j;

	quote_type = '\'';
	start = (*i) + 1;
	while (input[(*i)] && input[(*i)] != quote_type)
		(*i)++;
	if (input[(*i)] == '\0' && input[(*i) - 1] != quote_type)
		printf("Syntax error, quote is unclosed !\n");
	else
	{
		text = malloc(sizeof(char) * ((*i) - start + 1));
		j = 0;
		while (start < (*i))
			text[j++] = input[start++];
		text[j] = '\0';
		add_token(&tokens, WORD, text);
		free (text);
	}
	(*i)++;
}

void	handle_double_quotes(t_token *tokens, char *input, int *i)
{
	char quote_type;
	char *text;
	int	start;
	int j;

	quote_type = '"';
	start = (*i) + 1;
	while (input[(*i)] && (input[(*i)] != quote_type))
		(*i)++;
	if (input[(*i)] == '\0' && input[(*i) - 1] != quote_type)
		printf("Syntax error, quote is unclosed !\n");
	else
	{
		text = malloc(sizeof(char) * ((*i) - start + 1));
		j = 0;
		while (start < (*i))
			text[j++] = input[start++];
		text[j] = '\0';
		add_token(&tokens, WORD, text);
		free (text);
	}
	(*i)++;
}


void	handle_dquotesv2(t_token *tokens, char *input, int *i,char quote_type)
{
	char *quote_text;
	int	jx;
	int	quote_nb;

	jx = (*i);

	while (input[jx])
	{

	}

}


void	check_quotes(char	*input)
{
	int	ix;
	int	quote_nb;

	ix = 0;
	quote_nb = 0;
	while (input[ix])
	{
		if (input[ix] == '"' || input[ix] =='\'')
			quote_nb++;
		ix++;
	}
	return (quote_nb)
}