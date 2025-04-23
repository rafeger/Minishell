#include "minishell.h"

void	free_token_list(t_token *tok)
{
	t_token *tmp;
	while (tok)
	{
		tmp = tok->next;
		free(tok->value);
		free(tok->type);
		free(tok);
		tok = tmp;
	}
}

t_token	*new_token(t_token_type t_value, char *str)
{
	t_token	*new;
	new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->value = str;
	new->type = t_value;
	new->next = NULL;
	return (new);
}
void	append_tokens(t_token **head, t_token *new_tok)
{
	if (!*head)
		*head = new_tok;
	else
	{
		t_token	*tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_tok;
	}
}
void	handle_whitespaces(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}
t_token	*tokenizer(char *str)
{
	t_token *tokens = NULL;
	int i = 0;

	while (str[i])
	{
		if (ft_isspace(str[i]))
			i++;
		else if (str[i] == '|')
			append_tokens(&tokens, new_token(PIPE, tokens));
		else if (str[i] == '<')
			append_token(&tokens, new_token(INPUT, tokens));
		else if (str[i] == '>')
			append_token(&tokens, new_token(OUTPUT, tokens));
		else if (str[i] == '\'' || str[i] == '"')
			handle_quotes(str, &i);
		else
			handle_cmd(str, &i);
		i++;
	}
	append_tokens(&tokens, new_token(9, NULL));
	return (tokens);
}



void handle_quotes(char *str, int *i)
{
	char	*copy;
	if (str[*i] == '\'')
	{
		copy
	}
	else if (str[*i] == '"')
	{

	}
}
/* ls -l | grep test > outfile.txt | echo "hello world"*/

/*
token1 = ls
token2 = -l
token3 = |
token4 = grep
token5 = test
token6 = >
token7 = outfile.txt
token8 = |
token 9 = echo
token 10 = "hello world"
*/



int	main(void)
{
	char *str1 = "ls -l | grep test > outfile.txt | echo \"hello world\"";
	t_token *test = tokenizer(str1);
	t_token *tmp = test;
	while (tmp != NULL)
	{
		printf("%d\n%s\n", tmp->type, tmp->value);
		t_token *next = tmp->next;
		free(tmp);
		tmp = next;
	}
}