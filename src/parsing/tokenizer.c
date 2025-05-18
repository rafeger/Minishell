#include "../../minishell.h"


static t_token_type	get_token_type(char *str)
{
	char *temp = str;
	if (!ft_strcmp(temp, "|")) return T_PIPE;
	if (!ft_strcmp(temp, "<")) return T_REDIR_IN;
	if (!ft_strcmp(temp, ">")) return T_REDIR_OUT;
	if (!ft_strcmp(temp, ">>")) return T_REDIR_APPEND;
	if (!ft_strcmp(temp, "<<")) return T_REDIR_HEREDOC;
	return T_WORD;
}
static t_token	*new_token(char *str, t_token_type t_value)
{
	t_token	*new;
	new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->value = ft_strdup(str);
	if (!new->value)
	{
		free(new);
		return NULL;
	}
	new->type = t_value;
	new->next = NULL;
	return (new);
}
static int add_token(t_token **head, t_token *new)
{
	if (!new)
		return 0;
	if (!*head)
		*head = new;
	else
	{
		t_token *tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return 1;
}
t_token	*tokenize(char *input)
{
	t_token *head = NULL;
	int i = 0;

	while(input[i])
	{
		while(ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		
		char *word = extract_token(input, &i);
		if (!word)
		{
			free_token_list(head);
			return NULL;
		}
		t_token *new = new_token(word, get_token_type(word));
		free(word);
		if (!new || !add_token(&head, new))
		{
			free_token_list(head);
			return NULL;
		}
	}
	return (head);
}


