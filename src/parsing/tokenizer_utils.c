#include "../../minishell.h"

int	print_syntax_error(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	return (1);
}

int	check_syntax_errors(t_token *head)
{
	t_token	*curr;

	curr = head;
	if (!curr)
		return (print_syntax_error("Syntax error: empty command\n"));
	if (curr->type == T_PIPE)
		return (print_syntax_error("syntax error near unexpected token '|'\n"));
	while (curr)
	{
		if (curr->type == T_PIPE && curr->next && curr->next->type == T_PIPE)
			return (print_syntax_error("syntax error near unexpected token '|'\n"));
		if (curr->type == T_PIPE && !curr->next)
			return (print_syntax_error("syntax error: pipe at end of command\n"));
		if ((curr->type == T_REDIR_IN || curr->type == T_REDIR_OUT
				|| curr->type == T_REDIR_APPEND || curr->type == T_REDIR_HEREDOC)
			&& (!curr->next || curr->next->type != T_WORD))
			return (print_syntax_error("syntax error, expected arg\n"));
		if ((curr->type == T_REDIR_IN || curr->type == T_REDIR_OUT
				|| curr->type == T_REDIR_APPEND || curr->type == T_REDIR_HEREDOC)
			&& curr->next && (curr->next->type == T_REDIR_IN
				|| curr->next->type == T_REDIR_OUT || curr->next->type == T_REDIR_APPEND
				|| curr->next->type == T_REDIR_HEREDOC))
			return (print_syntax_error("Syntax error, redir error\n"));
		if (curr->type == T_INVALID)
			return (print_syntax_error("syntax error: invalid token\n"));
		curr = curr->next;
	}
	return (0);
}

char	*ft_strncpy(char *dest, const char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
